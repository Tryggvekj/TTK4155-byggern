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

#include "gpio.h"

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
 * 
 * @details Defines the possible directional states of the joystick
*******************************************************************************/
enum joystick_direction {
    JOYSTICK_NEUTRAL = 0,   /**< Joystick is in center/neutral position */
    JOYSTICK_UP,            /**< Joystick is pushed up */
    JOYSTICK_DOWN,          /**< Joystick is pushed down */
    JOYSTICK_LEFT,          /**< Joystick is pushed left */
    JOYSTICK_RIGHT          /**< Joystick is pushed right */
};

/** ***************************************************************************
 * @brief Enum for user I/O board commands
 * 
 * @details Command codes for communicating with the user I/O board via SPI
*******************************************************************************/
enum user_io_command {
    USER_IO_CMD_TOUCHPAD = 0x01,        /**< Command to read touchpad data */
    USER_IO_CMD_TOUCH_SLIDER = 0x02,    /**< Command to read touch slider data */
    USER_IO_CMD_JOYSTICK = 0x03,        /**< Command to read joystick data */
    USER_IO_CMD_BTNS = 0x04,            /**< Command to read button states */
    USER_IO_CMD_LED = 0x05,             /**< Command to control LEDs */
    USER_IO_CMD_LED_PWM = 0x06,         /**< Command to control LED PWM */
    USER_IO_CMD_INFO = 0x07             /**< Command to get board information */
};

/** ***************************************************************************
 * @brief Struct for storing button bitfields
 * 
 * @details Packed structure to represent button states from the I/O board.
 *          Uses union to allow both byte and bit-level access to button states.
*******************************************************************************/
struct buttons __attribute__((packed)) {
    union {
        uint8_t right;
        struct {
            uint8_t R1:1;
            uint8_t R2:1;
            uint8_t R3:1;
            uint8_t R4:1;
            uint8_t R5:1;
            uint8_t R6:1;
        };
    };
    union {
        uint8_t left;
        struct {
            uint8_t L1:1;
            uint8_t L2:1;
            uint8_t L3:1;
            uint8_t L4:1;
            uint8_t L5:1;
            uint8_t L6:1;
        };
    };
    union {
        uint8_t nav;
        struct {
            uint8_t NB:1;
            uint8_t NR:1;
            uint8_t ND:1;
            uint8_t NL:1;
            uint8_t NU:1;
        };
    };
};

/** ***************************************************************************
 * @brief For storing x and y coordinates
 * 
 * @details Structure to hold coordinate pairs, typically used for joystick
 *          and touchpad position data as percentages (0-100)
*******************************************************************************/
typedef struct {
    uint8_t x;  /**< X coordinate (0-100%) */
    uint8_t y;  /**< Y coordinate (0-100%) */
} x_y_coords;

/** ***************************************************************************
 * @brief Get the X and Y coordinates of the joystick, in percentages
 * 
 * @return x_y_coords Struct containing X and Y coordinates as percentages (0-100)
*******************************************************************************/
x_y_coords get_joystick_x_y_percentage(void);

/** ***************************************************************************
 * @brief Get the X and Y coordinates of the touchpad, in percentages
 * 
 * @return x_y_coords Struct containing X and Y coordinates as percentages (0-100)
*******************************************************************************/
x_y_coords get_touchpad_x_y_percentage(void);

/** ***************************************************************************
 * @brief Get the directional state of the joystick
 * 
 * @return enum joystick_direction Current joystick direction based on threshold values
*******************************************************************************/
enum joystick_direction get_joystick_direction(void);

/** ***************************************************************************
 * @brief Initialize joystick button pin
 * 
 * @param[in] _js_btn_pin GPIO pin structure for the joystick button
*******************************************************************************/
void joystick_btn_init(struct gpio_pin _js_btn_pin);

/** ***************************************************************************
 * @brief Get the state of the joystick button
 * 
 * @return bool True if button is pressed, false otherwise
*******************************************************************************/
bool get_joystick_btn_state(void);

/** ***************************************************************************
 * @brief Get the states of all buttons
 * 
 * @param[out] btn_states Pointer to buttons structure to store button states
 * @return bool True if SPI communication successful, false otherwise
*******************************************************************************/
bool get_button_states(struct buttons* btn_states);