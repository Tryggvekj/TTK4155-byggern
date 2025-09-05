/** ***************************************************************************
 * @file gpio.c
 * @author Magnuszszev Carlsevitz Haalski
 * @brief GPIO driver implementation
 * @version 0.1
 * @date 2025-09-06
 * 
 * @copyright Copyright (c) 2025 Byggarane
 * 
 ******************************************************************************/

#include "gpio.h"

/** ***************************************************************************
 * @brief Initialize a GPIO pin
 * 
 * @param port The port of the GPIO (A, B, C, D)
 * @param pin The pin number (0-7)
 * @param is_output True to set as output, false for input
 ******************************************************************************/
void gpio_init(uint8_t port, uint8_t pin, bool is_output) {

    switch(port) {
        case 'A':
            if (is_output) {
                DDRA |= (1 << pin);
            } else {
                DDRA &= ~(1 << pin);
            }
            break;
        case 'B':
            if (is_output) {
                DDRB |= (1 << pin);
            } else {
                DDRB &= ~(1 << pin);
            }
            break;
        case 'C':
            if (is_output) {
                DDRC |= (1 << pin);
            } else {
                DDRC &= ~(1 << pin);
            }
            break;
        case 'D':
            if (is_output) {
                DDRD |= (1 << pin);
            } else {
                DDRD &= ~(1 << pin);
            }
            break;
        default:
            // Invalid port
            break;
    }
}

/** ***************************************************************************
 * @brief Set the state of a GPIO pin
 * 
 * @param port The port of the GPIO (A, B, C, D)
 * @param pin The pin number (0-7)
 * @param value True to set HIGH, false to set LOW
 ******************************************************************************/
void gpio_set(uint8_t port, uint8_t pin, bool value) {

    switch(port) {
        case 'A':
            if (value) {
                PORTA |= (1 << pin);
            } else {
                PORTA &= ~(1 << pin);
            }
            break;
        case 'B':
            if (value) {
                PORTB |= (1 << pin);
            } else {
                PORTB &= ~(1 << pin);
            }
            break;
        case 'C':
            if (value) {
                PORTC |= (1 << pin);
            } else {
                PORTC &= ~(1 << pin);
            }
            break;
        case 'D':
            if (value) {
                PORTD |= (1 << pin);
            } else {
                PORTD &= ~(1 << pin);
            }
            break;
        default:
            // Invalid port
            break;
    }
}

/** ***************************************************************************
 * @brief Get the state of a GPIO pin
 * 
 * @param port The port of the GPIO (A, B, C, D)
 * @param pin The pin number (0-7)
 * @return true if HIGH, false if LOW
 ******************************************************************************/
bool gpio_get(uint8_t port, uint8_t pin) {

    switch(port) {
        case 'A':
            return (PINA & (1 << pin)) != 0;
        case 'B':
            return (PINB & (1 << pin)) != 0;
        case 'C':
            return (PINC & (1 << pin)) != 0;
        case 'D':
            return (PIND & (1 << pin)) != 0;
        default:
            // Invalid port
            return false;
    }
}

/** ***************************************************************************
 * @brief Toggle the state of a GPIO pin
 * 
 * @param port The port of the GPIO (A, B, C, D)
 * @param pin The pin number (0-7)
 ******************************************************************************/
void gpio_toggle(uint8_t port, uint8_t pin) {

    switch(port) {
        case 'A':
            PORTA ^= (1 << pin);
            break;
        case 'B':
            PORTB ^= (1 << pin);
            break;
        case 'C':
            PORTC ^= (1 << pin);
            break;
        case 'D':
            PORTD ^= (1 << pin);
            break;
        default:
            // Invalid port
            break;
    }   
}