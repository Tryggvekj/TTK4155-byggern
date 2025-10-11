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
#include <util/delay.h>
#include <avr/io.h>

#include "spi.h"


/**< SPI device structure for the MCP2515 */
struct spi_device mcp2515_dev;


int mcp2515_init(struct spi_device _mcp2515_dev) {
    mcp2515_dev = _mcp2515_dev;
    return spi_device_init(&mcp2515_dev);
}

uint8_t mcp2515_read(uint8_t address) {
    uint8_t tx[2] = {MCP2515_READ_INSTRUCTION, address};
    uint8_t rx[1] = {0};
    // Assume device 2 is the MCP2515
    bool ok = spi_query(&mcp2515_dev, tx, 2, rx, 1);
    if (!ok) {
        // Return 0xFF on error (MCP2515 default reset value for most registers)
        return 0xFF;
    }
    return rx[0];
}

bool mcp2515_write(uint8_t address, uint8_t data) {
    uint8_t tx[3] = {MCP2515_WRITE_INSTRUCTION, address, data};
    bool ok = spi_master_transmit(&mcp2515_dev, tx, 3) == 0;
    return ok;
}

bool mcp2515_request_to_send(void) {
    return true;
}

bool mcp2515_read_status(void) {
    return true;
}

bool mcp2515_bit_modify(void) {
    return true;
}

bool mcp2515_reset(void) {
    return true;
}