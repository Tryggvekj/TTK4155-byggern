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

int pwm_init(uint8_t period_ms, uint8_t ch_num)
{

    // Deactivate PIO and enable PWM peripheral on corresponding pin
    switch (ch_num)
    {
    case 0:
        // PB12
        PIOB->PIO_PDR |= PIO_PDR_P12;
        PIOB->PIO_ABSR |= PIO_PB12B_PWMH0;
        break;
    case 1:
        // PB13
        PIOB->PIO_PDR |= PIO_PDR_P13;
        PIOB->PIO_ABSR |= PIO_PB13B_PWMH1;
        break;
    case 2:
        // PB14
        PIOB->PIO_PDR |= PIO_PDR_P14;
        PIOB->PIO_ABSR |= PIO_PB14B_PWMH2;
        break;
    case 3:
        // PB15
        PIOB->PIO_PDR |= PIO_PDR_P15;
        PIOB->PIO_ABSR |= PIO_PB15B_PWMH3;
        break;
    }

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
    PWM->PWM_DIS = (1 << ch_num);
    while (PWM->PWM_SR & (1 << ch_num))
        ; // wait until fully disabled

    // Configure channel mode to use CLKA as source
    PWM->PWM_CH_NUM[ch_num].PWM_CMR = PWM_CMR_CPRE_CLKA | PWM_CMR_CPOL;

    // Set duty cycle to 0 initially
    PWM->PWM_CH_NUM[ch_num].PWM_CDTY = 0;

    // Set period
    PWM->PWM_CH_NUM[ch_num].PWM_CPRD = period_setting;

    // Enable the PWM channel
    PWM->PWM_ENA = (1 << ch_num);

    _delay(1);

    uint32_t cprd_readback = PWM->PWM_CH_NUM[ch_num].PWM_CPRD;

    return 0;
}

int pwm_init_us(uint8_t period_us, uint8_t ch_num)
{

    // Deactivate PIO and enable PWM peripheral on corresponding pin
    switch (ch_num)
    {
    case 0:
        // PB12
        PIOB->PIO_PDR |= PIO_PDR_P12;
        PIOB->PIO_ABSR |= PIO_PB12B_PWMH0;
        break;
    case 1:
        // PB13
        PIOB->PIO_PDR |= PIO_PDR_P13;
        PIOB->PIO_ABSR |= PIO_PB13B_PWMH1;
        break;
    case 2:
        // PB14
        PIOB->PIO_PDR |= PIO_PDR_P14;
        PIOB->PIO_ABSR |= PIO_PB14B_PWMH2;
        break;
    case 3:
        // PB15
        PIOB->PIO_PDR |= PIO_PDR_P15;
        PIOB->PIO_ABSR |= PIO_PB15B_PWMH3;
        break;
    }

    // Enable peripheral clock for PWM (ID = 36)
    PMC->PMC_PCER1 |= (1 << (ID_PWM - 32));

    // Configure PWM Clock A (CLKA = MCK / DIVA)
    // Choose DIVA = 42 -> CLKA = 84MHz / 42 = 2MHz
    PWM->PWM_CLK = PWM_CLK_PREA(0) | PWM_CLK_DIVA(42);

    // Compute period in microseconds (e.g. 20us at 2MHz = 40 ticks)
    uint32_t pwm_clk = PWM_CLOCK_FREQ / 42;
    uint32_t period_setting = (pwm_clk * period_us) / 1000000;

    // Disable write protection on PWM
    PWM->PWM_WPCR = PWM_WPCR_WPKEY(0x50574D); // "PWM"

    // Make sure channel is disabled before configuration
    PWM->PWM_DIS = (1 << ch_num);
    while (PWM->PWM_SR & (1 << ch_num))
        ; // wait until fully disabled

    // Configure channel mode to use CLKA as source
    PWM->PWM_CH_NUM[ch_num].PWM_CMR = PWM_CMR_CPRE_CLKA | PWM_CMR_CPOL;

    // Set duty cycle to 0 initially
    PWM->PWM_CH_NUM[ch_num].PWM_CDTY = 0;

    // Set period
    PWM->PWM_CH_NUM[ch_num].PWM_CPRD = period_setting;

    // Enable the PWM channel
    PWM->PWM_ENA = (1 << ch_num);

    _delay(1);

    uint32_t cprd_readback = PWM->PWM_CH_NUM[ch_num].PWM_CPRD;

    return 0;
}

int pwm_set_pulse_width_ms(float pulse_width_ms, uint8_t ch_num)
{

    // Check if channel is enabled
    if (!(PWM->PWM_SR & (1 << ch_num)))
    {
        printf("PWM channel %u not enabled!\r\n", ch_num);
        return -EINVAL;
    }

    // Convert pulse width in ms to register ticks
    uint32_t pwm_clk = PWM_CLOCK_FREQ / 42; // 2 MHz
    uint32_t duty_cycle_reg = (uint32_t)((pulse_width_ms / 1000.0f) * pwm_clk);

    PWM->PWM_CH_NUM[ch_num].PWM_CDTYUPD = duty_cycle_reg;

    return 0;
}

int pwm_set_duty_cycle(float duty_cycle_percentage, uint8_t ch_num)
{

    uint32_t period = PWM->PWM_CH_NUM[ch_num].PWM_CPRD;
    if (period == 0)
    {
        printf("ERROR: PWM not initialized on channel %u\r\n", ch_num);
        return -EINVAL;
    }

    float period_ms = ((float)period / (PWM_CLOCK_FREQ / 42)) * 1000.0f;

    // Compute pulse width in ms
    float pulse_width_ms = (duty_cycle_percentage / 100.0f) * period_ms;

    return pwm_set_pulse_width_ms(pulse_width_ms, ch_num);
}
