/** *************************************************************************************************************************
 * @brief Struct for menu objects
 * 
 * @details Contains all data needed to represent and manage a menu
*******************************************************************************/
/**********************
 * @file gui.h
 * @author Magnus Carlsen Haaland
 * @brief GUI header file
 * @version 0.1
 * @date 2025-10-02
 * 
 * @copyright Copyright (c) 2025 Byggarane
 * 
*******************************************************************************/

#pragma once

#include <stdint.h>
#include "spi.h"

#define SELECT_ICON_WIDTH 12
#define SEL_X_THRESHOLD_LOWER 40
#define SEL_X_THRESHOLD_UPPER 60
#define SEL_Y_THRESHOLD_LOWER 20
#define SEL_Y_THRESHOLD_UPPER 80


/** ***************************************************************************
 * @brief Enum for GUI states
*******************************************************************************/

/** ***************************************************************************
 * @brief Enum for GUI application states
 * 
 * @details Defines the different states the GUI application can be in
*******************************************************************************/
enum gui_state {
    GUI_STATE_MENU,         /**< Main menu state */
    GUI_STATE_GAME,         /**< Game running state */
    GUI_STATE_GAME_OVER,    /**< Game over state */
    GUI_STATE_ERROR         /**< Error state */
};

/** ***************************************************************************
 * @brief Struct for menu items
 * 
 * @details Represents a single menu item with display text and associated action
*******************************************************************************/
struct __attribute__((packed)) menu_item{
    const uint8_t* string;      /**< String to display */
    void (*action)(void* arg);  /**< Function pointer to action with optional argument */
};

/** ***************************************************************************
 * @brief Struct for menu objects
*******************************************************************************/
struct __attribute__((packed)) menu{
    const uint8_t size;             /**< Number of items in menu */
    uint8_t sel;                    /**< Index of the selected item */
    uint8_t prev_sel;               /**< Index of previously selected item */
    const struct menu_item* items;  /**< List of menu items */
};

/**< Main menu object */
extern struct menu main_menu;

/**< Settings menu object */
extern struct menu settings_menu;

/** ***************************************************************************
 * @brief Draws a menu on the OLED display
 * 
 * @param[in] device Pointer to the SPI device structure for the OLED
 * @param[in] menu Pointer to menu object
*******************************************************************************/
void draw_menu(const struct menu* menu);

/** ***************************************************************************
 * @brief Updates the menu based on user input
 * 
 * @param[in] device Pointer to the SPI device structure for the OLED
 * @param[in,out] menu Pointer to menu object
 * @param[in,out] state Pointer to current GUI state (can be modified by actions)
 * @details Moves selector if joystick is pushed up or down.
 *          Redraws if selector changes
*******************************************************************************/
void update_menu(struct menu* menu, enum gui_state* state);