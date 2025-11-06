/** ***************************************************************************
 * @file servo.c
 * @author Magnus Carlsen Haaland
 * @brief Servo motor driver
 * @version 0.1
 * @date 2025-11-06
 *
 * @copyright Copyright (c) 2025 Byggarane
 *
*******************************************************************************/

#include <stdio.h>
#include <errno.h>

#include "pwm.h"
#include "servo.h"
#include "time.h"


int servo_set_angle(float angle_degrees) {
    if (angle_degrees < 0.0f || angle_degrees > 180.0f) {
        printf("ERROR: %.2f degrees out of range (0-180)\r\n", angle_degrees);
        return -EINVAL;
    }

    // Map angle (0-180) to pulse width (0.9ms - 2.1ms)
    float pulse_width_ms = SERVO_MIN_PW_MS +
                           (angle_degrees / 180.0f) * (SERVO_MAX_PW_MS - SERVO_MIN_PW_MS);

    return pwm_set_pulse_width_ms(pulse_width_ms);
}

int servo_set_angle_percentage(float angle_percentage) {
    if (angle_percentage < 0.0f || angle_percentage > 100.0f) {
        printf("ERROR: %.2f%% angle out of range (0-100)\r\n", angle_percentage);
        return -EINVAL;
    }

    // Map percentage (0-100) to angle (0-180)
    float angle_degrees = (angle_percentage / 100.0f) * 180.0f;

    return servo_set_angle(angle_degrees);
}

int servo_test(void) {
    for (float angle = 0.0f; angle <= 180.0f; angle += 0.1f) {
        printf("Setting servo to %f degrees\r\n", angle);
        int ret = servo_set_angle(angle);
        if (ret) {
            printf("Failed to set servo angle: %d\r\n", ret);
            return ret;
        }
        _delay(1);
    }
    return 0;
}