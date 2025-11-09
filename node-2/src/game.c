/** ***************************************************************************
 * @file game.c
 * @author Magnus Carlsen Haaland
 * @brief Game logic and functions
 * @version 0.1
 * @date 2025-11-09
 * 
 * @copyright Copyright (c) 2025 Byggarane
 * 
*******************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "game.h"
#include "servo.h"

int set_servo_from_js_can(CanMsg* msg) {
    static struct xy_coords js;
    static struct xy_coords last_js;
    memcpy(js.x.bytes, &msg->byte[0], 4);
    memcpy(js.y.bytes, &msg->byte[4], 4);
    printf("Joystick X: %.2f%%, Y: %.2f%%\r\n", js.x.f, js.y.f);
    if (abs(js.x.f - last_js.x.f) < 3.0f &&
        abs(js.y.f - last_js.y.f) < 3.0f) {
        // Ignore small changes
        return 0;
    }
    servo_set_angle_percentage(js.y.f);
    last_js.x.f = js.x.f;
    last_js.y.f = js.y.f;
}