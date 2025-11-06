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
 * @param angle_degrees Angle in degrees (0.0 to 180.0)
 * @return int 0 on success, negative errno on failure
*******************************************************************************/
int servo_set_angle(float angle_degrees);