/** ***************************************************************************
 * @file MCP2515.h
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

#define MCP2515_READ_INSTRUCTION 0x03
#define MCP2515_WRITE_INSTRUCTION 0x02

/** ***************************************************************************
 * @brief Read a single register from the MCP2515
 * 
 * @param[in] address Register address to read from
 * @return uint8_t The value read from the register
*******************************************************************************/
uint8_t MCP2515_read(uint8_t address);

bool MCP2515_write(uint8_t address, uint8_t data);