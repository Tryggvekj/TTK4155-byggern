/** ***************************************************************************
 * @file MCP2515.c
 * @author Byggarane
 * @brief MCP2515 CAN controller driver implementation file
 * @version 0.1
 * @date 2025-10-09
 * 
 * @copyright Copyright (c) 2025 Byggarane
 * 
*******************************************************************************/


#include "MCP2515.h"

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/io.h>

#include "spi.h"

bool MCP2515_init(void) {
    return true;
}

uint8_t MCP2515_read(uint8_t address) {
    uint8_t tx[2] = {MCP2515_READ_INSTRUCTION, address};
    uint8_t rx[1] = {0};
    // Assume device 2 is the MCP2515
    bool ok = spi_query(tx, 2, rx, 1, 2);
    if (!ok) {
        // Return 0xFF on error (MCP2515 default reset value for most registers)
        return 0xFF;
    }
    return rx[0];
}

bool MCP2515_write(uint8_t address, uint8_t data) {
    uint8_t tx[3] = {MCP2515_WRITE_INSTRUCTION, address, data};
    // Assume device 2 is the MCP2515
    bool ok = spi_master_transmit(tx, 3, 2, true) == 0;
    return ok;
}

bool MCP2515_request_to_send(void) {
    return true;
}

bool MCP2515_read_status(void) {
    return true;
}

bool MCP2515_bit_modify(void) {
    return true;
}

bool MCP2515_reset(void) {
    return true;
}