/** ***************************************************************************
 * @file motor_ctrl.h
 * @author Magnus Carlsen Haaland
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
#define MOTOR_DIR_PIN 23

/** ***************************************************************************
 * @brief Initialize motor encoder
 *
 *******************************************************************************/
void encoder_init(void);

/** ***************************************************************************
 * @brief Initialize motor control
 *
 * @param[in] period_ms PWM period in ms
 * @return int 0 on success, negative errno on failure
 *******************************************************************************/
int motor_init(uint8_t period_ms);

/** ***************************************************************************
 * @brief Set direction of motor
 *
 * @param dir Direction of motor encoded in the sign
 *******************************************************************************/
void set_motor_dir(int dir);

/** ***************************************************************************
 * @brief Set the motor position
 *
 * @param speed Motor speed, in percentage
 *******************************************************************************/
void set_motor_pos(int speed);

/** ***************************************************************************
 * @brief Get the encoder position
 *
 * @return int Encoder position
 *******************************************************************************/
int get_encoder_pos(void);