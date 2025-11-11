/** ***************************************************************************
 * @file game.h
 * @author Magnus Carlsen Haaland
 * @brief Game logic and functions
 * @version 0.1
 * @date 2025-11-09
 *
 * @copyright Copyright (c) 2025 Byggarane
 *
 *******************************************************************************/

#pragma once

#include <stdint.h>

#include "can.h"

#define IR_ADC_THRESHOLD 1

/** ***************************************************************************
 * @brief X-Y coordinate struct
 *******************************************************************************/
struct xy_coords
{
    uint8_t x;
    uint8_t y;
};

/** ***************************************************************************
 * @brief Set the servo angle from joystick percentage in CAN message
 *
 * @param msg CAN message containing the joystick position
 * @return int 0 on success, negative error code on failure
 *******************************************************************************/
int set_servo_from_js_can(CanMsg *msg);

/** ***************************************************************************
 * @brief Set solenoid state from joystick button state in CAN message
 * @param msg CAN message containing the joystick button state
 * @return int 0 on success, negative error code on failure
 *******************************************************************************/
int set_solenoid_from_can(CanMsg *msg);

/** ***************************************************************************
 * @brief Set motor direction and speed from joystick value in CAN message
 *
 * @param msg CAN message containing the joystick position
 * @return int 0 on success, negative error code on failure
 *******************************************************************************/
int set_motor_from_js_can(CanMsg *msg);

/** ***************************************************************************
 * @brief Check if the game is over based on ADC value
 *
 * @param msg Pointer to CAN message to send game over signal
 * @return int 1 if game is over, 0 if game continues
 * @note Sends a CAN message with game over signal to node 1
 *******************************************************************************/
int check_game_over(CanMsg *msg);