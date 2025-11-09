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


/** ***************************************************************************
 * @brief X-Y coordinate struct 
*******************************************************************************/
struct xy_coords {
    union {
        float f;
        uint8_t bytes[4];
    } x;
    union {
        float f;
        uint8_t bytes[4];
    } y;
};

/** ***************************************************************************
 * @brief Set the servo angle from joystick percentage in CAN message
 * 
 * @param msg CAN message containing the joystick position
 * @return int 0 on success, negative error code on failure
*******************************************************************************/
int set_servo_from_js_can(CanMsg* msg);