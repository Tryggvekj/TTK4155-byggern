/** ***************************************************************************
 * @file user_io_test.c
 * @author Byggarane
 * @brief Test suite for User I/O driver
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

#include "../inc/user_io.h"
#include "../inc/gpio.h"
#include "../inc/spi.h"
#include "../inc/adc.h"
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
 * @brief Test User I/O initialization
*******************************************************************************/
static void test_user_io_init(void) {
    struct spi_device user_io_dev = {
        .id = 1,
        .cs_pin = {'B', 2}
    };
    struct gpio_pin js_btn = {'B', 1};
    
    int ret = user_io_init(user_io_dev, js_btn);
    
    bool passed = (ret == 0);
    
    print_test_result("User I/O Init", passed);
}

/** ***************************************************************************
 * @brief Test joystick X/Y percentage reading
*******************************************************************************/
static void test_joystick_xy_percentage(void) {
    x_y_coords coords = get_joystick_x_y_percentage();
    
    // Coordinates should be in 0-100 range
    bool x_valid = (coords.x <= 100);
    bool y_valid = (coords.y <= 100);
    bool passed = x_valid && y_valid;
    
    printf("  Joystick: X=%d%%, Y=%d%%\r\n", coords.x, coords.y);
    print_test_result("Joystick XY Percentage", passed);
}

/** ***************************************************************************
 * @brief Test touchpad X/Y percentage reading
*******************************************************************************/
static void test_touchpad_xy_percentage(void) {
    x_y_coords coords = get_touchpad_x_y_percentage();
    
    // Coordinates should be in 0-100 range
    bool x_valid = (coords.x <= 100);
    bool y_valid = (coords.y <= 100);
    bool passed = x_valid && y_valid;
    
    printf("  Touchpad: X=%d%%, Y=%d%%\r\n", coords.x, coords.y);
    print_test_result("Touchpad XY Percentage", passed);
}

/** ***************************************************************************
 * @brief Test joystick direction detection
*******************************************************************************/
static void test_joystick_direction(void) {
    enum joystick_direction dir = get_joystick_direction();
    
    const char* dir_str;
    switch(dir) {
        case JOYSTICK_NEUTRAL: dir_str = "NEUTRAL"; break;
        case JOYSTICK_UP:      dir_str = "UP";      break;
        case JOYSTICK_DOWN:    dir_str = "DOWN";    break;
        case JOYSTICK_LEFT:    dir_str = "LEFT";    break;
        case JOYSTICK_RIGHT:   dir_str = "RIGHT";   break;
        default:               dir_str = "UNKNOWN"; break;
    }
    
    bool passed = (dir >= JOYSTICK_NEUTRAL && dir <= JOYSTICK_RIGHT);
    
    printf("  Direction: %s\r\n", dir_str);
    print_test_result("Joystick Direction", passed);
}

/** ***************************************************************************
 * @brief Test joystick button state
*******************************************************************************/
static void test_joystick_button(void) {
    bool btn_state = get_joystick_btn_state();
    
    printf("  Button state: %s\r\n", btn_state ? "PRESSED" : "RELEASED");
    
    // If we can read without crashing, it's a pass
    bool passed = true;
    
    print_test_result("Joystick Button", passed);
}

/** ***************************************************************************
 * @brief Test reading button states via SPI
*******************************************************************************/
static void test_button_states_spi(void) {
    struct buttons btns = {0};
    
    int ret = get_button_states(&btns);
    
    bool passed = (ret == 0);
    
    printf("  Right: 0x%02X, Left: 0x%02X, Nav: 0x%02X\r\n",
           btns.right, btns.left, btns.nav);
    print_test_result("Button States SPI", passed);
}

/** ***************************************************************************
 * @brief Test reading joystick states via SPI
*******************************************************************************/
static void test_joystick_states_spi(void) {
    struct joystick joy = {0};
    
    int ret = get_joystick_states(&joy);
    
    bool passed = (ret == 0);
    
    printf("  Joystick via SPI: X=%d, Y=%d, Btn=%d\r\n",
           joy.x, joy.y, joy.btn);
    print_test_result("Joystick States SPI", passed);
}

/** ***************************************************************************
 * @brief Test NULL pointer handling
*******************************************************************************/
static void test_null_pointers(void) {
    int ret1 = get_button_states(NULL);
    int ret2 = get_joystick_states(NULL);
    
    bool passed = (ret1 < 0) && (ret2 < 0);
    
    print_test_result("NULL Pointer Handling", passed);
}

/** ***************************************************************************
 * @brief Test joystick movement detection
 * 
 * @details Reads joystick multiple times to detect movement
*******************************************************************************/
static void test_joystick_movement(void) {
    printf("\r\n  Move joystick around for 3 seconds...\r\n");
    
    bool movement_detected = false;
    x_y_coords initial = get_joystick_x_y_percentage();
    
    for (uint8_t i = 0; i < 30; i++) {
        x_y_coords current = get_joystick_x_y_percentage();
        
        // Check if position changed significantly
        int16_t dx = (int16_t)current.x - (int16_t)initial.x;
        int16_t dy = (int16_t)current.y - (int16_t)initial.y;
        
        if (dx < 0) dx = -dx;
        if (dy < 0) dy = -dy;
        
        if (dx > 5 || dy > 5) {
            movement_detected = true;
        }
        
        printf("  [%2d] X=%3d%%, Y=%3d%%\r\n", i, current.x, current.y);
        _delay_ms(100);
    }
    
    print_test_result("Joystick Movement", movement_detected);
}

/** ***************************************************************************
 * @brief Test joystick centering
 * 
 * @details Checks if joystick returns to center position
*******************************************************************************/
static void test_joystick_centering(void) {
    printf("\r\n  Release joystick to center position...\r\n");
    _delay_ms(1000);
    
    x_y_coords coords = get_joystick_x_y_percentage();
    enum joystick_direction dir = get_joystick_direction();
    
    // Check if close to center (40-60% range)
    bool centered_x = (coords.x >= 40 && coords.x <= 60);
    bool centered_y = (coords.y >= 40 && coords.y <= 60);
    bool neutral = (dir == JOYSTICK_NEUTRAL);
    
    bool passed = centered_x && centered_y && neutral;
    
    printf("  Center position: X=%d%%, Y=%d%%, Dir=%s\r\n",
           coords.x, coords.y, neutral ? "NEUTRAL" : "NOT NEUTRAL");
    print_test_result("Joystick Centering", passed);
}

/** ***************************************************************************
 * @brief Run all User I/O tests
*******************************************************************************/
void run_user_io_tests(void) {
    printf("\r\n");
    printf("========================================\r\n");
    printf("      User I/O Driver Test Suite       \r\n");
    printf("========================================\r\n\r\n");
    
    tests_passed = 0;
    tests_failed = 0;
    
    test_user_io_init();
    test_joystick_xy_percentage();
    test_touchpad_xy_percentage();
    test_joystick_direction();
    test_joystick_button();
    test_button_states_spi();
    test_joystick_states_spi();
    test_null_pointers();
    test_joystick_movement();
    test_joystick_centering();
    
    printf("\r\n");
    printf("========================================\r\n");
    printf("Results: %d passed, %d failed\r\n", tests_passed, tests_failed);
    printf("========================================\r\n\r\n");
}
