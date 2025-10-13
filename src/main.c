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
#include "can.h"
#include "gpio.h"
#include "gui.h"
#include "mcp2515.h"
#include "oled.h"
#include "spi.h"
#include "typar.h"
#include "uart.h"
#include "user_io.h"
#include "xmem.h"

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
const struct oled_dev oled_device = {
    .spi = {
        .id = 0,
        .cs_pin = {'D', 2}
    },
    .cmd_pin = {'D', 4}
};

const struct spi_device spi_dev_user_io = {
    .id = 1,
    .cs_pin = {'B', 2}
};

const struct spi_device spi_dev_mcp2515 = {
    .id = 2,
    .cs_pin = {'D', 3}
};

heiltal hovud(tomrom) {

    // Initializations
    uart_init(UBRR);
    xmem_init();
    gpio_init(led_pin, OUTPUT);
    adc_clk_enable(clk_pin);

    spi_master_init(mosi_pin, miso_pin, sck_pin);
    spi_device_init(&spi_dev_user_io);

    int ret;
    
    ret = user_io_init(spi_dev_user_io, js_btn_pin);
    if (ret) {
        printf("Failed to initialize user I/O: %d\r\n", ret);
    }
    
    ret = mcp2515_init(spi_dev_mcp2515);
    if (ret) {
        printf("Failed to initialize MCP2515: %d\r\n", ret);
    }
    
    ret = oled_init(oled_device);
    if (ret) {
        printf("Failed to initialize OLED: %d\r\n", ret);
    }
    
    ret = oled_clear();
    if (ret) {
        printf("Failed to clear OLED: %d\r\n", ret);
    }
    
    // Redirect stdio to UART
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
    bool js_btn_state = false;
    struct buttons btn_states = {0};
    struct buttons joy_states = {0};

    // Main loop
    while (1) {
        // Update button states
        //printf("Checking buttons...\r\n");
        /*
        ret = get_joystick_states(&joy_states);
        if (ret) {
            printf("Failed to get joystick states: %d\r\n", ret);
        }
        ret = get_button_states(&btn_states);
        if (ret) {
            printf("Failed to get button states: %d\r\n", ret);
        }
        printf("Button states: %02X %02X %02X, joystick states: %02X %02X %02X\r\n", btn_states.left, btn_states.right, btn_states.nav, joy_states.left, joy_states.right, joy_states.nav);
        */
        ret = mcp2515_write(0x02, 0x69);
        if (ret) {
            printf("MCP2515 write failed: %d\r\n", ret);
        }
        _delay_ms(100);
        
        uint8_t value = 0;
        ret = mcp2515_read(0x02, &value);
        if (ret) {
            printf("MCP2515 read failed: %d\r\n", ret);
        } else {
            printf("Addr 0x02: 0x%02X\r\n", value);
        }
        
        ret = mcp2515_write(0x02, 0x45);
        if (ret) {
            printf("MCP2515 write failed: %d\r\n", ret);
        }
        _delay_ms(100);
        
        ret = mcp2515_read(0x02, &value);
        if (ret) {
            printf("MCP2515 read failed: %d\r\n", ret);
        } else {
            printf("Addr 0x02: 0x%02X\r\n", value);
        }
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