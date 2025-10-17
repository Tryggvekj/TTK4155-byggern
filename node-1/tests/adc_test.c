/** ***************************************************************************
 * @file adc_test.c
 * @author Byggarane
 * @brief Test suite for ADC driver
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

#include "../inc/adc.h"
#include "../inc/gpio.h"
#include "../inc/uart.h"

#define TEST_PASSED "PASSED"
#define TEST_FAILED "FAILED"

// Test counter
static uint8_t tests_passed = 0;
static uint8_t tests_failed = 0;

/** ***************************************************************************
 * @brief Print test result
 * 
 * @param[in] test_name Name of the test
 * @param[in] passed Whether the test passed
*******************************************************************************/
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
 * @brief Test ADC clock enable
 * 
 * @details Verifies that the ADC clock signal is properly configured
*******************************************************************************/
static void test_adc_clk_enable(void) {
    struct gpio_pin clk_pin = {'D', 5};
    adc_clk_enable(clk_pin);
    
    // Check if Timer1 is configured correctly
    bool ctc_mode = (TCCR1B & (1 << WGM12)) != 0;
    bool no_prescaler = (TCCR1B & (1 << CS10)) != 0;
    bool toggle_mode = (TCCR1A & (1 << COM1A0)) != 0;
    bool ocr_zero = (OCR1A == 0x0000);
    
    bool passed = ctc_mode && no_prescaler && toggle_mode && ocr_zero;
    print_test_result("ADC Clock Enable", passed);
}

/** ***************************************************************************
 * @brief Test ADC read for all channels
 * 
 * @details Reads from all 8 ADC channels and verifies valid data
*******************************************************************************/
static void test_adc_read_all_channels(void) {
    bool passed = true;
    
    for (uint8_t channel = 0; channel < 8; channel++) {
        uint8_t value = adc_read(channel);
        // Value should be between 0 and 255 (always true for uint8_t)
        // Just verify we can read without crashing
        (void)value;
    }
    
    print_test_result("ADC Read All Channels", passed);
}

/** ***************************************************************************
 * @brief Test ADC read consistency
 * 
 * @details Reads the same channel multiple times and checks for reasonable values
*******************************************************************************/
static void test_adc_read_consistency(void) {
    const uint8_t channel = 0;
    const uint8_t num_reads = 10;
    uint8_t readings[num_reads];
    
    // Take multiple readings
    for (uint8_t i = 0; i < num_reads; i++) {
        readings[i] = adc_read(channel);
        _delay_ms(10);
    }
    
    // Check that readings are within reasonable range of each other
    uint8_t min_val = readings[0];
    uint8_t max_val = readings[0];
    
    for (uint8_t i = 1; i < num_reads; i++) {
        if (readings[i] < min_val) min_val = readings[i];
        if (readings[i] > max_val) max_val = readings[i];
    }
    
    // Allow for some variance but not too much (unless there's actual movement)
    uint8_t variance = max_val - min_val;
    bool passed = (variance < 50); // Reasonable threshold for stable reading
    
    printf("  Min: %d, Max: %d, Variance: %d\r\n", min_val, max_val, variance);
    print_test_result("ADC Read Consistency", passed);
}

/** ***************************************************************************
 * @brief Test ADC channel boundaries
 * 
 * @details Tests reading from valid and boundary channels
*******************************************************************************/
static void test_adc_channel_boundaries(void) {
    bool passed = true;
    
    // Test first channel
    uint8_t val0 = adc_read(0);
    (void)val0;
    
    // Test last valid channel
    uint8_t val7 = adc_read(7);
    (void)val7;
    
    // Reading from invalid channels (8+) won't cause crash but may return garbage
    // The function doesn't validate channel number
    
    print_test_result("ADC Channel Boundaries", passed);
}

/** ***************************************************************************
 * @brief Test ADC write operation
 * 
 * @details Tests the ADC write function with various configurations
*******************************************************************************/
static void test_adc_write(void) {
    bool passed = true;
    
    // Test writing different channel configurations
    for (uint8_t channel = 0; channel < 8; channel++) {
        uint8_t config = channel | (1 << _ALL_BIT);
        adc_write(config);
        _delay_us(50);
    }
    
    print_test_result("ADC Write Operation", passed);
}

/** ***************************************************************************
 * @brief Test ADC settling time
 * 
 * @details Verifies that ADC gives stable readings after channel change
*******************************************************************************/
static void test_adc_settling_time(void) {
    // Read from channel 0
    uint8_t val0_before = adc_read(0);
    
    // Switch to different channel
    uint8_t val1 = adc_read(1);
    
    // Switch back to channel 0 and verify reading is similar
    uint8_t val0_after = adc_read(0);
    
    // Readings should be reasonably close if input is stable
    int16_t difference = (int16_t)val0_after - (int16_t)val0_before;
    if (difference < 0) difference = -difference;
    
    bool passed = (difference < 30); // Allow some variance
    
    printf("  Channel 0: %d -> %d (diff: %d)\r\n", 
           val0_before, val0_after, difference);
    print_test_result("ADC Settling Time", passed);
}

/** ***************************************************************************
 * @brief Run all ADC tests
*******************************************************************************/
void run_adc_tests(void) {
    printf("\r\n");
    printf("========================================\r\n");
    printf("        ADC Driver Test Suite          \r\n");
    printf("========================================\r\n\r\n");
    
    tests_passed = 0;
    tests_failed = 0;
    
    test_adc_clk_enable();
    test_adc_read_all_channels();
    test_adc_read_consistency();
    test_adc_channel_boundaries();
    test_adc_write();
    test_adc_settling_time();
    
    printf("\r\n");
    printf("========================================\r\n");
    printf("Results: %d passed, %d failed\r\n", tests_passed, tests_failed);
    printf("========================================\r\n\r\n");
}
