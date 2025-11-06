/** ***************************************************************************
 * @file servo.h
 * @author Magnus Carlsen Haaland
 * @brief Servo motor driver
 * @version 0.1
 * @date 2025-11-06
 * 
 * @copyright Copyright (c) 2025 Byggarane
 * 
*******************************************************************************/

#pragma once


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
 * @param[in] angle_percentage Angle as a percentage (0.0 to 100.0)
 * @return int 0 on success, negative errno on failure
*******************************************************************************/
int servo_set_angle_percentage(float angle_percentage);

/** ***************************************************************************
 * @brief Test servo by sweeping from 0 to 180 degrees
 * 
 * @return int 0 on success, negative errno on failure
*******************************************************************************/
int servo_test(void);