#include "pwm.h"
#include "sam.h"
#include <errno.h>
#include <time.h>

int pwm_init(uint8_t period_ms, uint8_t ch_num) {
    if (ch_num > 7) {
        return -EINVAL;
    }

    uint32_t prescaler = 1;
    uint32_t period_setting = (PWM_CLOCK_FREQ / prescaler) / (1000 * period_ms);
    printf("PWM period setting: %u\r\n", period_setting);

    // REG_PWM_WPCR = 0x50574D00 | (0b111111 << 2); // Disable write protection
    //REG_PWM_OSS = (1 << ch_num) | (1 << (ch_num + 16)); // Output override single step
    PWM->PWM_WPCR = 0x50574DFC; // Disable write protection
    _delay(10);
    PWM->PWM_OSS = (1 << ch_num) | (1 << (ch_num + 16)); // Output override single step
    PWM->PWM_ENA = 0; // Disable all PWM channels before configuration
    PWM->PWM_CH_NUM[ch_num].PWM_CMR = 0x0; // Set channel mode register to up-counting mode
    PWM->PWM_CH_NUM[ch_num].PWM_CPRD = period_setting;
    
    // REG_PWM_ENA |= (1 << ch_num); // Enable PWM channel    
    PWM->PWM_ENA |= 1 << ch_num; // Enable PWM channel

    volatile uint32_t WPSR = REG_PWM_WPSR;
    printf("PWM WPSR: %u\r\n", WPSR);

    return 0;
}

int pwm_set_duty_cycle(float duty_cycle_percentage, uint8_t ch_num) {
    if (ch_num > 7 || duty_cycle_percentage > 100 || duty_cycle_percentage < 0) {
        return -EINVAL;
    }

    uint32_t period = PWM->PWM_CH_NUM[ch_num].PWM_CPRD;
    printf("Period reg: %u\r\n", period);

    float period_ms = ((float)period / PWM_CLOCK_FREQ) * 1000;
    printf("Period ms: %f\r\n", period_ms);

    float duty_cycle_ms = (1 - duty_cycle_percentage / 100.0) * period_ms;
    printf("Duty cycle ms: %f\r\n", duty_cycle_ms);
    
    if (duty_cycle_ms > period_ms || duty_cycle_ms < 0.9 || duty_cycle_ms > 2.1)
    {
        printf("Duty cycle out of range: %f ms\r\n", duty_cycle_ms);
        return -EINVAL;
    }
    uint32_t duty_cycle_reg = (1 - duty_cycle_percentage / 100.0) * period_ms * PWM_CLOCK_FREQ;
    PWM->PWM_CH_NUM[ch_num].PWM_CDTY = duty_cycle_reg;

    /*switch (ch_num)
    {
    case 0:
        period_reg = REG_PWM_CPRD0;
        printf("Period reg: %u\r\n", period_reg);
        period_ms = period_reg / channel_clk * 1000;
        printf("Period ms: %u\r\n", period_ms);
        duty_cycle_ms = (1 - duty_cycle_percentage / 100.0) * period_ms;
        printf("Duty cycle ms: %f\r\n", duty_cycle_ms);
        if (duty_cycle_ms > period_ms || duty_cycle_ms < 0.9 || duty_cycle_ms > 2.1)
        {
            printf("Duty cycle out of range: %f ms\r\n", duty_cycle_ms);
            return -EINVAL;
        }
        duty_cycle_reg = (1 - duty_cycle_percentage / 100.0) * period_ms * channel_clk;
        REG_PWM_CDTY0 = duty_cycle_reg;
        break;
    case 1:
        period_reg = REG_PWM_CPRD1;
        printf("Period reg: %u\r\n", period_reg);
        period_ms = period_reg / channel_clk * 1000;
        printf("Period ms: %u\r\n", period_ms);
        duty_cycle_ms = (1 - duty_cycle_percentage / 100.0) * period_ms;
        printf("Duty cycle ms: %f\r\n", duty_cycle_ms);
        if (duty_cycle_ms > period_ms || duty_cycle_ms < 0.9 || duty_cycle_ms > 2.1)
        {
            printf("Duty cycle out of range: %f ms\r\n", duty_cycle_ms);
            return -EINVAL;
        }
        duty_cycle_reg = (1 - duty_cycle_percentage / 100.0) * period_ms * channel_clk;
        REG_PWM_CDTY1 = duty_cycle_reg;
        break;
    case 2:
        period_reg = REG_PWM_CPRD2;
        period_ms = period_reg / channel_clk * 1000;
        duty_cycle_ms = (1 - duty_cycle_percentage / 100.0) * period_ms;
        if (duty_cycle_ms > period_ms || duty_cycle_ms < 0.9 || duty_cycle_ms > 2.1)
        {
            printf("Duty cycle out of range: %f ms\r\n", duty_cycle_ms);
            return -EINVAL;
        }
        duty_cycle_reg = (1 - duty_cycle_percentage / 100.0) * period_ms * channel_clk;
        REG_PWM_CDTY2 = duty_cycle_reg;
        break;
    case 3:
        period_reg = REG_PWM_CPRD3;
        period_ms = period_reg / channel_clk * 1000;
        duty_cycle_ms = (1 - duty_cycle_percentage / 100.0) * period_ms;
        if (duty_cycle_ms > period_ms || duty_cycle_ms < 0.9 || duty_cycle_ms > 2.1)
        {
            printf("Duty cycle out of range: %f ms\r\n", duty_cycle_ms);
            return -EINVAL;
        }
        duty_cycle_reg = (1 - duty_cycle_percentage / 100.0) * period_ms * channel_clk;
        REG_PWM_CDTY3 = duty_cycle_reg;
        break;
    case 4:
        period_reg = REG_PWM_CPRD4;
        period_ms = period_reg / channel_clk * 1000;
        duty_cycle_ms = (1 - duty_cycle_percentage / 100.0) * period_ms;
        if (duty_cycle_ms > period_ms || duty_cycle_ms < 0.9 || duty_cycle_ms > 2.1)
        {
            printf("Duty cycle out of range: %f ms\r\n", duty_cycle_ms);
            return -EINVAL;
        }
        duty_cycle_reg = (1 - duty_cycle_percentage / 100.0) * period_ms * channel_clk;
        REG_PWM_CDTY4 = duty_cycle_reg;
        break;
    case 5:
        period_reg = REG_PWM_CPRD5;
        period_ms = period_reg / channel_clk * 1000;
        duty_cycle_ms = (1 - duty_cycle_percentage / 100.0) * period_ms;
        if (duty_cycle_ms > period_ms || duty_cycle_ms < 0.9 || duty_cycle_ms > 2.1)
        {
            printf("Duty cycle out of range: %f ms\r\n", duty_cycle_ms);
            return -EINVAL;
        }
        duty_cycle_reg = (1 - duty_cycle_percentage / 100.0) * period_ms * channel_clk;
        REG_PWM_CDTY5 = duty_cycle_reg;
        break;
    case 6:
        period_reg = REG_PWM_CPRD6;
        period_ms = period_reg / channel_clk * 1000;
        duty_cycle_ms = (1 - duty_cycle_percentage / 100.0) * period_ms;
        if (duty_cycle_ms > period_ms || duty_cycle_ms < 0.9 || duty_cycle_ms > 2.1)
        {
            printf("Duty cycle out of range: %f ms\r\n", duty_cycle_ms);
            return -EINVAL;
        }
        duty_cycle_reg = (1 - duty_cycle_percentage / 100.0) * period_ms * channel_clk;
        REG_PWM_CDTY6 = duty_cycle_reg;
        break;
    case 7:
        period_reg = REG_PWM_CPRD7;
        period_ms = period_reg / channel_clk * 1000;
        duty_cycle_ms = (1 - duty_cycle_percentage / 100.0) * period_ms;
        if (duty_cycle_ms > period_ms || duty_cycle_ms < 0.9 || duty_cycle_ms > 2.1)
        {
            printf("Duty cycle out of range: %f ms\r\n", duty_cycle_ms);
            return -EINVAL;
        }
        duty_cycle_reg = (1 - duty_cycle_percentage / 100.0) * period_ms * channel_clk;
        REG_PWM_CDTY7 = duty_cycle_reg;
        break;
    default:
        return -EINVAL;
    }*/

    return 0;
}