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
#include "gpio.h"
#include "spi.h"


static struct spi_device user_io_dev;
static struct gpio_pin js_btn_pin;


/** ***************************************************************************
 * @brief Initialize user I/O board
 * 
 * @param[in] _user_io_dev SPI device structure for the user I/O board
 * @param[in] _js_btn_pin GPIO pin structure for the joystick button
 * @return int 0 on success, negative error code on failure
*******************************************************************************/
int user_io_init(const struct spi_device _user_io_dev, const struct gpio_pin _js_btn_pin) {

    user_io_dev = _user_io_dev;
    js_btn_pin = _js_btn_pin;

    int res = spi_device_init(&user_io_dev);
    if (res != 0) {
        return res;
    }

    gpio_init(js_btn_pin, INPUT);

    return 0;
}

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
/** ***************************************************************************
 * @brief Get the directional state of the joystick
 * 
 * @return enum joystick_direction Current joystick direction based on threshold values
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

    return !gpio_get(js_btn_pin); // Active low
}

/** ***************************************************************************
 * @brief Get the states of all buttons
 * 
 * @param[out] btn_states Pointer to buttons structure to store button states
 * @return bool True if SPI communication successful, false otherwise
*******************************************************************************/
bool get_button_states(struct buttons* btn_states) {
    uint8_t input[1] = {USER_IO_CMD_BTNS};
    bool success = spi_query(&user_io_dev, input, 1, (uint8_t*)btn_states, sizeof(*btn_states));
    if (!success) {
        // SPI communication failed
        return false;
    }
    return true;
}

/** ***************************************************************************
 * @brief Get the joystick states from the user I/O board via SPI
 * 
 * @param[out] joystick_states Pointer to buttons structure to store joystick state data
 * @return bool True if SPI communication successful, false otherwise
*******************************************************************************/
bool get_joystick_states(struct buttons* joystick_states) {
    uint8_t input[1] = {USER_IO_CMD_JOYSTICK};
    bool success = spi_query(&user_io_dev, input, 1, (uint8_t*)joystick_states, sizeof(*joystick_states));
    if (!success) {
        // SPI communication failed
        return false;
    }
    return true;
}