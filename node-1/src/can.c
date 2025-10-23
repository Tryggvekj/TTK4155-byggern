/** ***************************************************************************
 * @file can.c
 * @author Byggarane
 * @brief CAN driver implementation file
 * @version 0.1
 * @date 2025-10-09
 * 
 * @copyright Copyright (c) 2025 Byggarane
 * 
*******************************************************************************/

#include <errno.h>
#include "can.h"
#include "debug.h"

static int can_select_mode(enum can_mode mode);
static int can_set_timing(uint8_t baudrate);

uint8_t rx_data[13] = {0};

/** ***************************************************************************
 * @brief Initialize the CAN controller (MCP2515) and set operation mode
 * 
 * @param[in] _mcp2515_dev SPI device structure for the MCP2515
 * @param[in] mode CAN operating mode to set
 * @return int 0 on success, negative error code on failure
*******************************************************************************/
int can_init(const struct spi_device _mcp2515_dev, enum can_mode mode, uint8_t brp) {
    int ret = mcp2515_init(_mcp2515_dev);
    if(ret) {
        return ret;
    }
    ret = can_set_timing(brp);
    if(ret) {
        return ret;
    }
    return can_select_mode(mode);
}

/** ***************************************************************************
 * @brief Send a CAN message
 * 
 * @param[in] msg Pointer to the CAN message to send
 * @return int 0 on success, negative error code on failure
*******************************************************************************/
int can_send(const struct can_msg* msg) {
    if (!msg || msg->dlc > 8) {
        return -EINVAL;
    }
    
    // Prepare transmit buffer data (13 bytes total)
    // SIDH, SIDL, EID8, EID0, DLC, DATA0-7
    uint8_t tx_data[13] = {0};
    
    // Standard Identifier High (bits 10-3 of ID)
    tx_data[0] = (msg->id >> 3) & 0xFF;
    
    // Standard Identifier Low (bits 2-0 in bits 7-5)
    tx_data[1] = (msg->id << 5) & 0xE0;
    
    // EID8 and EID0 are left as 0 (not using extended IDs)
    tx_data[2] = 0; // EID8
    tx_data[3] = 0; // EID0
    
    // Data Length Code (lower 4 bits)
    tx_data[4] = msg->dlc & 0x0F;
    
    // Copy data bytes
    for (uint8_t i = 0; i < msg->dlc; i++) {
        tx_data[5 + i] = msg->data[i];
    }
    
    // Write all data to TXB0 starting at SIDH register
    int ret = mcp2515_write_multiple(MCP2515_TXB0SIDH, tx_data, 13);
    if (ret) {
        return ret;
    }
    
    // Request to send from TXB0
    return mcp2515_request_to_send(true, false, false);
}

/** ***************************************************************************
 * @brief Receive a CAN message
 * 
 * @param[out] msg Pointer to the CAN message structure to store received message
 * @return int 0 on success, negative error code on failure
*******************************************************************************/
int can_receive(struct can_msg* msg) {
    if (!msg) {
        return -EINVAL;
    }
    
    // Check interrupt flags to see if a message is available
    uint8_t intf;
    int ret = mcp2515_read(MCP2515_CANINTF, &intf);
    if (ret) {
        return ret;
    }
    
    // Check if RXB0 has a message (bit 0)
    bool rxb0_full = intf & 0x01;
    // Check if RXB1 has a message (bit 1)
    bool rxb1_full = intf & 0x02;
    
    if (!rxb0_full && !rxb1_full) {
        return -EAGAIN; // No message available
    }
    
    // Read from RXB0 if available, otherwise RXB1
    uint8_t rx_base = rxb0_full ? MCP2515_RXB0SIDH : MCP2515_RXB1SIDH;
    uint8_t flag_bit = rxb0_full ? 0x01 : 0x02;
    
    // Read receive buffer data (13 bytes: SIDH, SIDL, EID8, EID0, DLC, DATA0-7)
    ret = mcp2515_read_multiple(rx_base, rx_data, 13);
    if (ret) {
        return ret;
    }
    
    // Extract Standard Identifier (11 bits)
    // SIDH contains bits 10-3, SIDL bits 7-5 contain bits 2-0
    msg->id = ((uint32_t)rx_data[0] << 3) | ((rx_data[1] >> 5) & 0x07);
    
    // Extract Data Length Code (lower 4 bits)
    msg->dlc = rx_data[4] & 0x0F;
    if (msg->dlc > 8) {
        msg->dlc = 8; // Clamp to maximum
    }
    
    // Copy data bytes
    for (uint8_t i = 0; i < msg->dlc; i++) {
        msg->data[i] = rx_data[5 + i];
    }
    
    // Clear the receive interrupt flag
    ret = mcp2515_bit_modify(MCP2515_CANINTF, flag_bit, 0x00);
    if (ret) {
        return ret;
    }
    
    return 0;
}

/** ***************************************************************************
 * @brief Helper function to set the CAN controller's operation mode
 * 
 * @param[in] mode CAN operating mode to set
 * @return int 0 on success, negative error code on failure
*******************************************************************************/
static int can_select_mode(enum can_mode mode) {

    if (mode < CAN_MODE_NORMAL || mode > CAN_MODE_CONFIG) {
        return -EINVAL;
    }
    
    uint8_t mode_bits = mode << CAN_MODE_OFFSET;
    int ret = mcp2515_bit_modify(MCP2515_CANCTRL, CAN_MODE_MASK, mode_bits);
    if (ret) {
        return ret;
    }
    
    // Verify mode change
    uint8_t current_mode;
    ret = mcp2515_read(MCP2515_CANSTAT, &current_mode);
    if (ret) {
        return ret;
    }

    if ((current_mode & CAN_MODE_MASK) != mode_bits) {
        return -EIO; // Mode change failed
    }
    
    return 0;
}

/** ***************************************************************************
 * @brief Helper function to set the CAN controller's timing (baud rate)
 * 
 * @param[in] baudrate Desired baud rate setting
 * @return int 0 on success, negative error code on failure
*******************************************************************************/
static int can_set_timing(uint8_t baudrate) {
    uint8_t mask = 0b00111111;
    return mcp2515_bit_modify(MCP2515_CNF1, mask, baudrate);
}
