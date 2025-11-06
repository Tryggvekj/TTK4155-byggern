
#pragma once

#include <stdint.h>

#define _delay(time) time_spinFor(msecs(time))

#define PWM_CLOCK_FREQ 84000000UL  // 84 MHz main clock
#define CH_NUM 1 // Using channel 1 (PB13 - PWMH1)
#define SERVO_MIN_PW_MS 0.9f
#define SERVO_MAX_PW_MS 2.1f

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