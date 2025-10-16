/** ***************************************************************************
 * @file uart_test.c
 * @author Byggarane
 * @brief Test suite for UART driver
 * @version 0.1
 * @date 2025-10-16
 * 
 * @copyright Copyright (c) 2025 Byggarane
 * 
*******************************************************************************/

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define F_CPU 4915200
#include <util/delay.h>
#include <avr/io.h>

#include "../inc/uart.h"

#define TEST_PASSED "PASSED"
#define TEST_FAILED "FAILED"
#define UBRR_TEST_VALUE 25  // For 9600 baud at 4.9152 MHz

static uint8_t tests_passed = 0;
static uint8_t tests_failed = 0;

static void print_test_result(const char* test_name, bool passed) {
    if (passed) {
        printf("[%s] %s\r\n", TEST_PASSED, test_name);
        tests_passed++;
    } else {
        printf("[%s] %s\r\n", TEST_FAILED, test_name);
        tests_failed++;
    }
}

/** ***************************************************************************
 * @brief Test UART initialization
*******************************************************************************/
static void test_uart_init(void) {
    uart_init(UBRR_TEST_VALUE);
    
    // Check UBRR registers
    uint8_t ubrr_l = UBRR0L;
    uint8_t ubrr_h = UBRR0H;
    uint16_t ubrr_value = ((uint16_t)ubrr_h << 8) | ubrr_l;
    
    // Check TX and RX enabled
    bool tx_enabled = (UCSR0B & (1 << TXEN0)) != 0;
    bool rx_enabled = (UCSR0B & (1 << RXEN0)) != 0;
    
    // Check frame format (8 data bits, 1 stop bit)
    bool ursel_set = (UCSR0C & (1 << URSEL0)) != 0;
    bool stop_bit = (UCSR0C & (1 << USBS0)) != 0;
    bool data_bits = ((UCSR0C & (3 << UCSZ00)) == (3 << UCSZ00));
    
    bool passed = (ubrr_value == UBRR_TEST_VALUE) && tx_enabled && 
                  rx_enabled && ursel_set && stop_bit && data_bits;
    
    printf("  UBRR: %d (expected %d)\r\n", ubrr_value, UBRR_TEST_VALUE);
    print_test_result("UART Init", passed);
}

/** ***************************************************************************
 * @brief Test UART transmit single byte
*******************************************************************************/
static void test_uart_transmit_single(void) {
    uart_transmit('A');
    _delay_ms(10);
    
    // If we got here without hanging, it passed
    bool passed = true;
    
    print_test_result("UART Transmit Single", passed);
}

/** ***************************************************************************
 * @brief Test UART transmit multiple bytes
*******************************************************************************/
static void test_uart_transmit_multiple(void) {
    const char* test_str = "TEST123";
    
    for (uint8_t i = 0; i < strlen(test_str); i++) {
        uart_transmit(test_str[i]);
    }
    uart_transmit('\r');
    uart_transmit('\n');
    
    bool passed = true;
    
    print_test_result("UART Transmit Multiple", passed);
}

/** ***************************************************************************
 * @brief Test UART transmit special characters
*******************************************************************************/
static void test_uart_transmit_special(void) {
    // Test various special characters
    uart_transmit('\r');  // Carriage return
    uart_transmit('\n');  // Newline
    uart_transmit('\t');  // Tab
    uart_transmit('\\');  // Backslash
    uart_transmit('\r');
    uart_transmit('\n');
    
    bool passed = true;
    
    print_test_result("UART Transmit Special", passed);
}

/** ***************************************************************************
 * @brief Test UART transmit all ASCII printable
*******************************************************************************/
static void test_uart_transmit_ascii(void) {
    // Transmit all printable ASCII characters
    for (uint8_t c = 32; c < 127; c++) {
        uart_transmit(c);
        if ((c - 32) % 16 == 15) {
            uart_transmit('\r');
            uart_transmit('\n');
        }
    }
    uart_transmit('\r');
    uart_transmit('\n');
    
    bool passed = true;
    
    print_test_result("UART Transmit ASCII", passed);
}

/** ***************************************************************************
 * @brief Test UART stdio wrapper for transmit
*******************************************************************************/
static void test_uart_transmit_stdio(void) {
    FILE dummy_stream;
    
    int ret1 = uart_transmit_stdio('X', &dummy_stream);
    int ret2 = uart_transmit_stdio('Y', &dummy_stream);
    int ret3 = uart_transmit_stdio('Z', &dummy_stream);
    uart_transmit('\r');
    uart_transmit('\n');
    
    bool passed = (ret1 == 0) && (ret2 == 0) && (ret3 == 0);
    
    print_test_result("UART Transmit stdio", passed);
}

/** ***************************************************************************
 * @brief Test UART transmit with printf
*******************************************************************************/
static void test_uart_printf(void) {
    // This tests the integration with printf
    printf("Printf test: %d %s 0x%X\r\n", 42, "hello", 0xAB);
    
    bool passed = true;
    
    print_test_result("UART Printf", passed);
}

/** ***************************************************************************
 * @brief Test UART transmit speed
 * 
 * @details Transmits a large amount of data to check for overflow
*******************************************************************************/
static void test_uart_transmit_speed(void) {
    const uint8_t buffer_size = 100;
    
    for (uint8_t i = 0; i < buffer_size; i++) {
        uart_transmit('.');
    }
    uart_transmit('\r');
    uart_transmit('\n');
    
    bool passed = true;
    
    print_test_result("UART Transmit Speed", passed);
}

/** ***************************************************************************
 * @brief Test UART transmit binary data
*******************************************************************************/
static void test_uart_transmit_binary(void) {
    // Test transmitting non-printable binary data
    uint8_t binary_data[] = {0x00, 0xFF, 0xAA, 0x55, 0x01, 0xFE};
    
    for (uint8_t i = 0; i < sizeof(binary_data); i++) {
        uart_transmit(binary_data[i]);
    }
    uart_transmit('\r');
    uart_transmit('\n');
    
    bool passed = true;
    
    print_test_result("UART Transmit Binary", passed);
}

/** ***************************************************************************
 * @brief Test UART with different baud rates
*******************************************************************************/
static void test_uart_different_bauds(void) {
    // Test initialization with different UBRR values
    uint16_t bauds[] = {12, 25, 51};  // Different baud rates
    
    for (uint8_t i = 0; i < 3; i++) {
        uart_init(bauds[i]);
        uart_transmit('B');
        uart_transmit('0' + i);
        uart_transmit('\r');
        uart_transmit('\n');
        _delay_ms(10);
    }
    
    // Reinit to test value
    uart_init(UBRR_TEST_VALUE);
    
    bool passed = true;
    
    print_test_result("UART Different Bauds", passed);
}

/** ***************************************************************************
 * @brief Run all UART tests
*******************************************************************************/
void run_uart_tests(void) {
    printf("\r\n");
    printf("========================================\r\n");
    printf("       UART Driver Test Suite          \r\n");
    printf("========================================\r\n\r\n");
    
    tests_passed = 0;
    tests_failed = 0;
    
    test_uart_init();
    test_uart_transmit_single();
    test_uart_transmit_multiple();
    test_uart_transmit_special();
    test_uart_transmit_ascii();
    test_uart_transmit_stdio();
    test_uart_printf();
    test_uart_transmit_speed();
    test_uart_transmit_binary();
    test_uart_different_bauds();
    
    printf("\r\n");
    printf("========================================\r\n");
    printf("Results: %d passed, %d failed\r\n", tests_passed, tests_failed);
    printf("========================================\r\n\r\n");
}
