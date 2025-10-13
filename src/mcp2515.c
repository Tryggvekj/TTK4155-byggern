/** ***************************************************************************
 * @file mcp2515.c
 * @author Byggarane
 * @brief MCP2515 CAN controller driver implementation file
 * @version 0.1
 * @date 2025-10-09
 * 
 * @copyright Copyright (c) 2025 Byggarane
 * 
*******************************************************************************/


#include "mcp2515.h"

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <avr/io.h>

#include "spi.h"


/**< SPI device structure for the MCP2515 */
static struct spi_device mcp2515_dev;


int mcp2515_init(const struct spi_device _mcp2515_dev) {
    mcp2515_dev = _mcp2515_dev;
    spi_device_init(&mcp2515_dev);
    int ret = mcp2515_reset();
    if (ret) {
        return ret;
    }
    return 0;
}

int mcp2515_read(uint8_t address, uint8_t* data) {
    if (!data) {
        return -EINVAL;
    }
    
    uint8_t tx[2] = {MCP2515_READ, address};
    uint8_t rx[1] = {0};
    
    int ret = spi_query(&mcp2515_dev, tx, 2, rx, 1);
    if (ret) {
        return ret;
    }
    
    *data = rx[0];
    return 0;
}

int mcp2515_write(uint8_t address, uint8_t data) {
    uint8_t tx[3] = {MCP2515_WRITE, address, data};
    return spi_master_transmit(&mcp2515_dev, tx, 3);
}

int mcp2515_request_to_send(bool txb0, bool txb1, bool txb2) {
    uint8_t tx = MCP2515_RTS_BASE | (txb0 << 0) | (txb1 << 1) | (txb2 << 2);
    return spi_master_transmit(&mcp2515_dev, &tx, 1);
}

int mcp2515_read_status(uint8_t* rx) {
    if (!rx) {
        return -EINVAL;
    }
    
    uint8_t tx[1] = {MCP2515_READ_STATUS};
    return spi_query(&mcp2515_dev, tx, 1, rx, 2);
}

int mcp2515_bit_modify(uint8_t address, uint8_t mask, uint8_t data) {
    uint8_t tx[4] = {MCP2515_BIT_MODIFY, address, mask, data};
    return spi_master_transmit(&mcp2515_dev, tx, 4);
}

int mcp2515_reset(void) {
    uint8_t tx[1] = {MCP2515_RESET};
    return spi_master_transmit(&mcp2515_dev, tx, 1);
}