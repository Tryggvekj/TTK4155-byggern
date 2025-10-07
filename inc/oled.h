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

#pragma once

#include <stdbool.h>

#include <avr/io.h>

#include "gpio.h"

#define BASE_PAGE_COMMAND 0xB0
#define OLED_DEVICE_ID 0
#define NUM_PAGES 8
#define NUM_COLUMNS 128


/** ***************************************************************************
 * @brief OLED command set
 * 
 * @details Command codes for controlling the OLED display
*******************************************************************************/
enum oled_command {
    OLED_SET_SEG_DIR = 0xA1,            /**< Set segment direction normal */
    OLED_SET_SEG_DIR_INV = 0xA0,        /**< Set segment direction inverted */
    OLED_SET_SCAN_DIR = 0xC8,           /**< Set scan direction normal */
    OLED_SET_SCAN_DIR_INV = 0xC0,       /**< Set scan direction inverted */
    OLED_SET_RAM_START_LINE = 0x40,     /**< Set RAM start line to 0 */
    OLED_TURN_ON_DISPLAY = 0xAF,        /**< Turn display on */
    OLED_SET_DISPLAY_NORM = 0xA6,       /**< Set display to normal mode */
    OLED_SET_DISPLAY_INV = 0xA7,        /**< Set display to inverted mode */
    OLED_SHOW_FROM_MEM = 0xA4,          /**< Show display from memory */
    OLED_SET_MEM_ADDR_MODE = 0x20       /**< Set memory addressing mode */
};


/** ***************************************************************************
 * @brief Initialize the OLED display
 * 
 * @param[in] _cmd_pin GPIO pin for the OLED command/data selection
 * @details Configures the OLED display with default settings and turns it on
*******************************************************************************/
void oled_init(struct gpio_pin _cmd_pin);

/** ***************************************************************************
 * @brief Transmit data byte to the OLED using SPI
 * 
 * @param[in] data Data byte to be transmitted
 * @param[in] command Specifies if data byte is a command
*******************************************************************************/
void oled_transmit(uint8_t data, bool command);

/** ***************************************************************************
 * @brief Transmit multiple data bytes to the OLED using SPI
 * 
 * @param[in] data Array of data bytes to be transmitted
 * @param[in] size Number of bytes to transmit
 * @param[in] command Specifies if data bytes are commands
*******************************************************************************/
void oled_transmit_multiple(uint8_t* data, uint8_t size, bool command);

/** ***************************************************************************
 * @brief Selects position to write to
 * 
 * @param[in] page Page to select
 * @param[in] column Column to select
*******************************************************************************/
void oled_goto_address(uint8_t page, uint8_t column);

/** ***************************************************************************
 * @brief Draws a character on the OLED display
 * 
 * @param[in] page Page (row) to write in
 * @param[in] column Column to write in
 * @param[in] c Character to be written
 * @param[in] font Specifies the font of the character
 * 
 * @note Using ASCII 32-127 fonr (5x7)
*******************************************************************************/
void oled_draw_char(uint8_t page, uint8_t column, char c, char font);

/** ***************************************************************************
 * @brief Draw a string of characters on the OLED display
 * 
 * @param[in] page Page (row) to write in
 * @param[in] column Column to write in
 * @param[in] s String to be written
 * @param[in] font Specifies the font of the character
*******************************************************************************/
void oled_draw_string(const uint8_t page, const uint8_t column, const uint8_t* s, const uint8_t font);

/** ***************************************************************************
 * @brief Clear the OLED display
 * 
 * @details Writes 0x00 to all addresses
*******************************************************************************/
void oled_clear();