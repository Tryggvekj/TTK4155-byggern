/** ***************************************************************************
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

#define SELECT_ICON_WIDTH 12
#define SEL_X_THRESHOLD_LOWER 40
#define SEL_X_THRESHOLD_UPPER 60
#define SEL_Y_THRESHOLD_LOWER 20
#define SEL_Y_THRESHOLD_UPPER 80


/** ***************************************************************************
 * @brief Struct for menu items
*******************************************************************************/
struct menu_item{
    const uint8_t* string;  /**< String to display */
    void (*action)(void);   /**< Function pointer to action */
};

/** ***************************************************************************
 * @brief Struct for menu objects
*******************************************************************************/
struct menu{
    const uint8_t size;             /**< Number of items in menu */
    uint8_t sel;                    /**< Index of the selected item */
    uint8_t prev_sel;               /**< Index of previously selected item */
    const struct menu_item* items;  /**< List of menu items */
};

/**< Main menu object */
extern struct menu main_menu;

/** ***************************************************************************
 * @brief Draws a menu on the OLED display
 * 
 * @param[in] menu Pointer to menu object
*******************************************************************************/
void draw_menu(const struct menu* menu);

/** ***************************************************************************
 * @brief Updates the menu based on user input
 * 
 * @param menu Pointer to menu object
 * @details Moves selector if joystick is pushed up or down.
 *          Redraws if selector changes
*******************************************************************************/
void update_menu(struct menu* menu);