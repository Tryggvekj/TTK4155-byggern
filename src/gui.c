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

#define SELECT_ICON_WIDTH 12

/**< Main menu object */
struct menu main_menu = {
    .size = 5,
    .sel = 0,
    .prev_sel = 0, 
    .strings = (const uint8_t*[]){"Dette",
                                    "Er",
                                    "En",
                                    "Meny",
                                    "Byggve er dommedom"}
};

/**< Icon used to display the selected menu element */
const uint8_t* selected_icon = "->";


/** ***************************************************************************
 * @brief Draws a menu on the OLED display
 * 
 * @param[in] menu Menu object to draw 
*******************************************************************************/
void draw_menu(const struct menu* menu) {

    oled_clear();

    for(uint8_t i = 0; i < menu->size; i++) {
        if (menu->sel == i) {
            oled_draw_string(i, 0, selected_icon, 's');
            oled_draw_string(i, SELECT_ICON_WIDTH, menu->strings[i], 's');
        } else {
            oled_draw_string(i, SELECT_ICON_WIDTH, menu->strings[i], 's');
        }
    }

}

/** ***************************************************************************
 * @brief Update menu based on input
 * 
 * @param menu Menu object to be updated
 * @details Moves selector if joystick is pushed up or down.
 *          Redraws if selector changes
*******************************************************************************/
void update_menu(struct menu* menu) {

    // Redraw if selector has changed
    if(menu->sel != menu->prev_sel) {
        DEBUG_PRINT("Selector changed!\r\n");
        draw_menu(menu);
        menu->prev_sel = menu->sel;
    }

    // Read joystick position
    x_y_coords joystick_pos = get_joystick_x_y_percentage();
    
    // Move selector according to joystick input
    if (joystick_pos.x > 40 && joystick_pos.x < 60) {
        if (joystick_pos.y > 80) {
            if(menu->sel == 0) {
                menu->sel = menu->size;
            } else {
                menu->sel--;
            }
        } else if (joystick_pos.y < 20) {
            DEBUG_PRINT("Selector incremented\r\n");
            if(menu->sel == menu->size) {
                menu->sel = 0;
            } else {
                menu->sel++;
            }
        }

    }
}