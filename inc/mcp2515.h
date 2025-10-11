/** ***************************************************************************
 * @file mcp2515.h
 * @author Byggarane
 * @brief MCP2515 CAN controller driver header file
 * @version 0.1
 * @date 2025-10-09
 * 
 * @copyright Copyright (c) 2025 Byggarane
 * 
*******************************************************************************/

#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "spi.h"

#define MCP2515_READ_INSTRUCTION 0x03
#define MCP2515_WRITE_INSTRUCTION 0x02


/** ***************************************************************************
 * @brief Initialize the MCP2515 CAN controller
 * 
 * @param[in] _mcp2515_dev SPI device structure for the MCP2515
 * @return int 0 on success, negative error code on failure
 * @details Configures the MCP2515 device and initializes its SPI interface
*******************************************************************************/
int mcp2515_init(struct spi_device _mcp2515_dev);

/** ***************************************************************************
 * @brief Read a single register from the MCP2515
 * 
 * @param[in] address Register address to read from
 * @return uint8_t The value read from the register
*******************************************************************************/
uint8_t mcp2515_read(uint8_t address);

/** ***************************************************************************
 * @brief Write a single byte to a MCP2515 register
 * 
 * @param[in] address Register address to write to
 * @param[in] data Data byte to write
 * @return bool True on success, false on failure
*******************************************************************************/
bool mcp2515_write(uint8_t address, uint8_t data);