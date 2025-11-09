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

#include "game.h"
#include "gpio.h"
#include "servo.h"

#define SOLENOID_PIN 25

struct sam_gpio_pin solenoid_pin = {
    .port = 'B',
    .pin = 25
};

int solenoid_init(void) {
    PMC->PMC_PCER0 |= (1 << 12); // Activate clock for PIOB
 
    PIOB->PIO_PER |= (1 << SOLENOID_PIN); // Activate control over pin B25
 
    PIOB->PIO_OER |= (1 << SOLENOID_PIN); // Activate output
 
    PIOB->PIO_SODR |= (1 << SOLENOID_PIN); // Clear output register

    return 0;
}

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

int set_solenoid_from_can(CanMsg* msg) {
    bool state = msg->byte[0];
    printf("Solenoid state from CAN: %d\r\n", state);
    if(state) {
        PIOB->PIO_CODR |= (1 << SOLENOID_PIN); // Activate solenoid
        printf("Solenoid activated\r\n");
    } else {
        PIOB->PIO_SODR |= (1 << SOLENOID_PIN); // Deactivate solenoid
        printf("Solenoid deactivated\r\n");
    }
    return 0;
}