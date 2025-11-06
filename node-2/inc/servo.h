#pragma once


/** ***************************************************************************
 * @brief Set servo angle
 * 
 * @param angle_degrees Angle in degrees (0.0 to 180.0)
 * @return int 0 on success, negative errno on failure
*******************************************************************************/
int set_servo_angle(float angle_degrees);