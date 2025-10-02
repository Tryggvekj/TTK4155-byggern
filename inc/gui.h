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
 * @brief Struct for menu objects
*******************************************************************************/
struct menu{
    const uint8_t size;         /**< Number of elements in menu */
    uint8_t sel;                /**< Index of the selected element */
    uint8_t prev_sel;           /**< Index of previously selected element */
    const uint8_t** strings;    /**< List of menu elements */
};

extern struct menu main_menu;

void draw_menu(const struct menu* menu);

void update_menu(struct menu* menu);