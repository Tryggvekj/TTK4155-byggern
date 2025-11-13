/** ***************************************************************************
 * @file motor_ctrl.c
 * @author Magnus Carlsen Haaland, Tryggve Klevstul-Jensen, Walter Brynildsen
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

struct sam_gpio_pin motor_dir_pin = {
    .port = 'C',
    .pin = 23
};

struct {
    int16_t min_pos;
    int16_t max_pos;
} motor_cal;

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

int calibrate_motor(void) {
    int16_t last_val = get_encoder_value();
    
    // Calibrate min position
    do {
        set_motor_pos(0);
        last_val = get_encoder_value();
    } while (get_encoder_value() < last_val);
    motor_cal.min_pos = get_encoder_value();

    // Calibrate max position
    last_val = get_encoder_value();
    do {
        set_motor_pos(100);
        last_val = get_encoder_value();
    } while (get_encoder_value() > last_val);

    motor_cal.max_pos = get_encoder_value();
    return 0;
}

int get_encoder_value(void)
{
    return REG_TC2_CV0;
}

int get_motor_pos()
{
    int16_t encoder_val = get_encoder_value();
    int16_t pos_range = motor_cal.max_pos - motor_cal.min_pos;
    if (pos_range == 0) {
        printf("Motor not calibrated!\n");
        return 0; // Avoid division by zero
    }
    return (encoder_val - motor_cal.min_pos) * 100 / pos_range;
}

int motor_init(uint8_t period_us)
{
    int ret = sam_gpio_init(motor_dir_pin);
    if (ret) {
        return ret;
    }
    return pwm_init_us(period_us, MOTOR_PWM_CH);
}

void set_motor_dir(int joystick_value)
{

    if (joystick_value < MOTOR_DIR_THRESHOLD_LOW) {
        sam_gpio_set(motor_dir_pin, HIGH);
    } else if (joystick_value > MOTOR_DIR_THRESHOLD_HIGH) {
        sam_gpio_set(motor_dir_pin, LOW);
    }
}

void set_motor_pos(int joystick_value)
{
    uint8_t actual_pos = get_motor_pos();
    set_motor_dir(joystick_value);
    pwm_set_duty_cycle(joystick_value, MOTOR_PWM_CH);
}
