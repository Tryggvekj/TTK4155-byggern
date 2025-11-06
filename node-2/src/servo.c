#include <stdio.h>
#include <errno.h>

#include "pwm.h"
#include "servo.h"


int set_servo_angle(float angle_degrees) {
    if (angle_degrees < 0.0f || angle_degrees > 180.0f) {
        printf("ERROR: %.2f degrees out of range (0-180)\r\n", angle_degrees);
        return -EINVAL;
    }

    // Map angle (0-180) to pulse width (0.9ms - 2.1ms)
    float pulse_width_ms = SERVO_MIN_PW_MS +
                           (angle_degrees / 180.0f) * (SERVO_MAX_PW_MS - SERVO_MIN_PW_MS);

    return pwm_set_pulse_width_ms(pulse_width_ms);
}