/** ***************************************************************************
 * @file main.c
 * @author Magnuszszew Carlsewitz Haalski, Byggve, Walter Byggildsen
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
#include "xmem.h"
#include "typar.h"

#define BAUD_RATE 9600
#define UBRR (F_CPU/16/BAUD_RATE - 1)
#define DELAY_MS 1000


int main(void) {

    // Initializations
    uart_init(UBRR);
    xmem_init();
    gpio_init('B', 0, true); // Set PORTB pin 0 as output
    fdevopen(uart_transmit, uart_receive);

    // Tests
    teikn test_str[] = "Byggarane";
    printf("Hello world, %s!\r\n", test_str);
    SRAM_test();

    // Main loop
    while (1) {

        // Toggle GPIO pin
        gpio_toggle('B', 0);

        // Wait
        _delay_ms(DELAY_MS);
        
    }

    return 0;
}