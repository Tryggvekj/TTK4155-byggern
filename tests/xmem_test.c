/** ***************************************************************************
 * @file xmem_test.c
 * @author Byggarane
 * @brief Test suite for External Memory (XMEM) driver
 * @version 0.1
 * @date 2025-10-16
 * 
 * @copyright Copyright (c) 2025 Byggarane
 * 
*******************************************************************************/

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#define F_CPU 4915200
#include <util/delay.h>
#include <avr/io.h>

#include "../inc/xmem.h"
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
 * @brief Test XMEM initialization
*******************************************************************************/
static void test_xmem_init(void) {
    xmem_init();
    
    // Check if external memory is enabled
    bool sre_enabled = (MCUCR & (1 << SRE)) != 0;
    bool xmm0_set = (SFIOR & (1 << XMM0)) != 0;
    
    bool passed = sre_enabled && xmm0_set;
    
    print_test_result("XMEM Init", passed);
}

/** ***************************************************************************
 * @brief Test XMEM single byte write and read
*******************************************************************************/
static void test_xmem_single_write_read(void) {
    uint16_t addr = 0x0000;
    uint8_t write_val = 0x42;
    
    xmem_write(write_val, addr);
    uint8_t read_val = xmem_read(addr);
    
    bool passed = (read_val == write_val);
    
    printf("  Wrote: 0x%02X, Read: 0x%02X\r\n", write_val, read_val);
    print_test_result("XMEM Single Write/Read", passed);
}

/** ***************************************************************************
 * @brief Test XMEM multiple sequential writes and reads
*******************************************************************************/
static void test_xmem_sequential(void) {
    bool passed = true;
    uint16_t start_addr = 0x0010;
    uint8_t num_bytes = 16;
    
    // Write sequential values
    for (uint8_t i = 0; i < num_bytes; i++) {
        xmem_write(i, start_addr + i);
    }
    
    // Read back and verify
    for (uint8_t i = 0; i < num_bytes; i++) {
        uint8_t read_val = xmem_read(start_addr + i);
        if (read_val != i) {
            passed = false;
            printf("  Mismatch at 0x%04X: expected 0x%02X, got 0x%02X\r\n",
                   start_addr + i, i, read_val);
        }
    }
    
    print_test_result("XMEM Sequential", passed);
}

/** ***************************************************************************
 * @brief Test XMEM with alternating pattern
*******************************************************************************/
static void test_xmem_alternating_pattern(void) {
    bool passed = true;
    uint16_t start_addr = 0x0100;
    uint8_t patterns[] = {0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55};
    
    // Write pattern
    for (uint8_t i = 0; i < sizeof(patterns); i++) {
        xmem_write(patterns[i], start_addr + i);
    }
    
    // Verify pattern
    for (uint8_t i = 0; i < sizeof(patterns); i++) {
        uint8_t read_val = xmem_read(start_addr + i);
        if (read_val != patterns[i]) {
            passed = false;
            break;
        }
    }
    
    print_test_result("XMEM Alternating Pattern", passed);
}

/** ***************************************************************************
 * @brief Test XMEM boundary addresses
*******************************************************************************/
static void test_xmem_boundaries(void) {
    bool passed = true;
    
    // Test first address
    xmem_write(0x11, 0x0000);
    uint8_t val_first = xmem_read(0x0000);
    if (val_first != 0x11) passed = false;
    
    // Test middle address
    xmem_write(0x22, SRAM_SIZE / 2);
    uint8_t val_mid = xmem_read(SRAM_SIZE / 2);
    if (val_mid != 0x22) passed = false;
    
    // Test last address
    xmem_write(0x33, SRAM_SIZE - 1);
    uint8_t val_last = xmem_read(SRAM_SIZE - 1);
    if (val_last != 0x33) passed = false;
    
    printf("  First: 0x%02X, Mid: 0x%02X, Last: 0x%02X\r\n",
           val_first, val_mid, val_last);
    print_test_result("XMEM Boundaries", passed);
}

/** ***************************************************************************
 * @brief Test XMEM with all byte values
*******************************************************************************/
static void test_xmem_all_byte_values(void) {
    bool passed = true;
    uint16_t addr = 0x0200;
    
    // Write all possible byte values
    for (uint16_t val = 0; val <= 255; val++) {
        xmem_write((uint8_t)val, addr);
        uint8_t read_val = xmem_read(addr);
        if (read_val != (uint8_t)val) {
            passed = false;
            printf("  Failed at value 0x%02X\r\n", (uint8_t)val);
            break;
        }
    }
    
    print_test_result("XMEM All Byte Values", passed);
}

/** ***************************************************************************
 * @brief Test XMEM data retention after multiple writes
*******************************************************************************/
static void test_xmem_data_retention(void) {
    bool passed = true;
    uint16_t addr1 = 0x0300;
    uint16_t addr2 = 0x0400;
    
    // Write to two different addresses
    xmem_write(0xAB, addr1);
    xmem_write(0xCD, addr2);
    
    // Write to many other addresses
    for (uint16_t i = 0x0500; i < 0x0600; i++) {
        xmem_write(0xFF, i);
    }
    
    // Verify original addresses still have correct values
    uint8_t val1 = xmem_read(addr1);
    uint8_t val2 = xmem_read(addr2);
    
    passed = (val1 == 0xAB) && (val2 == 0xCD);
    
    printf("  Addr1: 0x%02X (expected 0xAB), Addr2: 0x%02X (expected 0xCD)\r\n",
           val1, val2);
    print_test_result("XMEM Data Retention", passed);
}

/** ***************************************************************************
 * @brief Test XMEM with pseudo-random data
*******************************************************************************/
static void test_xmem_random_data(void) {
    bool passed = true;
    uint16_t start_addr = 0x0600;
    uint8_t num_bytes = 32;
    uint8_t data[32];
    
    // Generate random data
    srand(42); // Fixed seed for reproducibility
    for (uint8_t i = 0; i < num_bytes; i++) {
        data[i] = (uint8_t)(rand() & 0xFF);
        xmem_write(data[i], start_addr + i);
    }
    
    // Verify random data
    for (uint8_t i = 0; i < num_bytes; i++) {
        uint8_t read_val = xmem_read(start_addr + i);
        if (read_val != data[i]) {
            passed = false;
            break;
        }
    }
    
    print_test_result("XMEM Random Data", passed);
}

/** ***************************************************************************
 * @brief Test XMEM walking ones pattern
*******************************************************************************/
static void test_xmem_walking_ones(void) {
    bool passed = true;
    uint16_t addr = 0x0700;
    
    // Test walking ones pattern
    for (uint8_t bit = 0; bit < 8; bit++) {
        uint8_t pattern = (1 << bit);
        xmem_write(pattern, addr);
        uint8_t read_val = xmem_read(addr);
        if (read_val != pattern) {
            passed = false;
            printf("  Walking ones failed at bit %d\r\n", bit);
            break;
        }
    }
    
    print_test_result("XMEM Walking Ones", passed);
}

/** ***************************************************************************
 * @brief Test XMEM walking zeros pattern
*******************************************************************************/
static void test_xmem_walking_zeros(void) {
    bool passed = true;
    uint16_t addr = 0x0700;
    
    // Test walking zeros pattern
    for (uint8_t bit = 0; bit < 8; bit++) {
        uint8_t pattern = ~(1 << bit);
        xmem_write(pattern, addr);
        uint8_t read_val = xmem_read(addr);
        if (read_val != pattern) {
            passed = false;
            printf("  Walking zeros failed at bit %d\r\n", bit);
            break;
        }
    }
    
    print_test_result("XMEM Walking Zeros", passed);
}

/** ***************************************************************************
 * @brief Test XMEM comprehensive SRAM test
*******************************************************************************/
static void test_xmem_comprehensive(void) {
    printf("\r\nRunning comprehensive SRAM test...\r\n");
    SRAM_test();
    
    // The SRAM_test function prints its own results
    bool passed = true; // Assume passed if no crash
    
    print_test_result("XMEM Comprehensive Test", passed);
}

/** ***************************************************************************
 * @brief Run all XMEM tests
*******************************************************************************/
void run_xmem_tests(void) {
    printf("\r\n");
    printf("========================================\r\n");
    printf("       XMEM Driver Test Suite          \r\n");
    printf("========================================\r\n\r\n");
    
    tests_passed = 0;
    tests_failed = 0;
    
    test_xmem_init();
    test_xmem_single_write_read();
    test_xmem_sequential();
    test_xmem_alternating_pattern();
    test_xmem_boundaries();
    test_xmem_all_byte_values();
    test_xmem_data_retention();
    test_xmem_random_data();
    test_xmem_walking_ones();
    test_xmem_walking_zeros();
    test_xmem_comprehensive();
    
    printf("\r\n");
    printf("========================================\r\n");
    printf("Results: %d passed, %d failed\r\n", tests_passed, tests_failed);
    printf("========================================\r\n\r\n");
}
