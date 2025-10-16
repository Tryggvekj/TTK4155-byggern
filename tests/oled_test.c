/** ***************************************************************************
 * @file oled_test.c
 * @author Byggarane
 * @brief Test suite for OLED driver
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

#include "../inc/oled.h"
#include "../inc/spi.h"
#include "../inc/gpio.h"
#include "../inc/uart.h"

#define TEST_PASSED "PASSED"
#define TEST_FAILED "FAILED"

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
 * @brief Test OLED initialization
*******************************************************************************/
static void test_oled_init(void) {
    struct oled_dev oled = {
        .spi = {.id = 0, .cs_pin = {'D', 2}},
        .cmd_pin = {'D', 4}
    };
    
    int ret = oled_init(oled);
    
    bool passed = (ret == 0);
    
    print_test_result("OLED Init", passed);
}

/** ***************************************************************************
 * @brief Test OLED clear
*******************************************************************************/
static void test_oled_clear(void) {
    int ret = oled_clear();
    
    bool passed = (ret == 0);
    
    _delay_ms(100);
    print_test_result("OLED Clear", passed);
}

/** ***************************************************************************
 * @brief Test OLED goto address
*******************************************************************************/
static void test_oled_goto_address(void) {
    bool passed = true;
    
    // Test valid addresses
    int ret1 = oled_goto_address(0, 0);
    int ret2 = oled_goto_address(3, 64);
    int ret3 = oled_goto_address(7, 127);
    
    passed = (ret1 == 0) && (ret2 == 0) && (ret3 == 0);
    
    print_test_result("OLED Goto Address", passed);
}

/** ***************************************************************************
 * @brief Test OLED goto address with invalid parameters
*******************************************************************************/
static void test_oled_goto_address_invalid(void) {
    // Test invalid page
    int ret1 = oled_goto_address(8, 0);
    
    // Test invalid column
    int ret2 = oled_goto_address(0, 128);
    
    bool passed = (ret1 < 0) && (ret2 < 0);
    
    print_test_result("OLED Goto Address Invalid", passed);
}

/** ***************************************************************************
 * @brief Test OLED draw character
*******************************************************************************/
static void test_oled_draw_char(void) {
    oled_clear();
    _delay_ms(50);
    
    // Draw a character in normal font
    int ret = oled_draw_char(0, 0, 'A', 'n');
    
    bool passed = (ret == 0);
    
    _delay_ms(500);
    print_test_result("OLED Draw Char", passed);
}

/** ***************************************************************************
 * @brief Test OLED draw character with different fonts
*******************************************************************************/
static void test_oled_draw_char_fonts(void) {
    oled_clear();
    _delay_ms(50);
    
    bool passed = true;
    
    // Small font
    int ret1 = oled_draw_char(0, 0, 'S', 's');
    if (ret1 != 0) passed = false;
    
    // Normal font
    int ret2 = oled_draw_char(2, 0, 'N', 'n');
    if (ret2 != 0) passed = false;
    
    // Large font
    int ret3 = oled_draw_char(4, 0, 'L', 'l');
    if (ret3 != 0) passed = false;
    
    _delay_ms(500);
    print_test_result("OLED Draw Char Fonts", passed);
}

/** ***************************************************************************
 * @brief Test OLED draw string
*******************************************************************************/
static void test_oled_draw_string(void) {
    oled_clear();
    _delay_ms(50);
    
    const uint8_t* test_str = "Hello";
    int ret = oled_draw_string(0, 0, test_str, 's');
    
    bool passed = (ret == 0);
    
    _delay_ms(500);
    print_test_result("OLED Draw String", passed);
}

/** ***************************************************************************
 * @brief Test OLED draw multiple strings
*******************************************************************************/
static void test_oled_draw_multiple_strings(void) {
    oled_clear();
    _delay_ms(50);
    
    bool passed = true;
    
    int ret1 = oled_draw_string(0, 0, "Line 1", 's');
    int ret2 = oled_draw_string(1, 0, "Line 2", 's');
    int ret3 = oled_draw_string(2, 0, "Line 3", 's');
    int ret4 = oled_draw_string(3, 0, "Line 4", 's');
    
    passed = (ret1 == 0) && (ret2 == 0) && (ret3 == 0) && (ret4 == 0);
    
    _delay_ms(500);
    print_test_result("OLED Draw Multiple Strings", passed);
}

/** ***************************************************************************
 * @brief Test OLED draw all printable ASCII
*******************************************************************************/
static void test_oled_draw_ascii(void) {
    oled_clear();
    _delay_ms(50);
    
    // Draw a subset of printable ASCII characters
    char test_chars[] = "0123456789";
    int ret = oled_draw_string(0, 0, (uint8_t*)test_chars, 's');
    
    bool passed = (ret == 0);
    
    _delay_ms(500);
    print_test_result("OLED Draw ASCII", passed);
}

/** ***************************************************************************
 * @brief Test OLED transmit single byte
*******************************************************************************/
static void test_oled_transmit_single(void) {
    // Transmit command
    int ret1 = oled_transmit_single(OLED_SET_DISPLAY_NORM, true);
    
    // Transmit data
    int ret2 = oled_transmit_single(0xFF, false);
    
    bool passed = (ret1 == 0) && (ret2 == 0);
    
    print_test_result("OLED Transmit Single", passed);
}

/** ***************************************************************************
 * @brief Test OLED transmit multiple bytes
*******************************************************************************/
static void test_oled_transmit_multiple(void) {
    uint8_t commands[] = {
        OLED_SET_SEG_DIR,
        OLED_SET_SCAN_DIR,
        OLED_SET_DISPLAY_NORM
    };
    
    int ret = oled_transmit(commands, sizeof(commands), true);
    
    bool passed = (ret == 0);
    
    print_test_result("OLED Transmit Multiple", passed);
}

/** ***************************************************************************
 * @brief Test OLED display patterns
*******************************************************************************/
static void test_oled_patterns(void) {
    oled_clear();
    _delay_ms(50);
    
    bool passed = true;
    
    // Create a checkerboard pattern
    for (uint8_t page = 0; page < 4; page++) {
        for (uint8_t col = 0; col < 128; col += 2) {
            oled_goto_address(page, col);
            oled_transmit_single(0xAA, false);
        }
    }
    
    _delay_ms(1000);
    print_test_result("OLED Patterns", passed);
}

/** ***************************************************************************
 * @brief Test OLED full screen test
*******************************************************************************/
static void test_oled_full_screen(void) {
    // Fill entire screen
    for (uint8_t page = 0; page < NUM_PAGES; page++) {
        oled_goto_address(page, 0);
        for (uint8_t col = 0; col < NUM_COLUMNS; col++) {
            oled_transmit_single(0xFF, false);
        }
    }
    
    _delay_ms(500);
    
    // Clear entire screen
    oled_clear();
    
    bool passed = true;
    
    print_test_result("OLED Full Screen Test", passed);
}

/** ***************************************************************************
 * @brief Test OLED special characters
*******************************************************************************/
static void test_oled_special_chars(void) {
    oled_clear();
    _delay_ms(50);
    
    // Test special characters
    const uint8_t* special = "!@#$%^&*()";
    int ret = oled_draw_string(0, 0, special, 's');
    
    bool passed = (ret == 0);
    
    _delay_ms(500);
    print_test_result("OLED Special Characters", passed);
}

/** ***************************************************************************
 * @brief Run all OLED tests
*******************************************************************************/
void run_oled_tests(void) {
    printf("\r\n");
    printf("========================================\r\n");
    printf("       OLED Driver Test Suite          \r\n");
    printf("========================================\r\n\r\n");
    
    tests_passed = 0;
    tests_failed = 0;
    
    test_oled_init();
    test_oled_clear();
    test_oled_goto_address();
    test_oled_goto_address_invalid();
    test_oled_draw_char();
    test_oled_draw_char_fonts();
    test_oled_draw_string();
    test_oled_draw_multiple_strings();
    test_oled_draw_ascii();
    test_oled_transmit_single();
    test_oled_transmit_multiple();
    test_oled_patterns();
    test_oled_full_screen();
    test_oled_special_chars();
    
    printf("\r\n");
    printf("========================================\r\n");
    printf("Results: %d passed, %d failed\r\n", tests_passed, tests_failed);
    printf("========================================\r\n\r\n");
}
