/** ***************************************************************************
 * @file gpio.c
 * @author Magnus Carlsen Haaland
 * @brief GPIO driver
 * @version 0.1
 * @date 2025-11-06
 *
 * @copyright Copyright (c) 2025 Byggarane
 *
*******************************************************************************/

#include "gpio.h"
#include <errno.h>
#include <sam.h>
#include <stdio.h>

int sam_gpio_init(struct sam_gpio_pin pin) {
    if (pin.pin > 31) {
        printf("Invalid pin number %d\r\n", pin.pin);
        return -EINVAL;
    }

    switch (pin.port){
    case 'A':
        /* code for initializing PORTA */
        PIOA->PIO_WPMR = 0x50494F00;
        PIOA->PIO_OER |= (1 << pin.pin);
        break;
    case 'B':
        /* code for initializing PORTB */
        PIOB->PIO_WPMR = 0x50494F00;
        PIOB->PIO_OER |= (1 << pin.pin);
        break;
    case 'C':
        /* code for initializing PORTC */
        PIOC->PIO_WPMR = 0x50494F00;
        PIOC->PIO_OER |= (1 << pin.pin);
        break;
    case 'D':
        /* code for initializing PORTD */
        PIOD->PIO_WPMR = 0x50494F00;
        PIOD->PIO_OER |= (1 << pin.pin);
        break;
    default:
        printf("Invalid port identifier %d\r\n", pin.port);
        return -EINVAL;
    }

    return 0;
}

int sam_gpio_set(struct sam_gpio_pin pin, bool value) {
    if (pin.pin > 31) {
        printf("Invalid pin number %d\r\n", pin.pin);
        return -EINVAL;
    }

    switch (pin.port){
    case 'A':
        /* code for initializing PORTA */
        if (value) {
            PIOA->PIO_SODR |= (1 << pin.pin); // Set pin
        } else {
            PIOA->PIO_CODR |= (1 << pin.pin); // Clear pin
        }
        break;
    case 'B':
        /* code for initializing PORTB */
        if (value) {
            PIOB->PIO_SODR |= (1 << pin.pin); // Set pin
        } else {
            PIOB->PIO_CODR |= (1 << pin.pin); // Clear pin
        }
        break;
    case 'C':
        /* code for initializing PORTC */
        if (value) {
            PIOC->PIO_SODR |= (1 << pin.pin); // Set pin
        } else {
            PIOC->PIO_CODR |= (1 << pin.pin); // Clear pin
        }
        break;
    case 'D':
        /* code for initializing PORTD */
        if (value) {
            PIOD->PIO_SODR |= (1 << pin.pin); // Set pin
        } else {
            PIOD->PIO_CODR |= (1 << pin.pin); // Clear pin
        }
        break;
    default:
        printf("Invalid port identifier %d\r\n", pin.port);
        return -EINVAL;
    }
    return 0;
}

bool sam_gpio_get(struct sam_gpio_pin pin) {
    if (pin.pin > 31) {
        printf("Invalid pin number %d\r\n", pin.pin);
        return false;
    }

    switch (pin.port){
    case 'A':
        /* code for initializing PORTA */
        if (PIOA->PIO_OSR & (1 << pin.pin)) {
            if (PIOA->PIO_ODSR & (1 << pin.pin)) {
                return true;
            } else {
                return false;
            }
        }
        break;
    case 'B':
        /* code for initializing PORTB */
        if (PIOB->PIO_OSR & (1 << pin.pin)) {
            if (PIOB->PIO_ODSR & (1 << pin.pin)) {
                return true;
            } else {
                return false;
            }
        }
        break;
    case 'C':
        /* code for initializing PORTC */
        if (PIOC->PIO_OSR & (1 << pin.pin)) {
            if (PIOC->PIO_ODSR & (1 << pin.pin)) {
                return true;
            } else {
                return false;
            }
        }
        if (PIOC->PIO_ODSR & (1 << pin.pin)) {
            return true;
        } else {
            return false;
        }
        break;
    case 'D':
        /* code for initializing PORTD */
        if (PIOD->PIO_OSR & (1 << pin.pin)) {
            if (PIOD->PIO_ODSR & (1 << pin.pin)) {
                return true;
            } else {
                return false;
            }
        }
        break;
    default:
        printf("Invalid port identifier %d\r\n", pin.port);
        return false;
    }
    return false;
}

int sam_gpio_toggle(struct sam_gpio_pin pin) {
    if (pin.pin > 31) {
        printf("Invalid pin number %d\r\n", pin.pin);
        return -EINVAL;
    }

    switch (pin.port){
    case 'A':
        /* code for initializing PORTA */
        if (PIOA->PIO_ODSR & (1 << pin.pin)) {
            PIOA->PIO_CODR |= (1 << pin.pin); // Clear pin
        } else {
            PIOA->PIO_SODR |= (1 << pin.pin); // Set pin
        }
        break;
    case 'B':
        /* code for initializing PORTB */
        if (PIOB->PIO_ODSR & (1 << pin.pin)) {
            PIOB->PIO_CODR |= (1 << pin.pin); // Clear pin
        } else {
            PIOB->PIO_SODR |= (1 << pin.pin); // Set pin
        }
        break;
    case 'C':
        /* code for initializing PORTC */
        if (PIOC->PIO_ODSR & (1 << pin.pin)) {
            PIOC->PIO_CODR |= (1 << pin.pin); // Clear pin
        } else {
            PIOC->PIO_SODR |= (1 << pin.pin); // Set pin
        }
        break;
    case 'D':
        /* code for initializing PORTD */
        if (PIOD->PIO_ODSR & (1 << pin.pin)) {
            PIOD->PIO_CODR |= (1 << pin.pin); // Clear pin
        } else {
            PIOD->PIO_SODR |= (1 << pin.pin); // Set pin
        }
        break;
    default:
        printf("Invalid port identifier %d\r\n", pin.port);
        return -EINVAL;
    }
    return 0;
}