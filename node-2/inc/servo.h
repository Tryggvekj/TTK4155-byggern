/** ***************************************************************************
 * @file servo.h
 * @author Magnus Carlsen Haaland, Tryggve Klevstul-Jensen, Walter Brynildsen
 * @brief Servo motor driver
 * @version 0.1
 * @date 2025-11-06
 *
 * @copyright Copyright (c) 2025 Byggarane
 *
 *******************************************************************************/

#pragma once

#define SERVO_PWM_CH 1

/** ***************************************************************************
 * @brief Initialize servo
 *
 * @param[in] period_ms PWM period in ms
 * @return int 0 on success, negative errno on failure
 *******************************************************************************/
int servo_init(uint8_t period_ms);

/** ***************************************************************************
 * @brief Set servo angle
 *
 * @param[in] angle_degrees Angle in degrees (0.0 to 180.0)
 * @return int 0 on success, negative errno on failure
 *******************************************************************************/
int servo_set_angle(float angle_degrees);

/** ***************************************************************************
 * @brief Set servo angle by percentage
 *
 * @param[in] angle_percentage Angle as a percentage (0 to 100)
 * @return int 0 on success, negative errno on failure
 *******************************************************************************/
int servo_set_angle_percentage(uint8_t angle_percentage);

/** ***************************************************************************
 * @brief Test servo by sweeping from 0 to 180 degrees
 *
 * @return int 0 on success, negative errno on failure
 *******************************************************************************/
int servo_test(void);