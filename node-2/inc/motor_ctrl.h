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
 * @param[in] period_us PWM period in microseconds
 * @return int 0 on success, negative errno on failure
 *******************************************************************************/
int motor_init(uint8_t period_us);

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

/** ***************************************************************************
 * @brief Set PI target from joystick X (0-100)
 *
 * Maps joystick input to a signed speed target and requested direction.
 * Keeps existing open-loop API untouched.
 *
 * @param js_x Joystick X value (0-100)
 * @return int 0 on success
 ******************************************************************************/
int motor_pi_set_target_from_js(uint8_t js_x);

/** ***************************************************************************
 * @brief Periodic PI controller update
 *
 * Call frequently (e.g. in the main loop). The function is non-blocking and
 * self-schedules internally to run every ~10 ms.
 ******************************************************************************/
void motor_pi_update(void);