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

#define F_CPU 4915200 // Hz
#include <util/delay.h>

#include "adc.h"
#include "gpio.h"
#include "gui.h"
#include "oled.h"
#include "spi.h"
#include "typar.h"
#include "uart.h"
#include "xmem.h"

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
    enum gui_state current_state = GUI_STATE_MENU;
    struct menu* current_menu = &main_menu;
    draw_menu(current_menu);
    x_y_coords joystick_pos;

    // Main loop
    while (1) {
        
        switch(current_state) {
            case GUI_STATE_MENU:
                update_menu(current_menu);
                break;
            case GUI_STATE_GAME:
                // Game state handling
                break;
            case GUI_STATE_GAME_OVER:
                // Game over state handling
                break;
            case GUI_STATE_ERROR:
                // Error state handling
                break;
            default:
                current_state = GUI_STATE_ERROR;
                break;
        }
        //_delay_ms(BLINK_DELAY_MS);
        //gpio_toggle('B', 0);
    }

    return 0;
}