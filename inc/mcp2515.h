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


enum mcp2515_instruction {
    MCP2515_WRITE = 0x02,       /**< Write instruction */
    MCP2515_READ = 0x03,        /**< Read instruction */
    MCP2515_BIT_MODIFY = 0x05,  /**< Bit modify instruction */
    MCP2515_RTS_BASE = 0x80,    /**< Request to Send base instruction */
    MCP2515_READ_STATUS = 0xA0, /**< Read Status instruction */
    MCP2515_RESET = 0xC0        /**< Reset instruction */
};

enum mcp2515_register {
    MCP2515_CANCTRL = 0x0F,     /**< CAN Control Register */
    MCP2515_CANSTAT = 0x0E,     /**< CAN Status Register */
    MCP2515_CNF1 = 0x2A,        /**< Configuration Register 1 */
    MCP2515_CNF2 = 0x29,        /**< Configuration Register 2 */
    MCP2515_CNF3 = 0x28,        /**< Configuration Register 3 */
    MCP2515_TXB0CTRL = 0x30,    /**< Transmit Buffer 0 Control Register */
    MCP2515_TXB1CTRL = 0x40,    /**< Transmit Buffer 1 Control Register */
    MCP2515_TXB2CTRL = 0x50,    /**< Transmit Buffer 2 Control Register */
};


/** ***************************************************************************
 * @brief Initialize the MCP2515 CAN controller
 * 
 * @param[in] _mcp2515_dev SPI device structure for the MCP2515
 * @return int 0 on success, negative error code on failure
 * @details Configures the MCP2515 device and initializes its SPI interface
*******************************************************************************/
int mcp2515_init(const struct spi_device* _mcp2515_dev);

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

/** ***************************************************************************
 * @brief Request to send a message from one or more transmit buffers
 * 
 * @param[in] txb0 If true, request to send from transmit buffer 0
 * @param[in] txb1 If true, request to send from transmit buffer 1
 * @param[in] txb2 If true, request to send from transmit buffer 2
 * @return bool True on success, false on failure
*******************************************************************************/
bool mcp2515_request_to_send(bool txb0, bool txb1, bool txb2);

/** ***************************************************************************
 * @brief Read the status of the MCP2515
 * 
 * @param[out] rx Buffer to store the read status bytes
 * @return bool True on success, false on failure
*******************************************************************************/
bool mcp2515_read_status(uint8_t* rx);

/** ***************************************************************************
 * @brief Modify specific bits in a MCP2515 register
 * 
 * @param[in] address Register address to modify
 * @param[in] mask Bit mask indicating which bits to modify
 * @param[in] data New data for the bits specified by the mask
 * @return bool True on success, false on failure
*******************************************************************************/
bool mcp2515_bit_modify(uint8_t address, uint8_t mask, uint8_t data);

/** ***************************************************************************
 * @brief Reset the MCP2515 device
 * 
 * @return bool True on success, false on failure
 * @details Sends the reset command to the MCP2515 to reinitialize it
*******************************************************************************/
bool mcp2515_reset(void);