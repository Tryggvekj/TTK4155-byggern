/** ***************************************************************************
 * @file user_io.h
 * @author Byggve Klevstul-Jensen
 * @brief User I/O functions for interacting with the user
 * @version 0.1
 * @date 2025-09-11
 * 
 * @copyright Copyright (c) 2025 Byggarane
 * 
*******************************************************************************/


#pragma once

#define JOYSTICK_X_CHANNEL 0
#define JOYSTICK_Y_CHANNEL 1
#define TOUCHPAD_X_CHANNEL 2
#define TOUCHPAD_Y_CHANNEL 3
#define JOYSTICK_ADC_OUTP_MAX 247
#define JOYSTICK_ADC_OUTP_MIN 66
#define TOUCHPAD_ADC_OUTP_MAX 255
#define TOUCHPAD_ADC_OUTP_MIN 0

/** ***************************************************************************
 * @brief For storing x and y coordinates
 * 
*******************************************************************************/
typedef struct {
    uint8_t x;
    uint8_t y;
} x_y_coords;

/** ***************************************************************************
 * @brief Get the x and y coordinates for the joystick
 * 
 * @return x_y_coords 
*******************************************************************************/
x_y_coords get_joystick_x_y_percentage(void);

/** ***************************************************************************
 * @brief Get the x and y coordinates for the touchpad
 * 
 * @return x_y_coords 
*******************************************************************************/
x_y_coords get_touchpad_x_y_percentage(void);


void OLED_init();

void OLED_write();