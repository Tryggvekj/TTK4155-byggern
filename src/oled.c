/** ***************************************************************************
 * @file oled.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2025-10-02
 * 
 * @copyright Copyright (c) 2025
 * 
*******************************************************************************/


#include <stdio.h>
#include <string.h>

#include "fonts.h"
#include "gpio.h"
#include "oled.h"
#include "spi.h"


#define LOWER_COLUMN_MASK 0x0F
#define UPPER_COLUMN_MASK 0x10


/**< Command/data pin for the OLED */
static struct gpio_pin cmd_pin;

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
void oled_draw_char(uint8_t page, uint8_t column, char c, char font) {

    if ( c<32 || c>127) {

        c='?';
    }

    uint8_t byte;
    int width;

    if (font == 's') {
        width = 4;
    } else if (font == 'l') {
        width = 8;
    } else {
        width = 5;
    }

    for ( int i = 0; i < width; i++) { // writing each column in the char to the display

        if (font == 's') byte = pgm_read_byte(&font4[c - 32][i]);
        else if (font == 'l') byte = pgm_read_byte(&font8[c - 32][i]);
        else byte = pgm_read_byte(&font5[c - 32][i]);
        

        oled_goto_address(page, column + i + 1);
        oled_transmit(byte, false);
    }
}

/** ***************************************************************************
 * @brief Draw a string of characters on the OLED display
 * 
 * @param[in] page Page (row) to write in
 * @param[in] column Column to write in
 * @param[in] s String to be written
 * @param[in] font Specifies the font of the character
*******************************************************************************/
void oled_draw_string(const uint8_t page, const uint8_t column, const uint8_t* s, const uint8_t font) {

    int column_number;

    if (font == 's') column_number = 4;
    else if (font == 'l') column_number = 8;
    else column_number = 5;

    for (int i = 0; i < strlen(s); i++) {

        if (i * (column_number + 1) > 125) {
            
            oled_draw_char(page + 1, column + i*(column_number + 1), s[i], font);
        
        } else if (i * (column_number + 1) > 250) { 

            oled_draw_char(page, column + i*(column_number + 1), s[i], font);

        } else {

            oled_draw_char(page + 1, column + i*(column_number + 1), s[i], font);
        }
    }
}

/** ***************************************************************************
 * @brief Initialize the OLED display
 * 
 * @param[in] _cmd_pin GPIO pin for the OLED command/data selection
 * @details Configures the OLED display with default settings and turns it on
*******************************************************************************/
void oled_init(struct gpio_pin _cmd_pin)
{
    cmd_pin = _cmd_pin;
    gpio_init(cmd_pin, true);

    oled_transmit(OLED_SET_SEG_DIR, true);          // Set segment direction
    oled_transmit(OLED_SET_SCAN_DIR, true);         // Set scan direction
    oled_transmit(OLED_SET_RAM_START_LINE, true);   // Set display RAM start line to 0
    oled_transmit(OLED_TURN_ON_DISPLAY, true);      // Turn display on
    oled_transmit(OLED_SET_DISPLAY_NORM, true);     // Set display to normal mode
    oled_transmit(OLED_SHOW_FROM_MEM, true);        // Set the display to show from memory
}

/** ***************************************************************************
 * @brief Transmit data byte to the OLED using SPI
 * 
 * @param[in] data Data byte to be transmitted
 * @param[in] command Specifies if data byte is a command
*******************************************************************************/
void oled_transmit(uint8_t data, bool command) 
{
    gpio_set(cmd_pin, !command);
    spi_master_transmit_single(data, OLED_DEVICE_ID);
}

/** ***************************************************************************
 * @brief Transmit multiple data bytes to the OLED using SPI
 * 
 * @param[in] data Array of data bytes to be transmitted
 * @param[in] size Number of bytes to transmit
 * @param[in] command Specifies if data bytes are commands
*******************************************************************************/
void oled_transmit_multiple(uint8_t* data, uint8_t size, bool command) 
{
    gpio_set(cmd_pin, !command);
    spi_master_transmit(data, size, OLED_DEVICE_ID);
}

/** ***************************************************************************
 * @brief Selects position to write to
 * 
 * @param[in] page Page to select
 * @param[in] column Column to select
*******************************************************************************/
void oled_goto_address(uint8_t page, uint8_t column)
{
    if(page >= NUM_PAGES) {
        printf("Invalid page\r\n");
        return;
    }

    if(column >= NUM_COLUMNS) {
        printf("Invalid column\r\n");
        return;
    }

    uint8_t page_command = BASE_PAGE_COMMAND + page;
    uint8_t lower_column_address = column & LOWER_COLUMN_MASK;
    uint8_t higher_column_address = UPPER_COLUMN_MASK | (column >> 4);

    uint8_t commands[3] = {page_command, lower_column_address, higher_column_address};

    oled_transmit_multiple(commands, sizeof(commands), true);
}

/** ***************************************************************************
 * @brief Clear the OLED display
 * 
 * @details Writes 0x00 to all addresses
 * @todo Clean up :)
*******************************************************************************/
void oled_clear()
{
    enum oled_command horizontal_addressing[2] = {OLED_SET_MEM_ADDR_MODE, 0x00};
    oled_transmit_multiple((uint8_t*)horizontal_addressing, sizeof(horizontal_addressing), true);
    for (int i = 0; i < NUM_PAGES; i++) {
        for (int j = 0; j < NUM_COLUMNS; j++) {
            
            oled_transmit(0x00, false);
        }
    }
    enum oled_command page_addressing[2] = {OLED_SET_MEM_ADDR_MODE, 0x02};
    oled_transmit_multiple((uint8_t*)page_addressing, sizeof(page_addressing), true);
}