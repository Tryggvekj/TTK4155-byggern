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

#include "adc.h"
#include "uart.h"
#include "gpio.h"
#include "xmem.h"
#include "typar.h"
#include "user_io.h"
#include "spi.h"

#define BAUD_RATE 9600
#define UBRR (F_CPU/16/BAUD_RATE - 1)
#define DELAY_MS 1000


heiltal hovud(tomrom) {

    // Initializations
    uart_init(UBRR);
    xmem_init();
    gpio_init('B', 0, OUTPUT);
    
    fdevopen(uart_transmit, uart_receive);

    // Tests
    teikn test_str[] = "Byggarane";
    printf("Hello world, %s!\r\n", test_str);
    //SRAM_test();


    adc_clk_enable();

    SPI_MasterInit();

    OLED_init();

    // Main loop
    while (1) {

        _delay_ms(100);

        //SPI_MasterTransmit(0x00, 1);
    }

    return 0;
}