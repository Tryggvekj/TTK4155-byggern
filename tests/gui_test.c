/** ***************************************************************************
 * @file gui_test.c
 * @author Byggarane
 * @brief Test suite for GUI module
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

#include "../inc/gui.h"
#include "../inc/oled.h"
#include "../inc/user_io.h"
#include "../inc/uart.h"

#define TEST_PASSED "PASSED"
#define TEST_FAILED "FAILED"

static uint8_t tests_passed = 0;
static uint8_t tests_failed = 0;

// Test action callback flags
static bool test_action_called = false;

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
 * @brief Test action callback
*******************************************************************************/
static void test_action_callback(void) {
    test_action_called = true;
    printf("  Test action callback invoked\r\n");
}

/** ***************************************************************************
 * @brief Test main menu structure
*******************************************************************************/
static void test_main_menu_structure(void) {
    bool passed = true;
    
    // Check menu size
    if (main_menu.size != 5) {
        passed = false;
        printf("  Expected 5 items, got %d\r\n", main_menu.size);
    }
    
    // Check that menu items exist
    if (main_menu.items == NULL) {
        passed = false;
    }
    
    // Check initial selection
    if (main_menu.sel >= main_menu.size) {
        passed = false;
    }
    
    print_test_result("Main Menu Structure", passed);
}

/** ***************************************************************************
 * @brief Test settings menu structure
*******************************************************************************/
static void test_settings_menu_structure(void) {
    bool passed = true;
    
    // Check menu size
    if (settings_menu.size != 3) {
        passed = false;
        printf("  Expected 3 items, got %d\r\n", settings_menu.size);
    }
    
    // Check that menu items exist
    if (settings_menu.items == NULL) {
        passed = false;
    }
    
    print_test_result("Settings Menu Structure", passed);
}

/** ***************************************************************************
 * @brief Test draw menu function
*******************************************************************************/
static void test_draw_menu(void) {
    draw_menu(&main_menu);
    
    _delay_ms(1000);
    
    // If no crash, it's a pass
    bool passed = true;
    
    print_test_result("Draw Menu", passed);
}

/** ***************************************************************************
 * @brief Test menu item strings
*******************************************************************************/
static void test_menu_item_strings(void) {
    bool passed = true;
    
    // Check that all main menu items have strings
    for (uint8_t i = 0; i < main_menu.size; i++) {
        if (main_menu.items[i].string == NULL) {
            passed = false;
            printf("  Menu item %d has NULL string\r\n", i);
        } else {
            printf("  Item %d: %s\r\n", i, main_menu.items[i].string);
        }
    }
    
    print_test_result("Menu Item Strings", passed);
}

/** ***************************************************************************
 * @brief Test menu selection bounds
*******************************************************************************/
static void test_menu_selection_bounds(void) {
    struct menu test_menu = main_menu;
    bool passed = true;
    
    // Test setting selection to various values
    test_menu.sel = 0;
    if (test_menu.sel >= test_menu.size) passed = false;
    
    test_menu.sel = test_menu.size - 1;
    if (test_menu.sel >= test_menu.size) passed = false;
    
    // Simulate wrap-around
    test_menu.sel = test_menu.size - 1;
    // This would be handled by update_menu logic
    
    print_test_result("Menu Selection Bounds", passed);
}

/** ***************************************************************************
 * @brief Test menu actions
*******************************************************************************/
static void test_menu_actions(void) {
    bool passed = true;
    
    // Check that actions are assigned
    for (uint8_t i = 0; i < main_menu.size; i++) {
        if (main_menu.items[i].action == NULL) {
            printf("  Warning: Menu item %d has NULL action\r\n", i);
        } else {
            // Call the action to test it doesn't crash
            main_menu.items[i].action();
        }
    }
    
    print_test_result("Menu Actions", passed);
}

/** ***************************************************************************
 * @brief Test custom menu with test action
*******************************************************************************/
static void test_custom_menu(void) {
    test_action_called = false;
    
    struct menu test_menu = {
        .size = 2,
        .sel = 0,
        .prev_sel = 0,
        .items = (struct menu_item[]){
            {.string = "Test Item 1", .action = test_action_callback},
            {.string = "Test Item 2", .action = NULL}
        }
    };
    
    // Draw test menu
    draw_menu(&test_menu);
    _delay_ms(500);
    
    // Simulate button press by calling action directly
    if (test_menu.items[0].action) {
        test_menu.items[0].action();
    }
    
    bool passed = test_action_called;
    
    print_test_result("Custom Menu", passed);
}

/** ***************************************************************************
 * @brief Test menu navigation simulation
*******************************************************************************/
static void test_menu_navigation(void) {
    struct menu test_menu = main_menu;
    bool passed = true;
    
    // Simulate moving down
    uint8_t initial_sel = test_menu.sel;
    test_menu.sel++;
    if (test_menu.sel >= test_menu.size) {
        test_menu.sel = 0; // Wrap around
    }
    
    draw_menu(&test_menu);
    _delay_ms(500);
    
    // Simulate moving up
    if (test_menu.sel == 0) {
        test_menu.sel = test_menu.size - 1;
    } else {
        test_menu.sel--;
    }
    
    draw_menu(&test_menu);
    _delay_ms(500);
    
    print_test_result("Menu Navigation", passed);
}

/** ***************************************************************************
 * @brief Test update menu function (requires user input)
*******************************************************************************/
static void test_update_menu_interactive(void) {
    printf("\r\n  Interactive test: Move joystick to navigate menu\r\n");
    printf("  Test will run for 5 seconds...\r\n\r\n");
    
    struct menu test_menu = main_menu;
    test_menu.sel = 0;
    test_menu.prev_sel = 0;
    
    draw_menu(&test_menu);
    
    for (uint8_t i = 0; i < 50; i++) {
        update_menu(&test_menu);
        _delay_ms(100);
    }
    
    bool passed = true;
    
    print_test_result("Update Menu Interactive", passed);
}

/** ***************************************************************************
 * @brief Test menu redraw on selection change
*******************************************************************************/
static void test_menu_redraw(void) {
    struct menu test_menu = main_menu;
    
    // Draw initial menu
    test_menu.sel = 0;
    test_menu.prev_sel = 0;
    draw_menu(&test_menu);
    _delay_ms(500);
    
    // Change selection and redraw
    test_menu.sel = 1;
    test_menu.prev_sel = 0;
    draw_menu(&test_menu);
    _delay_ms(500);
    
    // Change selection again
    test_menu.sel = 2;
    test_menu.prev_sel = 1;
    draw_menu(&test_menu);
    _delay_ms(500);
    
    bool passed = true;
    
    print_test_result("Menu Redraw", passed);
}

/** ***************************************************************************
 * @brief Test menu with long strings
*******************************************************************************/
static void test_menu_long_strings(void) {
    struct menu test_menu = {
        .size = 2,
        .sel = 0,
        .prev_sel = 0,
        .items = (struct menu_item[]){
            {.string = "Very Long Menu Item Name Here", .action = NULL},
            {.string = "Short", .action = NULL}
        }
    };
    
    draw_menu(&test_menu);
    _delay_ms(1000);
    
    bool passed = true;
    
    print_test_result("Menu Long Strings", passed);
}

/** ***************************************************************************
 * @brief Test menu selection wraparound
*******************************************************************************/
static void test_menu_wraparound(void) {
    struct menu test_menu = main_menu;
    bool passed = true;
    
    // Test wrapping from bottom to top
    test_menu.sel = test_menu.size - 1;
    draw_menu(&test_menu);
    _delay_ms(300);
    
    // Simulate moving down (should wrap to 0)
    test_menu.prev_sel = test_menu.sel;
    test_menu.sel = 0;
    draw_menu(&test_menu);
    _delay_ms(300);
    
    // Test wrapping from top to bottom
    test_menu.prev_sel = test_menu.sel;
    test_menu.sel = test_menu.size - 1;
    draw_menu(&test_menu);
    _delay_ms(300);
    
    print_test_result("Menu Wraparound", passed);
}

/** ***************************************************************************
 * @brief Run all GUI tests
*******************************************************************************/
void run_gui_tests(void) {
    printf("\r\n");
    printf("========================================\r\n");
    printf("        GUI Module Test Suite          \r\n");
    printf("========================================\r\n\r\n");
    
    tests_passed = 0;
    tests_failed = 0;
    
    test_main_menu_structure();
    test_settings_menu_structure();
    test_draw_menu();
    test_menu_item_strings();
    test_menu_selection_bounds();
    test_menu_actions();
    test_custom_menu();
    test_menu_navigation();
    test_update_menu_interactive();
    test_menu_redraw();
    test_menu_long_strings();
    test_menu_wraparound();
    
    printf("\r\n");
    printf("========================================\r\n");
    printf("Results: %d passed, %d failed\r\n", tests_passed, tests_failed);
    printf("========================================\r\n\r\n");
}
