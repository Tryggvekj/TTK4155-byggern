/** ***************************************************************************
 * @file gpio.h
 * @author Magnuszszev Carlsevitz Haalski
 * @brief Header file for GPIO operations
 * @version 0.1
 * @date 2025-09-06
 * 
 * @copyright Copyright (c) 2025 Byggarane
 * 
 ******************************************************************************/

#pragma once

#include <stdbool.h>

#include <avr/io.h>

#define HIGH 1
#define LOW 0

#define INPUT 0
#define OUTPUT 1


/** ***************************************************************************
 * @brief Structure for GPIO pins
 * 
 * @details Defines a GPIO pin by its port and pin number
 ******************************************************************************/
struct gpio_pin {
    uint8_t port;   /**< Port identifier ('A', 'B', 'C', 'D') */
    uint8_t pin;    /**< Pin number (0-7) */
};


/** ***************************************************************************
 * @brief Initialize a GPIO pin
 * 
 * @param[in] gpio GPIO pin structure containing port and pin information
 * @param[in] is_output True to set as output, false for input
 ******************************************************************************/
void gpio_init(struct gpio_pin gpio, bool is_output);

/** ***************************************************************************
 * @brief Set the state of a GPIO pin
 * 
 * @param[in] gpio GPIO pin structure containing port and pin information
 * @param[in] value True to set HIGH, false to set LOW
 ******************************************************************************/
void gpio_set(struct gpio_pin gpio, bool value);

/** ***************************************************************************
 * @brief Get the state of a GPIO pin
 * 
 * @param[in] gpio GPIO pin structure containing port and pin information
 * @return bool True if HIGH, false if LOW
 ******************************************************************************/
bool gpio_get(struct gpio_pin gpio);

/** ***************************************************************************
 * @brief Toggle the state of a GPIO pin
 * 
 * @param[in] gpio GPIO pin structure containing port and pin information
 ******************************************************************************/
void gpio_toggle(struct gpio_pin gpio);