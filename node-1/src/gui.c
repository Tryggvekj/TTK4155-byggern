/** ***************************************************************************
 * @file gui.c
 * @author Magnus Carlsen Haaland, Tryggve Klevstul-Jensen, Walter Brynildsen
 * @brief Module for handling GUI
 * @version 0.1
 * @date 2025-10-02
 *
 * @copyright Copyright (c) 2025 Byggarane
 *
 *******************************************************************************/

#include <stdint.h>
#include <stdio.h>

#define F_CPU 4915200 // Hz
#include <util/delay.h>

#include "debug.h"
#include "gui.h"
#include "oled.h"
#include "user_io.h"

#define JS_BTN_CLICK_DELAY_MS 200
#define JS_SEL_DELAY_MS 100

/**< Icon used to display the selected menu element */
const uint8_t *selected_icon = "->";

static void action_play_game(void *arg)
{
    enum gui_state *state = (enum gui_state *)arg;
    if (state)
    {
        *state = GUI_STATE_WAIT_START;
    }
    else
    {
        DEBUG_PRINT("Play Game action selected.\r\n");
    }
}

/*
static void action_display_scoreboard(void* arg) {
    (void)arg; // Unused
    DEBUG_PRINT("Display Scoreboard action selected.\r\n");
}

static void action_display_info(void* arg) {
    (void)arg; // Unused
    DEBUG_PRINT("GUI Info: This is a demo GUI for the embedded system.\r\n");
}

static void action_calibrate(void* arg) {
    (void)arg; // Unused
    DEBUG_PRINT("Calibrate action selected.\r\n");
}

static void action_open_settings(void* arg) {
    (void)arg; // Unused
    DEBUG_PRINT("Open Settings action selected.\r\n");
}

*/

/**< Main menu object */
struct menu main_menu = {
    .size = 5,
    .sel = 0,
    .prev_sel = 0,
    .items = (struct menu_item[]){
        {.string = "Play game", .action = action_play_game},
        {.string = "Scoreboard", .action = 0},
        {.string = "Info", .action = 0},
        {.string = "Calibrate", .action = 0},
        {.string = "Settings", .action = 0}}};

/**< Settings menu object */
struct menu settings_menu = {
    .size = 3,
    .sel = 0,
    .prev_sel = 0,
    .items = (struct menu_item[]){
        {.string = "Sound", .action = 0},
        {.string = "Brightness", .action = 0},
        {.string = "Back", .action = 0}}};

static void action_retry_game(void *arg)
{
    enum gui_state *state = (enum gui_state *)arg;
    if (state)
    {
        *state = GUI_STATE_GAME;
    }
    else
    {
        DEBUG_PRINT("Retry Game action selected.\r\n");
    }
}

static void action_return_to_main_menu(void *arg)
{
    enum gui_state *state = (enum gui_state *)arg;
    if (state)
    {
        *state = GUI_STATE_MENU;
    }
    else
    {
        DEBUG_PRINT("Return to Main Menu action selected.\r\n");
    }
}

/**< Game over menu object */
struct menu game_over_menu = {
    .size = 2,
    .sel = 0,
    .prev_sel = 0,
    .items = (struct menu_item[]){
        {.string = "Retry", .action = action_retry_game},
        {.string = "Main Menu", .action = action_return_to_main_menu}}};

/** ***************************************************************************
 * @brief Draws a menu on the OLED display
 *
 * @param[in] menu Pointer to menu object
 *******************************************************************************/
void draw_menu(const struct menu *menu)
{

    oled_clear();

    for (uint8_t i = 0; i < menu->size; i++)
    {
        if (menu->sel == i)
        {
            // Ignore errors for now - GUI code can be improved later
            (void)oled_draw_string(i, 0, selected_icon, 's');
            (void)oled_draw_string(i, SELECT_ICON_WIDTH, menu->items[i].string, 's');
        }
        else
        {
            (void)oled_draw_string(i, SELECT_ICON_WIDTH, menu->items[i].string, 's');
        }
    }
}

/** ***************************************************************************
 * @brief Updates the menu based on user input
 *
 * @param[in,out] menu Pointer to menu object
 * @param[in,out] state Pointer to current GUI state (can be modified by actions)
 * @details Moves selector if joystick is pushed up or down.
 *          Redraws if selector changes
 *******************************************************************************/
void update_menu(struct menu *menu, enum gui_state *state)
{

    // Read joystick button state
    bool js_btn_state = get_joystick_btn_state();
    if (js_btn_state)
    {
        // Execute action of selected menu item if button is pressed
        if (menu->items[menu->sel].action)
        {
            menu->items[menu->sel].action(state); // Pass state pointer to action
        }
        _delay_ms(JS_BTN_CLICK_DELAY_MS);
        return; // Avoid moving the selector on the same update
    }

    // Read joystick position
    x_y_coords joystick_pos = get_joystick_x_y_percentage();

    // Move selector according to joystick input
    if (joystick_pos.x > SEL_X_THRESHOLD_LOWER && joystick_pos.x < SEL_X_THRESHOLD_UPPER)
    {
        if (joystick_pos.y > SEL_Y_THRESHOLD_UPPER)
        {
            if (menu->sel == 0)
            {
                menu->sel = menu->size - 1;
            }
            else
            {
                menu->sel--;
                _delay_ms(JS_SEL_DELAY_MS);
            }
        }
        else if (joystick_pos.y < SEL_Y_THRESHOLD_LOWER)
        {
            if (menu->sel >= menu->size - 1)
            {
                menu->sel = 0;
            }
            else
            {
                menu->sel++;
                _delay_ms(JS_SEL_DELAY_MS);
            }
        }
    }

    // Redraw if selector has changed
    if (menu->sel != menu->prev_sel)
    {
        draw_menu(menu);
        menu->prev_sel = menu->sel;
    }
}