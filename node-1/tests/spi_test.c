/** ***************************************************************************
 * @file spi_test.c
 * @author Byggarane
 * @brief Test suite for SPI driver
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
 * @brief Test SPI master initialization
*******************************************************************************/
static void test_spi_master_init(void) {
    struct gpio_pin mosi = {'B', 5};
    struct gpio_pin miso = {'B', 6};
    struct gpio_pin sck = {'B', 7};
    
    spi_master_init(mosi, miso, sck);
    
    // Check SPI control register settings
    bool spe_enabled = (SPCR & (1 << SPE)) != 0;
    bool master_mode = (SPCR & (1 << MSTR)) != 0;
    bool double_speed = (SPSR & (1 << SPI2X)) != 0;
    
    bool passed = spe_enabled && master_mode && double_speed;
    
    print_test_result("SPI Master Init", passed);
}

/** ***************************************************************************
 * @brief Test SPI device initialization
*******************************************************************************/
static void test_spi_device_init(void) {
    struct spi_device test_dev = {
        .id = 0,
        .cs_pin = {'D', 2}
    };
    
    int ret = spi_device_init(&test_dev);
    
    // Check CS pin is set as output and HIGH (deselected)
    bool cs_output = (DDRD & (1 << 2)) != 0;
    bool cs_high = (PORTD & (1 << 2)) != 0;
    bool no_error = (ret == 0);
    
    bool passed = cs_output && cs_high && no_error;
    
    print_test_result("SPI Device Init", passed);
}

/** ***************************************************************************
 * @brief Test SPI device initialization with NULL pointer
*******************************************************************************/
static void test_spi_device_init_null(void) {
    int ret = spi_device_init(NULL);
    
    // Should return error code for NULL pointer
    bool passed = (ret < 0);
    
    print_test_result("SPI Device Init NULL", passed);
}

/** ***************************************************************************
 * @brief Test SPI transmit single byte
*******************************************************************************/
static void test_spi_transmit_single(void) {
    struct spi_device test_dev = {
        .id = 0,
        .cs_pin = {'D', 2}
    };
    
    spi_device_init(&test_dev);
    
    uint8_t test_data = 0xAA;
    int ret = spi_master_transmit_single(&test_dev, test_data);
    
    bool passed = (ret == 0);
    
    print_test_result("SPI Transmit Single", passed);
}

/** ***************************************************************************
 * @brief Test SPI transmit multiple bytes
*******************************************************************************/
static void test_spi_transmit_multiple(void) {
    struct spi_device test_dev = {
        .id = 1,
        .cs_pin = {'D', 3}
    };
    
    spi_device_init(&test_dev);
    
    uint8_t test_data[] = {0x01, 0x02, 0x03, 0x04, 0x05};
    int ret = spi_master_transmit(&test_dev, test_data, sizeof(test_data));
    
    bool passed = (ret == 0);
    
    print_test_result("SPI Transmit Multiple", passed);
}

/** ***************************************************************************
 * @brief Test SPI receive
*******************************************************************************/
static void test_spi_receive(void) {
    struct spi_device test_dev = {
        .id = 2,
        .cs_pin = {'B', 2}
    };
    
    spi_device_init(&test_dev);
    
    uint8_t rx_buffer[5] = {0};
    int ret = spi_receive(&test_dev, rx_buffer, sizeof(rx_buffer));
    
    bool passed = (ret == 0);
    
    printf("  Received: ");
    for (uint8_t i = 0; i < sizeof(rx_buffer); i++) {
        printf("0x%02X ", rx_buffer[i]);
    }
    printf("\r\n");
    
    print_test_result("SPI Receive", passed);
}

/** ***************************************************************************
 * @brief Test SPI query operation
*******************************************************************************/
static void test_spi_query(void) {
    struct spi_device test_dev = {
        .id = 3,
        .cs_pin = {'D', 3}
    };
    
    spi_device_init(&test_dev);
    
    uint8_t tx_data[] = {0xAA, 0x55};
    uint8_t rx_data[4] = {0};
    
    int ret = spi_query(&test_dev, tx_data, sizeof(tx_data), rx_data, sizeof(rx_data));
    
    bool passed = (ret == 0);
    
    printf("  TX: 0x%02X 0x%02X, RX: 0x%02X 0x%02X 0x%02X 0x%02X\r\n",
           tx_data[0], tx_data[1], rx_data[0], rx_data[1], rx_data[2], rx_data[3]);
    
    print_test_result("SPI Query", passed);
}

/** ***************************************************************************
 * @brief Test SPI with NULL device pointer
*******************************************************************************/
static void test_spi_null_device(void) {
    uint8_t data = 0x00;
    
    int ret = spi_master_transmit_single(NULL, data);
    
    // Should return error
    bool passed = (ret < 0);
    
    print_test_result("SPI NULL Device", passed);
}

/** ***************************************************************************
 * @brief Test SPI CS pin control
 * 
 * @details Verifies CS pin is properly controlled during transmission
*******************************************************************************/
static void test_spi_cs_control(void) {
    struct spi_device test_dev = {
        .id = 4,
        .cs_pin = {'D', 4}
    };
    
    spi_device_init(&test_dev);
    
    // CS should be HIGH initially
    bool cs_high_before = (PORTD & (1 << 4)) != 0;
    
    // During transmission, CS is controlled internally
    uint8_t data = 0x55;
    spi_master_transmit_single(&test_dev, data);
    
    // CS should be HIGH after transmission (deselected)
    bool cs_high_after = (PORTD & (1 << 4)) != 0;
    
    bool passed = cs_high_before && cs_high_after;
    
    print_test_result("SPI CS Control", passed);
}

/** ***************************************************************************
 * @brief Test SPI multiple devices
 * 
 * @details Tests that multiple SPI devices can coexist
*******************************************************************************/
static void test_spi_multiple_devices(void) {
    struct spi_device dev1 = {.id = 10, .cs_pin = {'D', 2}};
    struct spi_device dev2 = {.id = 11, .cs_pin = {'D', 3}};
    struct spi_device dev3 = {.id = 12, .cs_pin = {'D', 4}};
    
    int ret1 = spi_device_init(&dev1);
    int ret2 = spi_device_init(&dev2);
    int ret3 = spi_device_init(&dev3);
    
    // Transmit to each device
    uint8_t data1 = 0x11;
    uint8_t data2 = 0x22;
    uint8_t data3 = 0x33;
    
    int tx1 = spi_master_transmit_single(&dev1, data1);
    int tx2 = spi_master_transmit_single(&dev2, data2);
    int tx3 = spi_master_transmit_single(&dev3, data3);
    
    bool passed = (ret1 == 0) && (ret2 == 0) && (ret3 == 0) &&
                  (tx1 == 0) && (tx2 == 0) && (tx3 == 0);
    
    print_test_result("SPI Multiple Devices", passed);
}

/** ***************************************************************************
 * @brief Run all SPI tests
*******************************************************************************/
void run_spi_tests(void) {
    printf("\r\n");
    printf("========================================\r\n");
    printf("        SPI Driver Test Suite          \r\n");
    printf("========================================\r\n\r\n");
    
    tests_passed = 0;
    tests_failed = 0;
    
    test_spi_master_init();
    test_spi_device_init();
    test_spi_device_init_null();
    test_spi_transmit_single();
    test_spi_transmit_multiple();
    test_spi_receive();
    test_spi_query();
    test_spi_null_device();
    test_spi_cs_control();
    test_spi_multiple_devices();
    
    printf("\r\n");
    printf("========================================\r\n");
    printf("Results: %d passed, %d failed\r\n", tests_passed, tests_failed);
    printf("========================================\r\n\r\n");
}
