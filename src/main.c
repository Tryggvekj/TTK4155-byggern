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
#include "debug.h"
#include "uart.h"
#include "gpio.h"
#include "xmem.h"
#include "typar.h"
#include "user_io.h"
#include "spi.h"
#include "oled.h"
#include "gui.h"

#define BAUD_RATE 9600
#define UBRR (F_CPU/16/BAUD_RATE - 1)
#define BLINK_DELAY_MS 1000


heiltal hovud(tomrom) {

    // Initializations
    uart_init(UBRR);
    xmem_init();
    gpio_init('B', 0, OUTPUT);
    adc_clk_enable();

    spi_master_init();
    oled_init();
    oled_clear();
    
    fdevopen(uart_transmit_stdio, uart_receive_stdio);

    // Tests
    teikn test_str[] = "Byggarane";
    printf("Hello world, %s!\r\n", test_str);
    //SRAM_test();
    //oled_draw_string(0, 0, "Byggarane", 'l');
    
    // Set up GUI
    draw_menu(&main_menu);
    x_y_coords joystick_pos;

    // Main loop
    while (1) {
        
        //_delay_ms(BLINK_DELAY_MS);
        //gpio_toggle('B', 0);
        update_menu(&main_menu);
    }

    return 0;
}