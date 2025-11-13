/** ***************************************************************************
 * @file main.c
 * @author Magnus Carlsen Haaland, Tryggve Klevstul-Jensen, Walter Brynildsen
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
#define UBRR (F_CPU / 16 / BAUD_RATE - 1)
#define BLINK_DELAY_MS 1000

// NEVER USE PB4 FOR ANYTHING, IT HAS TO BE HIGH FOR SPI TO WORK
// Application-specific pin definitions
struct gpio_pin clk_pin = {'D', 5};
struct gpio_pin led_pin = {'B', 0};
struct gpio_pin js_btn_pin = {'B', 1};
struct gpio_pin mosi_pin = {'B', 5};
struct gpio_pin miso_pin = {'B', 6};
struct gpio_pin sck_pin = {'B', 7};

// SPI devices
const struct oled_dev oled_device = {
    .spi = {
        .id = 0,
        .cs_pin = {'D', 2}},
    .cmd_pin = {'D', 4}};

const struct spi_device spi_dev_user_io = {
    .id = 1,
    .cs_pin = {'B', 2}};

const struct spi_device spi_dev_mcp2515 = {
    .id = 2,
    .cs_pin = {'D', 3}};

// Bit timing: 250 kbps @ 16 MHz
// Sample point: 75%
// TQ: 16
struct can_config can_cfg = {
    .phase2 = 4, // Phase 2 segment (TQ)
    .propag = 3, // Propagation time segment (TQ)
    .phase1 = 8, // Phase 1 segment (TQ)
    .sjw = 1,    // Synchronization jump width (TQ)
    .brp = 1,    // Baud rate prescaler
    .smp = 0     // Sampling mode
};

char test_str[] = "Byggarane";

heiltal hovud(tomrom)
{

    // Initializations
    uart_init(UBRR);
    xmem_init();
    gpio_init(led_pin, OUTPUT);
    adc_clk_enable(clk_pin);

    spi_master_init(mosi_pin, miso_pin, sck_pin);
    spi_device_init(&spi_dev_user_io);

    int ret;

    ret = user_io_init(&spi_dev_user_io, js_btn_pin);
    if (ret)
    {
        // printf("Failed to initialize user I/O: %d\r\n", ret);
    }

    ret = can_init(&spi_dev_mcp2515, CAN_MODE_NORMAL, can_cfg);
    if (ret)
    {
        // printf("Failed to initialize CAN: %d\r\n", ret);
    }

    ret = oled_init(&oled_device);
    if (ret)
    {
        // printf("Failed to initialize OLED: %d\r\n", ret);
    }

    ret = oled_clear();
    if (ret)
    {
        // printf("Failed to clear OLED: %d\r\n", ret);
    }

    // Redirect stdio to UART
    fdevopen(uart_transmit_stdio, uart_receive_stdio);

    // Tests
    // SRAM_test();
    // oled_draw_string(0, 0, "Byggarane", 'l');

    // Set up GUI
    enum gui_state current_state = GUI_STATE_MENU;
    struct menu *current_menu = &main_menu;
    draw_menu(current_menu);
    struct buttons btn_states;

    ret = mcp2515_print_config();
    if (ret)
    {
        printf("Error on reading CAN config: %d\r\n", ret);
    }

    bool state_set = false;
    bool js_btn_state = 0;
    bool js_btn_prev_state = 0;

    struct can_msg msg;

    int return_code = 0;

    // Main loop
    while (1)
    {

        switch (current_state)
        {

            case GUI_STATE_MENU:
                // Clear old received messages
                return_code = can_receive(&msg);
                update_menu(current_menu, &current_state);
                break;

            case GUI_STATE_WAIT_START:
                if (state_set == false)
                {
                    oled_clear();
                    oled_draw_string(0, 0, "Waiting for", 'l');
                    oled_draw_string(1, 0, "game start...", 'l');
                    state_set = true;
                }
                msg.id = CAN_ID_GAME_START;
                msg.dlc = 1;
                msg.bytes[0] = 1;
                can_send(&msg);
                // Wait for game start message
                return_code = can_receive(&msg);
                if (return_code == 0)
                {
                    if (msg.id == CAN_ID_NODE2_RDY)
                    {
                        current_state = GUI_STATE_GAME;
                        state_set = false;
                    }
                }
                get_button_states(&btn_states);
                if (btn_states.L6)
                {
                    // Return to menu
                    state_set = false;
                    current_state = GUI_STATE_MENU;
                    draw_menu(current_menu);
                }
                break;

            case GUI_STATE_GAME:
                
                if (state_set == false)
                {
                    oled_clear();
                    oled_draw_string(0, 0, "Playing...", 'l');
                    state_set = true;
                }
                send_joystick_state_to_can(&msg);
                //_delay_ms(10);
                js_btn_state = get_joystick_btn_state();
                _delay_ms(10);
                if (js_btn_state != js_btn_prev_state)
                {
                    js_btn_prev_state = js_btn_state;
                    send_js_btn_to_can(&msg);
                }
                get_button_states(&btn_states);
                if (btn_states.L6)
                {
                    // Return to menu
                    state_set = false;
                    current_state = GUI_STATE_MENU;
                    draw_menu(current_menu);
                }

                return_code = can_receive(&msg);
                if (return_code == 0)
                {
                    printf("Received CAN message with ID: %X\r\n", msg.id);
                    switch (msg.id)
                    {
                    case CAN_ID_GAME_OVER:
                        state_set = false;
                        current_state = GUI_STATE_GAME_OVER;
                        break;
                    default:
                        break;
                    }
                }

                break;

            case GUI_STATE_GAME_OVER:
                if (state_set == false)
                {
                    oled_clear();
                    oled_draw_string(0, 0, "Game Over!", 'l');
                    state_set = true;
                }
                get_button_states(&btn_states);
                if (btn_states.L6)
                {
                    // Return to menu
                    state_set = false;
                    current_state = GUI_STATE_MENU;
                    draw_menu(current_menu);
                }
                break;

            case GUI_STATE_ERROR:
                oled_clear();
                oled_draw_string(0, 0, "Error!", 'l');
                break;

            default:
                current_state = GUI_STATE_ERROR;
                break;
        }
    }

    return 0;
}