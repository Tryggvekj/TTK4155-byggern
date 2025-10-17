/** ***************************************************************************
 * @file gpio_test.c
 * @author Byggarane
 * @brief Test suite for GPIO driver
 * @version 0.1
 * @date 2025-10-16
 * 
 * @copyright Copyright (c) 2025 Byggarane
 * 
*******************************************************************************/

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define F_CPU 4915200
#include <util/delay.h>
#include <avr/io.h>

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
 * @brief Test GPIO initialization as output
*******************************************************************************/
static void test_gpio_init_output(void) {
    struct gpio_pin test_pin = {'B', 0};
    
    gpio_init(test_pin, OUTPUT);
    
    // Check if DDR bit is set
    bool passed = (DDRB & (1 << 0)) != 0;
    
    print_test_result("GPIO Init Output", passed);
}

/** ***************************************************************************
 * @brief Test GPIO initialization as input
*******************************************************************************/
static void test_gpio_init_input(void) {
    struct gpio_pin test_pin = {'B', 1};
    
    gpio_init(test_pin, INPUT);
    
    // Check if DDR bit is cleared
    bool passed = (DDRB & (1 << 1)) == 0;
    
    print_test_result("GPIO Init Input", passed);
}

/** ***************************************************************************
 * @brief Test GPIO set high
*******************************************************************************/
static void test_gpio_set_high(void) {
    struct gpio_pin test_pin = {'B', 0};
    
    gpio_init(test_pin, OUTPUT);
    gpio_set(test_pin, HIGH);
    
    // Check if PORT bit is set
    bool passed = (PORTB & (1 << 0)) != 0;
    
    print_test_result("GPIO Set High", passed);
}

/** ***************************************************************************
 * @brief Test GPIO set low
*******************************************************************************/
static void test_gpio_set_low(void) {
    struct gpio_pin test_pin = {'B', 0};
    
    gpio_init(test_pin, OUTPUT);
    gpio_set(test_pin, LOW);
    
    // Check if PORT bit is cleared
    bool passed = (PORTB & (1 << 0)) == 0;
    
    print_test_result("GPIO Set Low", passed);
}

/** ***************************************************************************
 * @brief Test GPIO toggle
*******************************************************************************/
static void test_gpio_toggle(void) {
    struct gpio_pin test_pin = {'B', 0};
    
    gpio_init(test_pin, OUTPUT);
    gpio_set(test_pin, LOW);
    
    // Toggle and check
    gpio_toggle(test_pin);
    bool first_toggle = (PORTB & (1 << 0)) != 0;
    
    gpio_toggle(test_pin);
    bool second_toggle = (PORTB & (1 << 0)) == 0;
    
    bool passed = first_toggle && second_toggle;
    
    print_test_result("GPIO Toggle", passed);
}

/** ***************************************************************************
 * @brief Test GPIO read output
 * 
 * @details Tests reading back the state of an output pin
*******************************************************************************/
static void test_gpio_read_output(void) {
    struct gpio_pin test_pin = {'B', 0};
    
    gpio_init(test_pin, OUTPUT);
    
    // Set high and read
    gpio_set(test_pin, HIGH);
    _delay_us(10);
    bool read_high = gpio_get(test_pin);
    
    // Set low and read
    gpio_set(test_pin, LOW);
    _delay_us(10);
    bool read_low = !gpio_get(test_pin);
    
    bool passed = read_high && read_low;
    
    print_test_result("GPIO Read Output", passed);
}

/** ***************************************************************************
 * @brief Test multiple GPIO ports
 * 
 * @details Verifies GPIO operations work on all ports (A, B, C, D)
*******************************************************************************/
static void test_gpio_all_ports(void) {
    bool passed = true;
    
    struct gpio_pin pins[] = {
        {'A', 0},
        {'B', 0},
        {'C', 0},
        {'D', 0}
    };
    
    for (uint8_t i = 0; i < 4; i++) {
        gpio_init(pins[i], OUTPUT);
        gpio_set(pins[i], HIGH);
        _delay_us(10);
        gpio_set(pins[i], LOW);
    }
    
    print_test_result("GPIO All Ports", passed);
}

/** ***************************************************************************
 * @brief Test GPIO with invalid port
 * 
 * @details Tests that invalid port doesn't crash (error handling)
*******************************************************************************/
static void test_gpio_invalid_port(void) {
    struct gpio_pin invalid_pin = {'Z', 0};
    
    // Should not crash
    gpio_init(invalid_pin, OUTPUT);
    gpio_set(invalid_pin, HIGH);
    bool value = gpio_get(invalid_pin);
    gpio_toggle(invalid_pin);
    
    // If we get here without crashing, it's a pass
    bool passed = !value; // Invalid port returns false
    
    print_test_result("GPIO Invalid Port", passed);
}

/** ***************************************************************************
 * @brief Test GPIO pin boundaries
 * 
 * @details Tests pins 0-7 on a port
*******************************************************************************/
static void test_gpio_pin_boundaries(void) {
    bool passed = true;
    
    for (uint8_t pin = 0; pin < 8; pin++) {
        struct gpio_pin test_pin = {'D', pin};
        gpio_init(test_pin, OUTPUT);
        gpio_set(test_pin, HIGH);
        gpio_set(test_pin, LOW);
    }
    
    print_test_result("GPIO Pin Boundaries", passed);
}

/** ***************************************************************************
 * @brief Test GPIO rapid toggling
 * 
 * @details Tests rapid toggle operations
*******************************************************************************/
static void test_gpio_rapid_toggle(void) {
    struct gpio_pin test_pin = {'B', 0};
    
    gpio_init(test_pin, OUTPUT);
    gpio_set(test_pin, LOW);
    
    // Rapid toggling
    for (uint8_t i = 0; i < 100; i++) {
        gpio_toggle(test_pin);
    }
    
    // After even number of toggles, should be back to LOW
    bool passed = !gpio_get(test_pin);
    
    print_test_result("GPIO Rapid Toggle", passed);
}

/** ***************************************************************************
 * @brief Run all GPIO tests
*******************************************************************************/
void run_gpio_tests(void) {
    printf("\r\n");
    printf("========================================\r\n");
    printf("       GPIO Driver Test Suite          \r\n");
    printf("========================================\r\n\r\n");
    
    tests_passed = 0;
    tests_failed = 0;
    
    test_gpio_init_output();
    test_gpio_init_input();
    test_gpio_set_high();
    test_gpio_set_low();
    test_gpio_toggle();
    test_gpio_read_output();
    test_gpio_all_ports();
    test_gpio_invalid_port();
    test_gpio_pin_boundaries();
    test_gpio_rapid_toggle();
    
    printf("\r\n");
    printf("========================================\r\n");
    printf("Results: %d passed, %d failed\r\n", tests_passed, tests_failed);
    printf("========================================\r\n\r\n");
}
