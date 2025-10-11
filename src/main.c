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
#include "user_io.h"
#include "xmem.h"
#include "mcp2515.h"

#define BAUD_RATE 9600
#define UBRR (F_CPU/16/BAUD_RATE - 1)
#define BLINK_DELAY_MS 1000


// NEVER USE PB4 FOR ANYTHING, IT HAS TO BE HIGH FOR SPI TO WORK
// Application-specific pin definitions
struct gpio_pin clk_pin = { 'D', 5 };
struct gpio_pin led_pin = { 'B', 0 };
struct gpio_pin js_btn_pin = { 'B', 1 };
struct gpio_pin mosi_pin = { 'B', 5 };
struct gpio_pin miso_pin = { 'B', 6 };
struct gpio_pin sck_pin = { 'B', 7 };


// SPI devices
const struct spi_device spi_dev_oled = {
    .id = 0,
    .cs_pin = {'D', 2}
};

const struct spi_device spi_dev_user_io = {
    .id = 1,
    .cs_pin = {'B', 2}
};

const struct spi_device spi_dev_mcp2515 = {
    .id = 2,
    .cs_pin = {'D', 3}
};

const struct oled_dev oled_device = {
    .spi = spi_dev_oled,
    .cmd_pin = {'D', 4}
};

heiltal hovud(tomrom) {

    // Initializations
    uart_init(UBRR);
    xmem_init();
    gpio_init(led_pin, OUTPUT);
    //joystick_btn_init(js_btn_pin);
    adc_clk_enable(clk_pin);

    spi_master_init(mosi_pin, miso_pin, sck_pin);
    spi_device_init(&spi_dev_user_io);

    oled_init(oled_device);
    mcp2515_init(spi_dev_mcp2515);
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
    bool btn_state = false;
    struct buttons btn_states = {0};
    struct buttons joy_states = {0};

    // Main loop
    while (1) {
        // Update button states
        //printf("Checking buttons...\r\n");
        /*
        get_joystick_states(&joy_states);
        get_button_states(&btn_states);
        printf("Button states: %02X %02X %02X, joystick states: %02X %02X %02X\r\n", btn_states.left, btn_states.right, btn_states.nav, joy_states.left, joy_states.right, joy_states.nav);
        */
        mcp2515_write(0x01, 0x69);
        _delay_ms(10);
        uint8_t value = mcp2515_read(0x01);
        printf("Addr 0x01: 0x%02X\r\n", value);
        //_delay_ms(200);

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



        //_delay_ms(1000);
        //gpio_toggle(led_pin);
    }

    return 0;
}