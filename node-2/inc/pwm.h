
#pragma once

#include <stdint.h>

#define PWM_CLOCK_FREQ 84000000
#define CH_NUM 1 // Using channel 1 (PB13 - PWMH1)
#define _delay(time) time_spinFor(msecs(time))

/** ***************************************************************************
 * @brief Initialize PWM
 * 
 * @param period_ms PWM period in milliseconds
 * @return int 0 on success, negative errno on failure
 * @note Uses fixed channel number CH_NUM
*******************************************************************************/
int pwm_init(uint8_t period_ms);

/** ***************************************************************************
 * @brief Set PWM duty cycle
 * 
 * @param duty_cycle_percentage Duty cycle as a percentage (0.0 to 100.0)
 * @return int 0 on success, negative errno on failure
 * @note Uses fixed channel number CH_NUM
*******************************************************************************/
int pwm_set_duty_cycle(float duty_cycle_percentage);