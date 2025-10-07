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
*******************************************************************************/
enum joystick_direction {
    JOYSTICK_NEUTRAL = 0,
    JOYSTICK_UP,
    JOYSTICK_DOWN,
    JOYSTICK_LEFT,
    JOYSTICK_RIGHT
};

/** ***************************************************************************
 * @brief Enum for user I/O board commands
*******************************************************************************/
enum user_io_command {
    USER_IO_CMD_TOUCHPAD = 0x01,
    USER_IO_CMD_TOUCH_SLIDER = 0x02,
    USER_IO_CMD_JOYSTICK = 0x03,
    USER_IO_CMD_BTNS = 0x04,
    USER_IO_CMD_LED = 0x05,
    USER_IO_CMD_LED_PWM = 0x06,
    USER_IO_CMD_INFO = 0x07
};

/** ***************************************************************************
 * @brief Struct for storing button bitfields
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

/** ***************************************************************************
 * @brief Initialize the joystick button pin
 *
 * @param _js_btn_pin The GPIO pin struct for the joystick button
 ******************************************************************************/
void joystick_btn_init(struct gpio_pin _js_btn_pin);

/** ***************************************************************************
 * @brief Get the state of the joystick button
 * 
 * @return bool True if pressed, false if not pressed
*******************************************************************************/
bool get_joystick_btn_state(void);

/** ***************************************************************************
 * @brief Get the states of all buttons
 * 
 * @param[out] btn_states Pointer to struct to store button states
 * @return true if successful, false otherwise
*******************************************************************************/
bool get_button_states(struct buttons* btn_states);