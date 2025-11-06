/** ***************************************************************************
 * @file pwm.c
 * @author Magnus Carlsen Haaland
 * @brief PWM driver
 * @version 0.1
 * @date 2025-11-06
 *
 * @copyright Copyright (c) 2025 Byggarane
 *
*******************************************************************************/

#include "pwm.h"
#include "sam.h"
#include <errno.h>
#include <time.h>
#include <stdio.h>


int pwm_init(uint8_t period_ms)
{

    // Deactivate PIO on PB13
    PIOB->PIO_PDR |= PIO_PDR_P13;
    PIOB->PIO_ABSR |= PIO_PB13B_PWMH1; // Peripheral B for PB13

    // Enable peripheral clock for PWM (ID = 36)
    PMC->PMC_PCER1 |= (1 << (ID_PWM - 32));

    // Enable peripheral clock for PWM (ID = 36)
    PMC->PMC_PCER1 |= (1 << (ID_PWM - 32));

    // Configure PWM Clock A (CLKA = MCK / DIVA)
    // Choose DIVA = 42 -> CLKA = 84MHz / 42 = 2MHz
    PWM->PWM_CLK = PWM_CLK_PREA(0) | PWM_CLK_DIVA(42);

    // Compute period (e.g. 20ms at 2MHz = 40,000 ticks)
    uint32_t pwm_clk = PWM_CLOCK_FREQ / 42;
    uint32_t period_setting = (pwm_clk * period_ms) / 1000;

    // Disable write protection on PWM
    PWM->PWM_WPCR = PWM_WPCR_WPKEY(0x50574D); // "PWM"

    // Make sure channel is disabled before configuration
    PWM->PWM_DIS = (1 << CH_NUM);
    while (PWM->PWM_SR & (1 << CH_NUM)); // wait until fully disabled

    // Configure channel mode to use CLKA as source
    PWM->PWM_CH_NUM[CH_NUM].PWM_CMR = PWM_CMR_CPRE_CLKA | PWM_CMR_CPOL;

    // Set duty cycle to 0 initially
    PWM->PWM_CH_NUM[CH_NUM].PWM_CDTY = 0;

    // Set period
    PWM->PWM_CH_NUM[CH_NUM].PWM_CPRD = period_setting;

    // Enable the PWM channel
    PWM->PWM_ENA = (1 << CH_NUM);

    _delay(1);

    uint32_t cprd_readback = PWM->PWM_CH_NUM[CH_NUM].PWM_CPRD;

    return 0;
}

int pwm_set_pulse_width_ms(float pulse_width_ms) {

    // Servo-safe range check
    if (pulse_width_ms < SERVO_MIN_PW_MS || pulse_width_ms > SERVO_MAX_PW_MS) {
        printf("ERROR: %.3f ms pulse width out of safe servo range (0.9–2.1)\r\n",
               pulse_width_ms);
        return -EINVAL;
    }

    // Check if channel is enabled
    if (!(PWM->PWM_SR & (1 << CH_NUM))) {
        printf("PWM channel %u not enabled!\r\n", CH_NUM);
        return -EINVAL;
    }

    // Convert pulse width in ms to register ticks
    uint32_t pwm_clk = PWM_CLOCK_FREQ / 42;  // 2 MHz
    uint32_t duty_cycle_reg = (uint32_t)((pulse_width_ms / 1000.0f) * pwm_clk);
    
    PWM->PWM_CH_NUM[CH_NUM].PWM_CDTYUPD = duty_cycle_reg;

    return 0;
}

int pwm_set_duty_cycle(float duty_cycle_percentage)
{

    uint32_t period = PWM->PWM_CH_NUM[CH_NUM].PWM_CPRD;
    if (period == 0) {
        printf("ERROR: PWM not initialized on channel %u\r\n", CH_NUM);
        return -EINVAL;
    }

    float period_ms = ((float)period / (PWM_CLOCK_FREQ / 42)) * 1000.0f;

    // Compute pulse width in ms
    float pulse_width_ms = (duty_cycle_percentage / 100.0f) * period_ms;

    return pwm_set_pulse_width_ms(pulse_width_ms);
}
