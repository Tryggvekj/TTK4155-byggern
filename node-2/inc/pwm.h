/** ***************************************************************************
 * @file pwm.h
 * @author Magnus Carlsen Haaland, Tryggve Klevstul-Jensen, Walter Brynildsen
 * @brief PWM driver
 * @version 0.1
 * @date 2025-11-06
 *
 * @copyright Copyright (c) 2025 Byggarane
 *
 *******************************************************************************/

#pragma once

#include <stdint.h>

#define _delay(time) time_spinFor(msecs(time))

#define PWM_CLOCK_FREQ 84000000UL // 84 MHz main clock
#define SERVO_MIN_PW_MS 0.9f
#define SERVO_MAX_PW_MS 2.1f

/** ***************************************************************************
 * @brief Initialize PWM
 *
 * @param[in] period_ms PWM period in milliseconds
 * @return int 0 on success, negative errno on failure
 * @note Uses fixed channel number CH_NUM
 *******************************************************************************/
int pwm_init(uint8_t period_ms, uint8_t ch_num);

/** ***************************************************************************
 * @brief Initialize PWM with microsecond period
 *
 * @param[in] period_us PWM period in microseconds
 * @return int 0 on success, negative errno on failure
 * @note Uses fixed channel number CH_NUM
 *******************************************************************************/
int pwm_init_us(uint8_t period_us, uint8_t ch_num);

/** ***************************************************************************
 * @brief Set PWM pulse width in milliseconds
 *
 * @param[in] pulse_width_ms Pulse width in milliseconds
 * @return int 0 on success, negative errno on failure
 * @note Uses fixed channel number CH_NUM
 *******************************************************************************/
int pwm_set_pulse_width_ms(float pulse_width_ms, uint8_t ch_num);

/** ***************************************************************************
 * @brief Set PWM duty cycle
 *
 * @param[in] duty_cycle_percentage Duty cycle as a percentage (0.0 to 100.0)
 * @return int 0 on success, negative errno on failure
 * @note Uses fixed channel number CH_NUM
 *******************************************************************************/
int pwm_set_duty_cycle(float duty_cycle_percentage, uint8_t ch_num);