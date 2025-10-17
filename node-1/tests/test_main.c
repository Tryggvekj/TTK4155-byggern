/** ***************************************************************************
 * @file test_main.c
 * @author Byggarane
 * @brief Master test runner for all driver test suites
 * @version 0.1
 * @date 2025-10-16
 * 
 * @copyright Copyright (c) 2025 Byggarane
 * 
 * @details This file provides a unified test runner that can execute all
 *          individual test suites or run them selectively.
 * 
*******************************************************************************/

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define F_CPU 4915200
#include <util/delay.h>

#include "../inc/uart.h"
#include "../inc/gpio.h"
#include "../inc/spi.h"
#include "../inc/adc.h"
#include "../inc/xmem.h"
#include "../inc/oled.h"
#include "../inc/mcp2515.h"
#include "../inc/user_io.h"

// Test suite function declarations
extern void run_adc_tests(void);
extern void run_can_tests(void);
extern void run_gpio_tests(void);
extern void run_gui_tests(void);
extern void run_mcp2515_tests(void);
extern void run_oled_tests(void);
extern void run_spi_tests(void);
extern void run_uart_tests(void);
extern void run_user_io_tests(void);
extern void run_xmem_tests(void);

#define BAUD_RATE 9600
#define UBRR (F_CPU/16/BAUD_RATE - 1)

/** ***************************************************************************
 * @brief Print test menu
*******************************************************************************/
static void print_test_menu(void) {
    printf("\r\n");
    printf("╔════════════════════════════════════════╗\r\n");
    printf("║   TTK4155 Byggern Test Suite v0.1     ║\r\n");
    printf("╚════════════════════════════════════════╝\r\n");
    printf("\r\n");
    printf("Select test suite to run:\r\n");
    printf("  1. ADC Driver Tests\r\n");
    printf("  2. CAN Driver Tests\r\n");
    printf("  3. GPIO Driver Tests\r\n");
    printf("  4. GUI Module Tests\r\n");
    printf("  5. MCP2515 Driver Tests\r\n");
    printf("  6. OLED Driver Tests\r\n");
    printf("  7. SPI Driver Tests\r\n");
    printf("  8. UART Driver Tests\r\n");
    printf("  9. User I/O Driver Tests\r\n");
    printf("  A. XMEM Driver Tests\r\n");
    printf("  0. Run ALL Tests\r\n");
    printf("  Q. Quit\r\n");
    printf("\r\n");
    printf("Enter selection: ");
}

/** ***************************************************************************
 * @brief Initialize hardware for testing
*******************************************************************************/
static void test_hardware_init(void) {
    // Initialize UART
    uart_init(UBRR);
    fdevopen(uart_transmit_stdio, uart_receive_stdio);
    
    printf("\r\n");
    printf("Initializing test hardware...\r\n");
    
    // Initialize external memory
    xmem_init();
    printf("  [OK] External Memory\r\n");
    
    // Initialize GPIO for LED indicator
    struct gpio_pin led_pin = {'B', 0};
    gpio_init(led_pin, OUTPUT);
    printf("  [OK] GPIO\r\n");
    
    // Initialize ADC clock
    struct gpio_pin clk_pin = {'D', 5};
    adc_clk_enable(clk_pin);
    printf("  [OK] ADC Clock\r\n");
    
    // Initialize SPI
    struct gpio_pin mosi_pin = {'B', 5};
    struct gpio_pin miso_pin = {'B', 6};
    struct gpio_pin sck_pin = {'B', 7};
    spi_master_init(mosi_pin, miso_pin, sck_pin);
    printf("  [OK] SPI Master\r\n");
    
    // Initialize OLED
    struct oled_dev oled_device = {
        .spi = {.id = 0, .cs_pin = {'D', 2}},
        .cmd_pin = {'D', 4}
    };
    int ret = oled_init(oled_device);
    if (ret == 0) {
        printf("  [OK] OLED Display\r\n");
        oled_clear();
    } else {
        printf("  [WARN] OLED Init Failed\r\n");
    }
    
    // Initialize User I/O
    struct spi_device user_io_dev = {
        .id = 1,
        .cs_pin = {'B', 2}
    };
    struct gpio_pin js_btn_pin = {'B', 1};
    ret = user_io_init(user_io_dev, js_btn_pin);
    if (ret == 0) {
        printf("  [OK] User I/O\r\n");
    } else {
        printf("  [WARN] User I/O Init Failed\r\n");
    }
    
    // Initialize MCP2515
    struct spi_device mcp2515_dev = {
        .id = 2,
        .cs_pin = {'D', 3}
    };
    ret = mcp2515_init(mcp2515_dev);
    if (ret == 0) {
        printf("  [OK] MCP2515\r\n");
    } else {
        printf("  [WARN] MCP2515 Init Failed\r\n");
    }
    
    printf("\r\nHardware initialization complete!\r\n");
    _delay_ms(500);
}

/** ***************************************************************************
 * @brief Run all test suites
*******************************************************************************/
static void run_all_tests(void) {
    printf("\r\n");
    printf("════════════════════════════════════════\r\n");
    printf("        RUNNING ALL TEST SUITES        \r\n");
    printf("════════════════════════════════════════\r\n");
    
    run_gpio_tests();
    _delay_ms(500);
    
    run_uart_tests();
    _delay_ms(500);
    
    run_spi_tests();
    _delay_ms(500);
    
    run_adc_tests();
    _delay_ms(500);
    
    run_xmem_tests();
    _delay_ms(500);
    
    run_mcp2515_tests();
    _delay_ms(500);
    
    run_oled_tests();
    _delay_ms(500);
    
    run_user_io_tests();
    _delay_ms(500);
    
    run_gui_tests();
    _delay_ms(500);
    
    run_can_tests();
    _delay_ms(500);
    
    printf("\r\n");
    printf("════════════════════════════════════════\r\n");
    printf("      ALL TEST SUITES COMPLETED        \r\n");
    printf("════════════════════════════════════════\r\n");
}

/** ***************************************************************************
 * @brief Main test application entry point
*******************************************************************************/
int main(void) {
    // Initialize hardware
    test_hardware_init();
    
    // Main test loop
    while (1) {
        print_test_menu();
        
        // Wait for user input
        char selection = uart_receive();
        printf("%c\r\n\r\n", selection);
        
        switch(selection) {
            case '1':
                run_adc_tests();
                break;
            case '2':
                run_can_tests();
                break;
            case '3':
                run_gpio_tests();
                break;
            case '4':
                run_gui_tests();
                break;
            case '5':
                run_mcp2515_tests();
                break;
            case '6':
                run_oled_tests();
                break;
            case '7':
                run_spi_tests();
                break;
            case '8':
                run_uart_tests();
                break;
            case '9':
                run_user_io_tests();
                break;
            case 'a':
            case 'A':
                run_xmem_tests();
                break;
            case '0':
                run_all_tests();
                break;
            case 'q':
            case 'Q':
                printf("Exiting test suite. Reset to run again.\r\n");
                while(1); // Halt
                break;
            default:
                printf("Invalid selection. Please try again.\r\n");
                break;
        }
        
        printf("\r\nPress any key to continue...\r\n");
        uart_receive();
    }
    
    return 0;
}
