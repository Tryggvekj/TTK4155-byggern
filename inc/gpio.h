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
 * @brief Initialize a GPIO pin
 * 
 * @param port The port of the GPIO (A, B, C, D)
 * @param pin The pin number (0-7)
 * @param is_output True to set as output, false for input
 ******************************************************************************/
void gpio_init(uint8_t port, uint8_t pin, bool is_output);

/** ***************************************************************************
 * @brief Set the state of a GPIO pin
 * 
 * @param port The port of the GPIO (A, B, C, D)
 * @param pin The pin number (0-7)
 * @param value True to set HIGH, false to set LOW
 ******************************************************************************/
void gpio_set(uint8_t port, uint8_t pin, bool value);

/** ***************************************************************************
 * @brief Get the state of a GPIO pin
 * 
 * @param port The port of the GPIO (A, B, C, D)
 * @param pin The pin number (0-7)
 * @return true if HIGH, false if LOW
 ******************************************************************************/
bool gpio_get(uint8_t port, uint8_t pin);

/** ***************************************************************************
 * @brief Toggle the state of a GPIO pin
 * 
 * @param port The port of the GPIO (A, B, C, D)
 * @param pin The pin number (0-7)
 ******************************************************************************/
void gpio_toggle(uint8_t port, uint8_t pin);