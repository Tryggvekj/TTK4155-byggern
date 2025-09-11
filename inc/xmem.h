/** ***************************************************************************
 * @file xmem.h
 * @author Magnuszsew Carlsewitz Haalski
 * @brief External memory driver header file
 * @version 0.1
 * @date 2025-09-11
 * 
 * @copyright Copyright (c) 2025 Byggarane
 * 
*******************************************************************************/

#pragma once

#define BASE_ADDRESS 0x1800
#define SRAM_SIZE 0x800
#define ADC_SIZE 0x800


/** ***************************************************************************
 * @brief Initialize the external memory interface
 * 
*******************************************************************************/
void xmem_init(void);

/** ***************************************************************************
 * @brief Write a data byte to the specified external memory address
 * 
 * @param[in] data The data byte to write
 * @param[in] addr The external memory address to write to
*******************************************************************************/
void xmem_write(uint8_t data, uint16_t addr);

/** ***************************************************************************
 * @brief Read a data byte from the specified external memory address
 * 
 * @param[in] addr The external memory address to read from
 * @return uint8_t The data byte read from the specified address
*******************************************************************************/
uint8_t xmem_read(uint16_t addr);

/** ***************************************************************************
 * @brief Test the external SRAM
 * 
 * @details Writes a series of pseudo-random values and reads them back to verify
 *          SRAM functionality.
*******************************************************************************/
void SRAM_test(void);