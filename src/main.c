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

#include <stdio.h>

#include <avr/io.h>

#define F_CPU 4915200 // Hz
#include <util/delay.h>

#include "uart.h"
#include "gpio.h"
#include "typar.h"

#define BAUD_RATE 9600
#define UBRR (F_CPU/16/BAUD_RATE - 1)
#define DELAY_MS 500


int main(void) {

    // Initialize UART
    uart_init(UBRR);

    // Redirect standard input and output to UART
    fdevopen(uart_transmit, uart_receive);

    // Test GPIO init
    gpio_init('C', 0, true); // Set PORTA pin 0 as output

    teikn test_str[] = "Byggarane";
    printf("Hello world, %s!\r\n", test_str);

    while (1) {
        
        // Transmit character 'B'
        uart_transmit(uart_receive());

        // Toggle GPIO pin
        gpio_toggle('C', 0);

        // Wait
        _delay_ms(DELAY_MS);

        true;
    }

    return 0;
}