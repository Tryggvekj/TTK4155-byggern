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
    MCP2515_TXB0SIDH = 0x31,    /**< Transmit Buffer 0 Standard Identifier High */
    MCP2515_TXB0SIDL = 0x32,    /**< Transmit Buffer 0 Standard Identifier Low */
    MCP2515_TXB0EID8 = 0x33,    /**< Transmit Buffer 0 Extended Identifier High */
    MCP2515_TXB0EID0 = 0x34,    /**< Transmit Buffer 0 Extended Identifier Low */
    MCP2515_TXB0DLC = 0x35,     /**< Transmit Buffer 0 Data Length Code */
    MCP2515_TXB0DATA = 0x36,    /**< Transmit Buffer 0 Data Start */
    MCP2515_TXB1CTRL = 0x40,    /**< Transmit Buffer 1 Control Register */
    MCP2515_TXB2CTRL = 0x50,    /**< Transmit Buffer 2 Control Register */
    MCP2515_RXB0CTRL = 0x60,    /**< Receive Buffer 0 Control Register */
    MCP2515_RXB0SIDH = 0x61,    /**< Receive Buffer 0 Standard Identifier High */
    MCP2515_RXB0SIDL = 0x62,    /**< Receive Buffer 0 Standard Identifier Low */
    MCP2515_RXB0EID8 = 0x63,    /**< Receive Buffer 0 Extended Identifier High */
    MCP2515_RXB0EID0 = 0x64,    /**< Receive Buffer 0 Extended Identifier Low */
    MCP2515_RXB0DLC = 0x65,     /**< Receive Buffer 0 Data Length Code */
    MCP2515_RXB0DATA = 0x66,    /**< Receive Buffer 0 Data Start */
    MCP2515_RXB1CTRL = 0x70,    /**< Receive Buffer 1 Control Register */
    MCP2515_RXB1SIDH = 0x71,    /**< Receive Buffer 1 Standard Identifier High */
    MCP2515_RXB1SIDL = 0x72,    /**< Receive Buffer 1 Standard Identifier Low */
    MCP2515_RXB1EID8 = 0x73,    /**< Receive Buffer 1 Extended Identifier High */
    MCP2515_RXB1EID0 = 0x74,    /**< Receive Buffer 1 Extended Identifier Low */
    MCP2515_RXB1DLC = 0x75,     /**< Receive Buffer 1 Data Length Code */
    MCP2515_RXB1DATA = 0x76,    /**< Receive Buffer 1 Data Start */
    MCP2515_CANINTF = 0x2C      /**< CAN Interrupt Flag Register */
};


/** ***************************************************************************
 * @brief Initialize the MCP2515 CAN controller
 * 
 * @param[in] _mcp2515_dev SPI device structure for the MCP2515
 * @return int 0 on success, negative error code on failure
 * @details Configures the MCP2515 device and initializes its SPI interface
*******************************************************************************/
int mcp2515_init(const struct spi_device _mcp2515_dev);

/** ***************************************************************************
 * @brief Read a single register from the MCP2515
 * 
 * @param[in] address Register address to read from
 * @param[out] data Pointer to store the read value
 * @return int 0 on success, negative error code on failure
*******************************************************************************/
int mcp2515_read(uint8_t address, uint8_t* data);

/** ***************************************************************************
 * @brief Write a single byte to a MCP2515 register
 * 
 * @param[in] address Register address to write to
 * @param[in] data Data byte to write
 * @return int 0 on success, negative error code on failure
*******************************************************************************/
int mcp2515_write(uint8_t address, uint8_t data);

/** ***************************************************************************
 * @brief Write multiple sequential bytes to MCP2515 registers
 * 
 * @param[in] address Starting register address to write to
 * @param[in] data Pointer to data bytes to write
 * @param[in] length Number of bytes to write
 * @return int 0 on success, negative error code on failure
*******************************************************************************/
int mcp2515_write_multiple(uint8_t address, const uint8_t* data, uint8_t length);

/** ***************************************************************************
 * @brief Read multiple sequential bytes from MCP2515 registers
 * 
 * @param[in] address Starting register address to read from
 * @param[out] data Pointer to buffer to store read bytes
 * @param[in] length Number of bytes to read
 * @return int 0 on success, negative error code on failure
*******************************************************************************/
int mcp2515_read_multiple(uint8_t address, uint8_t* data, uint8_t length);

/** ***************************************************************************
 * @brief Request to send a message from one or more transmit buffers
 * 
 * @param[in] txb0 If true, request to send from transmit buffer 0
 * @param[in] txb1 If true, request to send from transmit buffer 1
 * @param[in] txb2 If true, request to send from transmit buffer 2
 * @return int 0 on success, negative error code on failure
*******************************************************************************/
int mcp2515_request_to_send(bool txb0, bool txb1, bool txb2);

/** ***************************************************************************
 * @brief Read the status of the MCP2515
 * 
 * @param[out] rx Buffer to store the read status bytes
 * @return int 0 on success, negative error code on failure
*******************************************************************************/
int mcp2515_read_status(uint8_t* rx);

/** ***************************************************************************
 * @brief Modify specific bits in a MCP2515 register
 * 
 * @param[in] address Register address to modify
 * @param[in] mask Bit mask indicating which bits to modify
 * @param[in] data New data for the bits specified by the mask
 * @return int 0 on success, negative error code on failure
*******************************************************************************/
int mcp2515_bit_modify(uint8_t address, uint8_t mask, uint8_t data);

/** ***************************************************************************
 * @brief Reset the MCP2515 device
 * 
 * @return int 0 on success, negative error code on failure
 * @details Sends the reset command to the MCP2515 to reinitialize it
*******************************************************************************/
int mcp2515_reset(void);

/** ***************************************************************************
 * @brief Print the contents of the config registers
 * 
 * @return int 0 on success, negative error code on failure
*******************************************************************************/
int mcp2515_print_config(void);