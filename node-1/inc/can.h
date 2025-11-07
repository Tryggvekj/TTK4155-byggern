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
 * @brief CAN message identifiers
*******************************************************************************/
enum can_msg_id {
    CAN_ID_JOYSTICK = 0x01,
    CAN_ID_IR_LED = 0x02,
};

/** ***************************************************************************
 * @brief CAN bit timing configuration structure
*******************************************************************************/
struct __attribute__((packed)) can_config {
    union {
        struct {
            uint32_t phase2:4;  /**< Phase 2 segment */
            uint32_t propag:4;  /**< Propagation time segment */
            uint32_t phase1:4;  /**< Phase 1 segment */
            uint32_t sjw:4;     /**< Synchronization jump width */
            uint32_t brp:8;     /**< Baud rate prescaler */
            uint32_t smp:8;     /**< Sampling mode */
        };
        uint32_t reg;
    };
};

/** ***************************************************************************
 * @brief CAN message structure 
*******************************************************************************/
struct __attribute__((packed)) can_msg {
    uint32_t id;         /**< 11-bit or 29-bit identifier */
    uint8_t dlc;         /**< Data Length Code (0-8) */
    union {
        uint8_t bytes[8];     /**< Data bytes (0-8 bytes) */
        uint64_t data;
    };
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
int can_init(const struct spi_device _mcp2515_dev, enum can_mode mode, struct can_config cfg);

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