/** ***************************************************************************
 * @file gui.c
 * @author Magnus Carlsen Haaland
 * @brief Module for handling GUI
 * @version 0.1
 * @date 2025-10-02
 * 
 * @copyright Copyright (c) 2025 Byggarane
 * 
*******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "debug.h"
#include "oled.h"
#include "gui.h"
#include "user_io.h"



/**< Icon used to display the selected menu element */
const uint8_t* selected_icon = "->";

/**< Main menu object */
struct menu main_menu = {
    .size = 5,
    .sel = 0,
    .prev_sel = 0, 
    .items = (struct menu_item[]){
        {.string = "Play game", .action = NULL},
        {.string = "Settings", .action = NULL},
        {.string = "Info", .action = NULL},
        {.string = "Calibrate", .action = NULL},
        {.string = "Shutdown", .action = NULL}
    }
};


/** ***************************************************************************
 * @brief Draws a menu on the OLED display
 * 
 * @param[in] menu Pointer to menu object
*******************************************************************************/
void draw_menu(const struct menu* menu) {

    oled_clear();

    for(uint8_t i = 0; i < menu->size; i++) {
        if (menu->sel == i) {
            oled_draw_string(i, 0, selected_icon, 's');
            oled_draw_string(i, SELECT_ICON_WIDTH, menu->items[i].string, 's');
        } else {
            oled_draw_string(i, SELECT_ICON_WIDTH, menu->items[i].string, 's');
        }
    }

}

/** ***************************************************************************
 * @brief Updates the menu based on user input
 * 
 * @param menu Pointer to menu object
 * @details Moves selector if joystick is pushed up or down.
 *          Redraws if selector changes
*******************************************************************************/
void update_menu(struct menu* menu) {

    // Redraw if selector has changed
    if(menu->sel != menu->prev_sel) {
        draw_menu(menu);
        menu->prev_sel = menu->sel;
    }

    // Read joystick position
    x_y_coords joystick_pos = get_joystick_x_y_percentage();
    
    // Move selector according to joystick input
    if (joystick_pos.x > SEL_X_THRESHOLD_LOWER && joystick_pos.x < SEL_X_THRESHOLD_UPPER) {
        if (joystick_pos.y > SEL_Y_THRESHOLD_UPPER) {
            if(menu->sel == 0) {
                menu->sel = menu->size - 1;
            } else {
                menu->sel--;
            }
        } else if (joystick_pos.y < SEL_Y_THRESHOLD_LOWER) {
            if(menu->sel >= menu->size - 1) {
                menu->sel = 0;
            } else {
                menu->sel++;
            }
        }

    }
}