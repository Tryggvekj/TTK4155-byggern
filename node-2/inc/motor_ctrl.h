/** ***************************************************************************
 * @file motor_ctrl.h
 * @author Magnus Carlsen Haaland, Tryggve Klevstul-Jensen, Walter Brynildsen
 * @brief Driver for the motor controller
 * @version 0.1
 * @date 2025-11-09
 *
 * @copyright Copyright (c) 2025 Byggarane
 *
 *******************************************************************************/

#pragma once

#include "gpio.h"

#define MOTOR_PWM_CH 0
#define MOTOR_DIR_THRESHOLD_LOW 45
#define MOTOR_DIR_THRESHOLD_HIGH 55

/** ***************************************************************************
 * @brief Initialize motor encoder
 *
 ******************************************************************************/
void encoder_init(void);

/** ***************************************************************************
 * @brief Calibrate the motor
 *
 * @return int 0 on success, negative errno on failure
 * @details Sets the offsets for max and min positions of the motor
 ******************************************************************************/
int calibrate_motor(void);

/** ***************************************************************************
 * @brief Initialize motor control
 *
 * @param[in] period_us PWM period in microseconds
 * @return int 0 on success, negative errno on failure
 ******************************************************************************/
int motor_init(uint8_t period_us);

/** ***************************************************************************
 * @brief Set direction of motor
 *
 * @param dir Direction of motor encoded in the sign
 ******************************************************************************/
void set_motor_dir(bool dir);

/** ***************************************************************************
 * @brief Set the motor position
 *
 * @param joystick_value Joystick value controlling motor position, as a percentage (0-100)
 ******************************************************************************/
void set_motor_pos(int joystick_value);

/** ***************************************************************************
 * @brief Get the encoder value
 *
 * @return int Encoder value
 ******************************************************************************/
int get_encoder_value(void);