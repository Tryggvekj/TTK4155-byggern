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

#include "sam.h"

#include "adc.h"
#include "game.h"
#include "gpio.h"
#include "motor_ctrl.h"
#include "servo.h"

struct sam_gpio_pin solenoid_pin = {
    .port = 'B',
    .pin = 25,
};

int set_servo_from_js_can(CanMsg *msg)
{
    static struct xy_coords js;
    static struct xy_coords last_js;
    memcpy(js.y.bytes, &msg->byte[4], 4);
    if (abs(js.y.f - last_js.y.f) >= 3.0f)
    {
        servo_set_angle_percentage(js.y.f);
    }
    last_js.y.f = js.y.f;
    return 0;
}

int set_motor_from_js_can(CanMsg *msg)
{
    static struct xy_coords js;
    memcpy(js.x.bytes, &msg->byte[0], 4);
    set_motor_dir(js.x.f);
    set_motor_pos(js.x.f);

    return 0;
}

int set_solenoid_from_can(CanMsg *msg)
{
    bool state = msg->byte[0];
    printf("Solenoid state from CAN: %d\r\n", state);
    sam_gpio_set(solenoid_pin, state);

    return 0;
}

int check_game_over(CanMsg *msg)
{
    uint16_t adc_value = 0;
    adc_read(&adc_value);
    if (adc_value < IR_ADC_THRESHOLD)
    {
        msg->id = CAN_ID_IR_LED;
        msg->length = 1;
        msg->byte[0] = 0x01; // Game over signal
        can_tx(*msg);
        return 1; // Game over
    }
    return 0; // Game continues
}