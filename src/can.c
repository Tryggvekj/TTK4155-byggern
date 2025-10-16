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


/** ***************************************************************************
 * @brief Initialize the CAN controller (MCP2515) and set operation mode
 * 
 * @param[in] _mcp2515_dev SPI device structure for the MCP2515
 * @param[in] mode CAN operating mode to set
 * @return int 0 on success, negative error code on failure
*******************************************************************************/
int can_init(const struct spi_device _mcp2515_dev, enum can_mode mode) {
    int ret = mcp2515_init(_mcp2515_dev);
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
    int ret = 0;
    //TODO: Implement CAN message transmission
    return ret;
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
    int ret = 0;
    // TODO: Implement CAN message reception    
    return ret;
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