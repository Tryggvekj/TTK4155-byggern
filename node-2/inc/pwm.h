
#pragma once

#include <stdint.h>

#define PWM_CLOCK_FREQ 84000000
#define _delay(time) time_spinFor(msecs(time))

int pwm_init(uint8_t period_ms, uint8_t ch_num);

int pwm_set_duty_cycle(float duty_cycle_percentage, uint8_t ch_num);