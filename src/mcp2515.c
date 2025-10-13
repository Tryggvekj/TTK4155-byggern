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
#define F_CPU 4915200 // Hz
#include <util/delay.h>
#include <avr/io.h>

#include "spi.h"


/**< SPI device pointer for the MCP2515 */
static const struct spi_device* mcp2515_dev = NULL;


int mcp2515_init(const struct spi_device* _mcp2515_dev) {
    mcp2515_dev = _mcp2515_dev;
    spi_device_init(mcp2515_dev);
    bool ok = mcp2515_reset();
    if (!ok) {
        return -EIO;
    }
    return 0;
}

uint8_t mcp2515_read(uint8_t address) {
    uint8_t tx[2] = {MCP2515_READ, address};
    uint8_t rx[1] = {0};
    // Assume device 2 is the MCP2515
    bool ok = spi_query(mcp2515_dev, tx, 2, rx, 1);
    if (!ok) {
        // Return 0xFF on error (MCP2515 default reset value for most registers)
        return 0xFF;
    }
    return rx[0];
}

bool mcp2515_write(uint8_t address, uint8_t data) {
    uint8_t tx[3] = {MCP2515_WRITE, address, data};
    bool ok = spi_master_transmit(mcp2515_dev, tx, 3) == 0;
    return ok;
}

bool mcp2515_request_to_send(bool txb0, bool txb1, bool txb2) {
    uint8_t tx = MCP2515_RTS_BASE | (txb0 << 0) | (txb1 << 1) | (txb2 << 2);
    bool ok = spi_master_transmit(mcp2515_dev, &tx, 1);
    return ok;
}

bool mcp2515_read_status(uint8_t* rx) {
    uint8_t tx[1] = {MCP2515_READ_STATUS};
    bool ok = spi_query(mcp2515_dev, tx, 1, rx, 2);
    return ok;
}

bool mcp2515_bit_modify(uint8_t address, uint8_t mask, uint8_t data) {
    uint8_t tx[4] = {MCP2515_BIT_MODIFY, address, mask, data};
    bool ok = spi_master_transmit(mcp2515_dev, tx, 4);
    return true;
}

bool mcp2515_reset(void) {
    uint8_t tx[1] = {MCP2515_RESET};
    bool ok = spi_master_transmit(mcp2515_dev, tx, 1);
    return ok;
}