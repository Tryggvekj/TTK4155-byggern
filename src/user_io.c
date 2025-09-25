/** ***************************************************************************
 * @file user_io.c
 * @author Magnuszszew Carlsewitz Haalski
 * @brief User I/O functions implementation file
 * @version 0.1
 * @date 2025-09-11
 * 
 * @copyright Copyright (c) 2025 Byggarane
 * 
*******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>

#include "user_io.h"
#include "xmem.h"
#include "gpio.h"
#include "adc.h"

x_y_coords get_joystick_x_y_percentage(void) {
    uint8_t x_val_analog = adc_read(JOYSTICK_X_CHANNEL);
    uint8_t y_val_analog = adc_read(JOYSTICK_Y_CHANNEL);
    
    uint8_t x_percentage = ((x_val_analog - JOYSTICK_ADC_OUTP_MIN) * 100) / (JOYSTICK_ADC_OUTP_MAX - JOYSTICK_ADC_OUTP_MIN);
    uint8_t y_percentage = ((y_val_analog - JOYSTICK_ADC_OUTP_MIN) * 100) / (JOYSTICK_ADC_OUTP_MAX - JOYSTICK_ADC_OUTP_MIN);

    x_y_coords coords;
    coords.x = x_percentage;
    coords.y = y_percentage;

    return coords;
}

x_y_coords get_touchpad_x_y_percentage(void) {
    uint8_t x_val_analog = adc_read(TOUCHPAD_X_CHANNEL);
    uint8_t y_val_analog = adc_read(TOUCHPAD_Y_CHANNEL);
    
    uint8_t x_percentage = ((x_val_analog - TOUCHPAD_ADC_OUTP_MIN) * 100) / (TOUCHPAD_ADC_OUTP_MAX - TOUCHPAD_ADC_OUTP_MIN);
    uint8_t y_percentage = ((y_val_analog - TOUCHPAD_ADC_OUTP_MIN) * 100) / (TOUCHPAD_ADC_OUTP_MAX - TOUCHPAD_ADC_OUTP_MIN);

    x_y_coords coords;
    coords.x = x_percentage;
    coords.y = y_percentage;

    return coords;
}