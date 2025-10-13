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


#include <errno.h>
#include <stdio.h>
#include <string.h>

#define F_CPU 4915200 // Hz
#include <util/delay.h>

#include "fonts.h"
#include "gpio.h"
#include "oled.h"
#include "spi.h"


#define LOWER_COLUMN_MASK 0x0F
#define UPPER_COLUMN_MASK 0x10


/**< OLED device structure */
static struct oled_dev oled_device;

/** ***************************************************************************
 * @brief Draws a character on the OLED display
 * 
 * @param[in] page Page (row) to write in
 * @param[in] column Column to write in
 * @param[in] c Character to be written
 * @param[in] font Specifies the font of the character
 * 
 * @note Using ASCII 32-127 fonr (5x7)
 * @return int 0 on success, negative error code on failure
*******************************************************************************/
int oled_draw_char(uint8_t page, uint8_t column, char c, char font) {

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
        

        int ret = oled_goto_address(page, column + i + 1);
        if (ret) {
            return ret;
        }
        
        ret = oled_transmit_single(byte, false);
        if (ret) {
            return ret;
        }
    }
    
    return 0;
}

/** ***************************************************************************
 * @brief Draw a string of characters on the OLED display
 * 
 * @param[in] page Page (row) to write in
 * @param[in] column Column to write in
 * @param[in] s String to be written
 * @param[in] font Specifies the font of the character
 * @return int 0 on success, negative error code on failure
*******************************************************************************/
int oled_draw_string(const uint8_t page, const uint8_t column, const uint8_t* s, const uint8_t font) {

    int column_number;

    if (font == 's') column_number = 4;
    else if (font == 'l') column_number = 8;
    else column_number = 5;

    for (int i = 0; i < strlen(s); i++) {
        int ret;

        if (i * (column_number + 1) > 125) {
            ret = oled_draw_char(page + 1, column + i*(column_number + 1), s[i], font);
        } else if (i * (column_number + 1) > 250) { 
            ret = oled_draw_char(page, column + i*(column_number + 1), s[i], font);
        } else {
            ret = oled_draw_char(page + 1, column + i*(column_number + 1), s[i], font);
        }
        
        if (ret) {
            return ret;
        }
    }
    
    return 0;
}

/** ***************************************************************************
 * @brief Initialize the OLED display
 * 
 * @param[in] _oled_device OLED device structure containing SPI device and command pin
 * @details Configures the OLED display with default settings and turns it on
 * @return int 0 on success, negative error code on failure
*******************************************************************************/
int oled_init(const struct oled_dev _oled_device)
{
    oled_device = _oled_device;
    
    // Initialize SPI device first
    int ret = spi_device_init(&oled_device.spi);
    if (ret) {
        return ret;
    }
    
    // Initialize command pin as output and set to high (data mode initially)
    gpio_init(oled_device.cmd_pin, OUTPUT);
    gpio_set(oled_device.cmd_pin, HIGH);

    // Add a small delay to ensure hardware is ready
    _delay_ms(10);

    //TODO: Make array and transmit all at once
    ret = oled_transmit_single(OLED_SET_SEG_DIR, true);          // Set segment direction
    if (ret) return ret;
    ret = oled_transmit_single(OLED_SET_SCAN_DIR, true);         // Set scan direction
    if (ret) return ret;
    ret = oled_transmit_single(OLED_SET_RAM_START_LINE, true);   // Set display RAM start line to 0
    if (ret) return ret;
    ret = oled_transmit_single(OLED_TURN_ON_DISPLAY, true);      // Turn display on
    if (ret) return ret;
    ret = oled_transmit_single(OLED_SET_DISPLAY_NORM, true);     // Set display to normal mode
    if (ret) return ret;
    ret = oled_transmit_single(OLED_SHOW_FROM_MEM, true);        // Set the display to show from memory
    if (ret) return ret;
    
    return 0;
}

/** ***************************************************************************
 * @brief Transmit data byte to the OLED using SPI
 * 
 * @param[in] data Data byte to be transmitted
 * @param[in] command Specifies if data byte is a command
 * @return int 0 on success, negative error code on failure
*******************************************************************************/
int oled_transmit_single(uint8_t data, bool command) 
{
    gpio_set(oled_device.cmd_pin, !command);
    return spi_master_transmit_single(&oled_device.spi, data);
}

/** ***************************************************************************
 * @brief Transmit multiple data bytes to the OLED using SPI
 * 
 * @param[in] data Array of data bytes to be transmitted
 * @param[in] size Number of bytes to transmit
 * @param[in] command Specifies if data bytes are commands
 * @return int 0 on success, negative error code on failure
*******************************************************************************/
int oled_transmit(uint8_t* data, uint8_t size, bool command) 
{
    gpio_set(oled_device.cmd_pin, !command);
    return spi_master_transmit(&oled_device.spi, data, size);
}

/** ***************************************************************************
 * @brief Selects position to write to
 * 
 * @param[in] page Page to select
 * @param[in] column Column to select
 * @return int 0 on success, negative error code on failure
*******************************************************************************/
int oled_goto_address(uint8_t page, uint8_t column)
{
    if(page >= NUM_PAGES) {
        printf("Invalid page\r\n");
        return -EINVAL;
    }

    if(column >= NUM_COLUMNS) {
        printf("Invalid column\r\n");
        return -EINVAL;
    }

    uint8_t page_command = BASE_PAGE_COMMAND + page;
    uint8_t lower_column_address = column & LOWER_COLUMN_MASK;
    uint8_t higher_column_address = UPPER_COLUMN_MASK | (column >> 4);

    uint8_t commands[3] = {page_command, lower_column_address, higher_column_address};

    return oled_transmit(commands, sizeof(commands), true);
}

/** ***************************************************************************
 * @brief Clear the OLED display
 * 
 * @details Writes 0x00 to all addresses
 * @todo Clean up :)
 * @return int 0 on success, negative error code on failure
*******************************************************************************/
int oled_clear(void)
{
    enum oled_command horizontal_addressing[2] = {OLED_SET_MEM_ADDR_MODE, 0x00};
    int ret = oled_transmit((uint8_t*)horizontal_addressing, sizeof(horizontal_addressing), true);
    if (ret) {
        return ret;
    }
    
    for (int i = 0; i < NUM_PAGES; i++) {
        for (int j = 0; j < NUM_COLUMNS; j++) {
            ret = oled_transmit_single(0x00, false);
            if (ret) {
                return ret;
            }
        }
    }
    
    enum oled_command page_addressing[2] = {OLED_SET_MEM_ADDR_MODE, 0x02};
    return oled_transmit((uint8_t*)page_addressing, sizeof(page_addressing), true);
}