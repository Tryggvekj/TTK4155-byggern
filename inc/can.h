/** ***************************************************************************
 * @file can.h
 * @author Byggarane
 * @brief CAN driver header file
 * @version 0.1
 * @date 2025-10-09
 * 
 * @copyright Copyright (c) 2025 Byggarane
 * 
*******************************************************************************/

#pragma once

#include "mcp2515.h"

#define CAN_MODE_OFFSET 5
#define CAN_MODE_MASK 0xE0

/** ***************************************************************************
 * @brief CAN message structure 
*******************************************************************************/
struct can_msg {
    uint32_t id;         /**< 11-bit or 29-bit identifier */
    uint8_t dlc;         /**< Data Length Code (0-8) */
    uint8_t data[8];     /**< Data bytes (0-8 bytes) */
};

/** ***************************************************************************
 * @brief CAN operating modes
*******************************************************************************/
enum can_mode {
    CAN_MODE_NORMAL = 0,   /**< Normal operation mode */
    CAN_MODE_SLEEP,        /**< Sleep mode */
    CAN_MODE_LOOPBACK,     /**< Loopback mode for self-test */
    CAN_MODE_LISTEN_ONLY,  /**< Listen-only mode */
    CAN_MODE_CONFIG        /**< Configuration mode */
};



/** ***************************************************************************
 * @brief Initialize the CAN controller (MCP2515)
 * 
 * @param[in] _mcp2515_dev SPI device structure for the MCP2515
 * @param[in] mode CAN operating mode to set
 * @return int 0 on success, negative error code on failure
*******************************************************************************/
int can_init(const struct spi_device _mcp2515_dev, enum can_mode mode);

/** ***************************************************************************
 * @brief Send a CAN message
 * 
 * @param[in] msg Pointer to the CAN message to send
 * @return int 0 on success, negative error code on failure
*******************************************************************************/
int can_send(const struct can_msg* msg);

/** ***************************************************************************
 * @brief Receive a CAN message
 * 
 * @param[out] msg Pointer to the CAN message structure to store received message
 * @return int 0 on success, negative error code on failure
*******************************************************************************/
int can_receive(struct can_msg* msg);