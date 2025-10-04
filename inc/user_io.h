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

#define JOYSTICK_THRESHOLD_UPPER 55
#define JOYSTICK_THRESHOLD_LOWER 45


/** ***************************************************************************
 * @brief Enum for joystick directions
*******************************************************************************/
enum joystick_direction {
    JOYSTICK_NEUTRAL = 0,
    JOYSTICK_UP,
    JOYSTICK_DOWN,
    JOYSTICK_LEFT,
    JOYSTICK_RIGHT
};

/** ***************************************************************************
 * @brief For storing x and y coordinates
 * 
*******************************************************************************/
typedef struct {
    uint8_t x;
    uint8_t y;
} x_y_coords;

/** ***************************************************************************
 * @brief Get the X and Y coordinates of the joystick, in percentages
 * 
 * @return x_y_coords Struct containing the percentages
*******************************************************************************/
x_y_coords get_joystick_x_y_percentage(void);

/** ***************************************************************************
 * @brief Get the X and Y coordinates of the touchpad, in percentages
 * 
 * @return x_y_coords Struct containing the percentages
*******************************************************************************/
x_y_coords get_touchpad_x_y_percentage(void);