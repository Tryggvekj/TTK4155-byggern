/** ***************************************************************************
 * @file gpio.h
 * @author Magnus Carlsen Haaland
 * @brief GPIO driver
 * @version 0.1
 * @date 2025-10-23
 * 
 * @copyright Copyright (c) 2025
 * 
*******************************************************************************/

#pragma once

#include <stdbool.h>
#include <stdint.h>

struct sam_gpio_pin {
    uint8_t port;   /**< Port identifier ('A', 'B', 'C', 'D') */
    uint8_t pin;    /**< Pin number (0-7) */
};

int sam_gpio_init(struct sam_gpio_pin);

int sam_gpio_set(struct sam_gpio_pin, bool);

bool sam_gpio_get(struct sam_gpio_pin);

int sam_gpio_toggle(struct sam_gpio_pin);