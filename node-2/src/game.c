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
    js.y = msg->byte[1];
    if (abs(js.y - last_js.y) >= 3)
    {
        printf("Y: %u\r\n", js.y);
        servo_set_angle_percentage(js.y);
    }
    last_js.y = js.y;
    return 0;
}

int set_motor_from_js_can(CanMsg *msg)
{
    static struct xy_coords js;
    js.x = msg->byte[0];
    printf("X: %u\r\n", js.x);
    // Closed-loop: set PI target instead of open-loop duty
    motor_pi_set_target_from_js(js.x);

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
        printf("IR diode ADC value: %d\r\n", adc_value);
        return 1; // Game over
    }
    return 0; // Game continues
}

int send_game_over(CanMsg *msg)
{
    msg->id = CAN_ID_IR_LED;
    msg->length = 1;
    msg->byte[0] = 10; // Game over signal
    can_tx(*msg);

    return 0; // Game continues
}