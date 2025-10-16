/** ***************************************************************************
 * @file mcp2515_test.c
 * @author Byggarane
 * @brief Test suite for MCP2515 CAN controller driver
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

#include "../inc/mcp2515.h"
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
 * @brief Test MCP2515 initialization
*******************************************************************************/
static void test_mcp2515_init(void) {
    struct spi_device mcp_dev = {
        .id = 2,
        .cs_pin = {'D', 3}
    };
    
    int ret = mcp2515_init(mcp_dev);
    
    bool passed = (ret == 0);
    
    print_test_result("MCP2515 Init", passed);
}

/** ***************************************************************************
 * @brief Test MCP2515 reset
*******************************************************************************/
static void test_mcp2515_reset(void) {
    int ret = mcp2515_reset();
    
    bool passed = (ret == 0);
    
    _delay_ms(10); // Wait for reset to complete
    
    print_test_result("MCP2515 Reset", passed);
}

/** ***************************************************************************
 * @brief Test MCP2515 read operation
*******************************************************************************/
static void test_mcp2515_read(void) {
    uint8_t data = 0;
    
    // Read CANSTAT register
    int ret = mcp2515_read(MCP2515_CANSTAT, &data);
    
    bool passed = (ret == 0);
    
    printf("  CANSTAT: 0x%02X\r\n", data);
    print_test_result("MCP2515 Read", passed);
}

/** ***************************************************************************
 * @brief Test MCP2515 read with NULL pointer
*******************************************************************************/
static void test_mcp2515_read_null(void) {
    int ret = mcp2515_read(MCP2515_CANSTAT, NULL);
    
    // Should return error for NULL pointer
    bool passed = (ret < 0);
    
    print_test_result("MCP2515 Read NULL", passed);
}

/** ***************************************************************************
 * @brief Test MCP2515 write operation
*******************************************************************************/
static void test_mcp2515_write(void) {
    // Write to CNF1 register (configuration)
    uint8_t config_value = 0x00;
    int ret = mcp2515_write(MCP2515_CNF1, config_value);
    
    bool passed = (ret == 0);
    
    print_test_result("MCP2515 Write", passed);
}

/** ***************************************************************************
 * @brief Test MCP2515 write and read back
*******************************************************************************/
static void test_mcp2515_write_read_back(void) {
    uint8_t write_val = 0x55;
    uint8_t read_val = 0;
    
    // Write to a test register (CNF3)
    int ret_write = mcp2515_write(MCP2515_CNF3, write_val);
    
    // Small delay
    _delay_us(100);
    
    // Read back
    int ret_read = mcp2515_read(MCP2515_CNF3, &read_val);
    
    bool passed = (ret_write == 0) && (ret_read == 0) && (read_val == write_val);
    
    printf("  Wrote: 0x%02X, Read: 0x%02X\r\n", write_val, read_val);
    print_test_result("MCP2515 Write/Read Back", passed);
}

/** ***************************************************************************
 * @brief Test MCP2515 bit modify operation
*******************************************************************************/
static void test_mcp2515_bit_modify(void) {
    // Read initial value
    uint8_t initial_val = 0;
    mcp2515_read(MCP2515_CANCTRL, &initial_val);
    
    // Modify specific bits (set bit 0)
    uint8_t mask = 0x01;
    uint8_t data = 0x01;
    int ret = mcp2515_bit_modify(MCP2515_CANCTRL, mask, data);
    
    // Read modified value
    uint8_t modified_val = 0;
    mcp2515_read(MCP2515_CANCTRL, &modified_val);
    
    bool passed = (ret == 0) && ((modified_val & mask) == data);
    
    printf("  Initial: 0x%02X, Modified: 0x%02X\r\n", initial_val, modified_val);
    print_test_result("MCP2515 Bit Modify", passed);
}

/** ***************************************************************************
 * @brief Test MCP2515 read status
*******************************************************************************/
static void test_mcp2515_read_status(void) {
    uint8_t status[2] = {0};
    
    int ret = mcp2515_read_status(status);
    
    bool passed = (ret == 0);
    
    printf("  Status: 0x%02X 0x%02X\r\n", status[0], status[1]);
    print_test_result("MCP2515 Read Status", passed);
}

/** ***************************************************************************
 * @brief Test MCP2515 read status with NULL pointer
*******************************************************************************/
static void test_mcp2515_read_status_null(void) {
    int ret = mcp2515_read_status(NULL);
    
    // Should return error for NULL pointer
    bool passed = (ret < 0);
    
    print_test_result("MCP2515 Read Status NULL", passed);
}

/** ***************************************************************************
 * @brief Test MCP2515 request to send
*******************************************************************************/
static void test_mcp2515_rts(void) {
    // Request to send from buffer 0
    int ret = mcp2515_request_to_send(true, false, false);
    
    bool passed = (ret == 0);
    
    print_test_result("MCP2515 RTS Buffer 0", passed);
}

/** ***************************************************************************
 * @brief Test MCP2515 request to send multiple buffers
*******************************************************************************/
static void test_mcp2515_rts_multiple(void) {
    // Request to send from all buffers
    int ret = mcp2515_request_to_send(true, true, true);
    
    bool passed = (ret == 0);
    
    print_test_result("MCP2515 RTS All Buffers", passed);
}

/** ***************************************************************************
 * @brief Test MCP2515 register access patterns
*******************************************************************************/
static void test_mcp2515_register_access(void) {
    bool passed = true;
    
    // Test accessing multiple registers
    uint8_t registers[] = {
        MCP2515_CANCTRL,
        MCP2515_CANSTAT,
        MCP2515_CNF1,
        MCP2515_CNF2,
        MCP2515_CNF3
    };
    
    for (uint8_t i = 0; i < sizeof(registers); i++) {
        uint8_t value = 0;
        int ret = mcp2515_read(registers[i], &value);
        if (ret != 0) {
            passed = false;
            break;
        }
        printf("  Reg 0x%02X: 0x%02X\r\n", registers[i], value);
    }
    
    print_test_result("MCP2515 Register Access", passed);
}

/** ***************************************************************************
 * @brief Test MCP2515 configuration sequence
*******************************************************************************/
static void test_mcp2515_configuration(void) {
    bool passed = true;
    
    // Reset device
    if (mcp2515_reset() != 0) passed = false;
    _delay_ms(10);
    
    // Write configuration registers
    if (mcp2515_write(MCP2515_CNF1, 0x00) != 0) passed = false;
    if (mcp2515_write(MCP2515_CNF2, 0x90) != 0) passed = false;
    if (mcp2515_write(MCP2515_CNF3, 0x02) != 0) passed = false;
    
    // Verify configuration
    uint8_t cnf1 = 0, cnf2 = 0, cnf3 = 0;
    if (mcp2515_read(MCP2515_CNF1, &cnf1) != 0) passed = false;
    if (mcp2515_read(MCP2515_CNF2, &cnf2) != 0) passed = false;
    if (mcp2515_read(MCP2515_CNF3, &cnf3) != 0) passed = false;
    
    printf("  CNF1: 0x%02X, CNF2: 0x%02X, CNF3: 0x%02X\r\n", cnf1, cnf2, cnf3);
    
    print_test_result("MCP2515 Configuration", passed);
}

/** ***************************************************************************
 * @brief Run all MCP2515 tests
*******************************************************************************/
void run_mcp2515_tests(void) {
    printf("\r\n");
    printf("========================================\r\n");
    printf("     MCP2515 Driver Test Suite         \r\n");
    printf("========================================\r\n\r\n");
    
    tests_passed = 0;
    tests_failed = 0;
    
    test_mcp2515_init();
    test_mcp2515_reset();
    test_mcp2515_read();
    test_mcp2515_read_null();
    test_mcp2515_write();
    test_mcp2515_write_read_back();
    test_mcp2515_bit_modify();
    test_mcp2515_read_status();
    test_mcp2515_read_status_null();
    test_mcp2515_rts();
    test_mcp2515_rts_multiple();
    test_mcp2515_register_access();
    test_mcp2515_configuration();
    
    printf("\r\n");
    printf("========================================\r\n");
    printf("Results: %d passed, %d failed\r\n", tests_passed, tests_failed);
    printf("========================================\r\n\r\n");
}
