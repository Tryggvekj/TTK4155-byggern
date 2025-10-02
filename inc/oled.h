/** ***************************************************************************
 * @file oled.h
 * @author Walter Byggildsen, Magnus Carlsen Haaland
 * @brief 
 * @version 0.1
 * @date 2025-10-02
 * 
 * @copyright Copyright (c) 2025
 * 
*******************************************************************************/

#include <stdbool.h>
#include <avr/io.h>

#define BASE_PAGE_COMMAND 0xB0
#define OLED_DEVICE_ID 0
#define NUM_PAGES 8
#define NUM_COLUMNS 128


enum OLED_commands {

    OLED_SET_SEG_DIR = 0xA1,
    OLED_SET_SEG_DIR_INV = 0xA0,
    OLED_SET_SCAN_DIR = 0xC8,
    OLED_SET_SCAN_DIR_INV = 0xC0,
    OLED_SET_RAM_START_LINE = 0x40,
    OLED_TURN_ON_DISPLAY = 0xAF,
    OLED_SET_DISPLAY_NORM = 0xA6,
    OLED_SET_DISPLAY_INV = 0xA7,
    OLED_SHOW_FROM_MEM = 0xA4
};


void oled_init();

void oled_transmit(uint8_t data, bool command);

void oled_goto_address(uint8_t page, uint8_t column);

void oled_draw_char(uint8_t page, uint8_t column, char c, char font);

void oled_draw_string(const uint8_t page, const uint8_t column, const uint8_t* s, const uint8_t font);

void oled_clear();