/** ***************************************************************************
 * @file user_io.c
 * @author Byggve Klevstul-Jensen
 * @brief User I/O functions implementation file
 * @version 0.1
 * @date 2025-09-11
 * 
 * @copyright Copyright (c) 2025 Byggarane
 * 
*******************************************************************************/

#include <stdbool.h>
#include <stdint.h>

#include "adc.h"
#include "user_io.h"


/** ***************************************************************************
 * @brief Get the X and Y coordinates of the joystick, in percentages
 * 
 * @return x_y_coords Struct containing the percentages
*******************************************************************************/
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

/** ***************************************************************************
 * @brief Get the X and Y coordinates of the touchpad, in percentages
 * 
 * @return x_y_coords Struct containing the percentages
*******************************************************************************/
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

/** ***************************************************************************
 * @brief Get the direction of the joystick
 * 
 * @return enum joystick_direction The direction of the joystick
*******************************************************************************/
enum joystick_direction get_joystick_direction(void) {


    //TODO: Set sensible thresholds

    x_y_coords coords = get_joystick_x_y_percentage();

    if (coords.x > JOYSTICK_THRESHOLD_UPPER) {
        return JOYSTICK_RIGHT;
    } else if (coords.x < JOYSTICK_THRESHOLD_LOWER) {
        return JOYSTICK_LEFT;
    } else if (coords.y > JOYSTICK_THRESHOLD_UPPER) {
        return JOYSTICK_UP;
    } else if (coords.y < JOYSTICK_THRESHOLD_LOWER) {
        return JOYSTICK_DOWN;
    } else {
        return JOYSTICK_NEUTRAL;
    }
}

/** ***************************************************************************
 * @brief Get the state of the joystick button
 * 
 * @return bool True if pressed, false if not pressed
*******************************************************************************/
bool get_joystick_btn_state(void) {

    // TODO: Implement this
    return false;
}