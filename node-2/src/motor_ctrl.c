/** ***************************************************************************
 * @file motor_ctrl.c
 * @author Magnus Carlsen Haaland
 * @brief Driver for the motor controller
 * @version 0.1
 * @date 2025-11-09
 *
 * @copyright Copyright (c) 2025 Byggarane
 *
 *******************************************************************************/

#include <stdint.h>
#include "sam.h"

#include "gpio.h"
#include "motor_ctrl.h"
#include "pwm.h"
#include "time.h"

#define F_CPU 84000000

struct sam_gpio_pin motor_dir_pin = {
    .port = 'C',
    .pin = 23};

// --- PI controller state and constants ---
// Control period in milliseconds
#define MOTOR_PI_PERIOD_MS 10

// Max encoder counts per control period at full speed (tune this!)
// This depends on motor+encoder+gearbox. Start with a conservative value.
#define MOTOR_MAX_COUNTS_PER_PERIOD 500

// PI gains (tune as needed)
#define MOTOR_KP 0.5f
#define MOTOR_KI 0.10f

static uint8_t s_target_percent = 0;    // 0-100 (magnitude)
static int8_t s_target_sign = 0;        // -1, 0, +1
static float s_integral = 0.0f;         // integral of normalized error
static uint32_t s_last_pos = 0;         // last encoder position
static uint8_t s_init_done = 0;         // lazy init flag
static uint64_t s_next_run = 0;         // next scheduled run (ticks)
static float s_target_percent_f = 0.0f; // smoothed target percent

// Joystick shaping: less sensitive near center, responsive to big moves
#define JS_CENTER 50
#define JS_DEADBAND 8     // +/- around center
#define JS_FASTSTEP 25.0f // if target changes more than this, jump immediately
#define JS_SMOOTH_A 0.2f  // EMA factor for small changes

void encoder_init(void)
{
    // activate clock for the Timer counter- module in Power management controller
    // Peripheral ID = 29 (PWM) --> PMC_PCER0 bit 4
    PMC->PMC_PCER0 |= PMC_PCER0_PID29; // setting bit nr 29 high as it corresponds to 29
    PMC->PMC_PCER0 |= (1 << ID_PIOC);
    PMC->PMC_PCER1 |= (1 << (ID_TC6 - 32));
    PIOC->PIO_PDR |= (PIO_PC25 | PIO_PC26);  // deactivating PIO, opening pin PC25&PC26 for perihperal
    PIOC->PIO_ABSR |= (PIO_PC25 | PIO_PC26); // setting peripheral function B at pin PC25&26

    // PMC->PMC_PCER0 |= (1 << ID_PIOC); // Activate clock for PIOC

    // deactivating write protection
    TC2->TC_WPMR = (0x54494D << TC_WPMR_WPKEY_Pos); // WPKEY =0x54494D

    TC2->TC_CHANNEL[0].TC_CMR &= ~TC_CMR_WAVE;          // enable capture mode
    TC2->TC_CHANNEL[0].TC_CMR |= TC_CMR_TCCLKS_XC0;     // setting XC0 as clock
    TC2->TC_CHANNEL[0].TC_CMR |= TC_CMR_ETRGEDG_RISING; // triggering edge
    TC2->TC_CHANNEL[0].TC_CMR |= TC_CMR_ABETRG;         // aetting external trigger as TIOA

    // setting block mode register
    TC2->TC_BMR = 0;
    TC2->TC_BMR |= TC_BMR_QDEN;
    TC2->TC_BMR |= TC_BMR_POSEN;
    // TC2 -> TC_BMR |= TC_BMR_SPEEDEN;
    TC2->TC_BMR |= TC_BMR_EDGPHA;
    TC2->TC_BMR |= TC_BMR_MAXFILT(5);
    PMC->PMC_PCER0 |= PMC_PCER0_PID12;

    // starting timer counter 2 (TC2)
    TC2->TC_CHANNEL[0].TC_CCR = (TC_CCR_CLKEN | TC_CCR_SWTRG);

    PIOC->PIO_PUDR |= (PIO_PC25 | PIO_PC26);
    ; // disabling pull up resister on I/O line
    PIOC->PIO_IFER |= (PIO_PC25 | PIO_PC26);
    ; // enables input glitch filter on I/O-line

    REG_TC2_WPMR = 0b1 | 0x54494D;
    REG_TC2_CCR0 = 0b101;
    REG_TC2_CMR0 = 5;
    REG_TC2_BMR = (1 << 8) | (1 << 9);

    // reactivating write protection
    // TC2 -> TC_WPMR = ((0x54494D << 8) | 1);
    TC2->TC_WPMR |= 1;
}

int get_encoder_pos(void)
{
    return REG_TC2_CV0;
}

int motor_init(uint8_t period_us)
{
    sam_gpio_init(motor_dir_pin);
    return pwm_init_us(period_us, MOTOR_PWM_CH);
}

void set_motor_dir(int joystick_value)
{

    if (joystick_value < 45)
    {
        sam_gpio_set(motor_dir_pin, 1);
    }
    else if (joystick_value > 55)
    {
        sam_gpio_set(motor_dir_pin, 0);
    }
}

void set_motor_pos(int joystick_value)
{
    set_motor_dir(joystick_value);
    pwm_set_duty_cycle(joystick_value, MOTOR_PWM_CH);
}

int motor_pi_set_target_from_js(uint8_t js_x)
{
    // Map joystick to direction using deadband around center
    int16_t centered = (int16_t)js_x - JS_CENTER;
    int16_t abs_c = centered < 0 ? -centered : centered;
    if (abs_c <= JS_DEADBAND)
    {
        s_target_sign = 0;
    }
    else
    {
        s_target_sign = (centered < 0) ? -1 : 1;
    }

    // Magnitude after deadband, normalized to [0,1]
    int16_t mag_raw = abs_c - JS_DEADBAND;
    if (mag_raw < 0)
        mag_raw = 0;
    int16_t mag_den = (50 - JS_DEADBAND);
    float mag_norm = (mag_den > 0) ? ((float)mag_raw / (float)mag_den) : 0.0f;
    if (mag_norm > 1.0f)
        mag_norm = 1.0f;

    // Non-linear curve (square) for less sensitivity near center
    float target_raw = (mag_norm * mag_norm) * 100.0f; // 0..100

    // Smooth small changes, jump on large steps to keep responsiveness
    if (!s_init_done)
    {
        s_target_percent_f = target_raw;
    }
    else
    {
        float diff = target_raw - s_target_percent_f;
        float adiff = diff < 0 ? -diff : diff;
        if (adiff > JS_FASTSTEP)
        {
            s_target_percent_f = target_raw; // react quickly to big moves
        }
        else
        {
            s_target_percent_f += JS_SMOOTH_A * diff; // smooth small moves
        }
    }
    // Quantize to 0..100 for duty percent
    if (s_target_percent_f < 0.0f)
        s_target_percent_f = 0.0f;
    if (s_target_percent_f > 100.0f)
        s_target_percent_f = 100.0f;
    s_target_percent = (uint8_t)(s_target_percent_f + 0.5f);

    // Initialize scheduling on first call
    if (!s_init_done)
    {
        s_last_pos = (uint32_t)REG_TC2_CV0;
        s_next_run = time_now() + msecs(MOTOR_PI_PERIOD_MS);
        s_init_done = 1;
    }
    return 0;
}

void motor_pi_update(void)
{
    if (!s_init_done)
    {
        // Not configured yet
        return;
    }

    uint64_t now = time_now();
    if (now < s_next_run)
    {
        return; // not time yet
    }
    s_next_run += msecs(MOTOR_PI_PERIOD_MS);

    // Read encoder position and compute counts in the last period
    uint32_t pos = (uint32_t)REG_TC2_CV0;
    int32_t delta = (int32_t)(pos - s_last_pos); // handles wrap with unsigned arithmetic
    s_last_pos = pos;

    // We run at fixed period, so measured_counts ≈ delta per period
    float measured_counts = (float)(delta >= 0 ? delta : -delta);

    // Compute target counts for this period based on target percent
    float target_counts = ((float)s_target_percent / 100.0f) * (float)MOTOR_MAX_COUNTS_PER_PERIOD;

    // Speed error (counts per period). Use magnitude only; direction handled separately
    float error_counts = target_counts - measured_counts;

    // Normalize error to [-1,1] scale for gain consistency
    float error_norm = 0.0f;
    if (MOTOR_MAX_COUNTS_PER_PERIOD > 0)
    {
        error_norm = error_counts / (float)MOTOR_MAX_COUNTS_PER_PERIOD;
    }

    // Update integral term with simple anti-windup clamp
    s_integral += error_norm;
    if (s_integral > 1.0f)
        s_integral = 1.0f;
    if (s_integral < -1.0f)
        s_integral = -1.0f;

    // PI output around feedforward = target_percent
    float duty = (float)s_target_percent + (MOTOR_KP * error_norm + MOTOR_KI * s_integral) * 100.0f;

    // Clamp to [0,100]
    if (duty < 0.0f)
        duty = 0.0f;
    if (duty > 100.0f)
        duty = 100.0f;

    // Set direction according to target sign using existing helper
    // Use representative joystick values to respect threshold logic
    if (s_target_sign < 0)
    {
        set_motor_dir(0); // reverse
    }
    else if (s_target_sign > 0)
    {
        set_motor_dir(100); // forward
    }
    else
    {
        // Deadband: no movement requested
        duty = 0.0f;
        set_motor_dir(50); // neutral (no change)
    }

    pwm_set_duty_cycle(duty, MOTOR_PWM_CH);
}
