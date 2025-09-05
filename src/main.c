/** ***************************************************************************
 * @file main.c
 * @author Magnuszszev Carlsevitz Haalski, Byggve, Walter Byggildsen
 * @brief Main application file
 * @version 0.1
 * @date 2025-08-05
 * 
 * @copyright Copyright (c) 2025 Byggarane
 * 
 ******************************************************************************/

#include <avr/io.h>

#define F_CPU 4915200 // Hz
#include <util/delay.h>

#define BAUD_RATE 9600
#define UBRR (F_CPU/16/BAUD_RATE - 1)


int main(void) {

    uart_init(UBRR);

    while (1) {
        
        uart_transmit('B');
        uart_transmit('y');
        uart_transmit('g');
        uart_transmit('g');
        uart_transmit('a');
        uart_transmit('r');
        uart_transmit('a');
        uart_transmit('n');
        uart_transmit('e');
        uart_transmit('\n');
    }
}