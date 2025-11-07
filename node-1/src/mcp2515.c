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

#include "debug.h"
#include "spi.h"

#define TX_BUF_SIZE 15

static uint8_t tx_buf[TX_BUF_SIZE] = {0};


/**< SPI device structure for the MCP2515 */
static const struct spi_device* mcp2515_dev;


int mcp2515_init(const struct spi_device* _mcp2515_dev) {
    mcp2515_dev = _mcp2515_dev;
    spi_device_init(mcp2515_dev);
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

    tx_buf[0] = MCP2515_READ;
    tx_buf[1] = address;

    int ret = spi_query(mcp2515_dev, tx_buf, 2, data, 1);
    if (ret) {
        return ret;
    }
    
    return 0;
}

int mcp2515_read_multiple(uint8_t address, uint8_t* data, uint8_t length) {
    if (!data || length == 0) {
        return -EINVAL;
    }

    tx_buf[0] = MCP2515_READ;
    tx_buf[1] = address;

    int ret = spi_query(mcp2515_dev, tx_buf, 2, data, length);
    if (ret) {
        return ret;
    }
    
    return 0;
}

int mcp2515_write(uint8_t address, uint8_t data) {
    tx_buf[0] = MCP2515_WRITE;
    tx_buf[1] = address;
    tx_buf[2] = data;
    return spi_master_transmit(mcp2515_dev, tx_buf, 3);
}

int mcp2515_write_multiple(uint8_t address, const uint8_t* data, uint8_t length) {
    if (!data || length == 0) {
        return -EINVAL;
    }
    
    // Allocate buffer for command (1 byte) + address (1 byte) + data
    uint8_t buffer_size = 2 + length;
    
    tx_buf[0] = MCP2515_WRITE;
    tx_buf[1] = address;
    
    // Copy data bytes
    for (uint8_t i = 0; i < length; i++) {
        tx_buf[2 + i] = data[i];
    }
    
    return spi_master_transmit(mcp2515_dev, tx_buf, buffer_size);
}

int mcp2515_request_to_send(bool txb0, bool txb1, bool txb2) {
    tx_buf[0] = MCP2515_RTS_BASE | (txb0 << 0) | (txb1 << 1) | (txb2 << 2);
    return spi_master_transmit(mcp2515_dev, tx_buf, 1);
}

int mcp2515_read_status(uint8_t* rx) {
    if (!rx) {
        return -EINVAL;
    }

    tx_buf[0] = MCP2515_READ_STATUS;
    return spi_query(mcp2515_dev, tx_buf, 1, rx, 2);
}

int mcp2515_bit_modify(uint8_t address, uint8_t mask, uint8_t data) {
    tx_buf[0] = MCP2515_BIT_MODIFY;
    tx_buf[1] = address;
    tx_buf[2] = mask;
    tx_buf[3] = data;
    return spi_master_transmit(mcp2515_dev, tx_buf, 4);
}

int mcp2515_reset(void) {
    tx_buf[0] = MCP2515_RESET;
    return spi_master_transmit(mcp2515_dev, tx_buf, 1);
}

int mcp2515_print_config(void) {
    int ret;
    volatile uint8_t reg_val;
    ret = mcp2515_read(MCP2515_CNF1, &reg_val);
    if(ret) {
        return ret;
    }
    DEBUG_PRINTF("CNF1: %2X \r\n", reg_val);
    ret = mcp2515_read(MCP2515_CNF2, &reg_val);
    if(ret) {
        return ret;
    }
    DEBUG_PRINTF("CNF2: %2X \r\n", reg_val);
    ret = mcp2515_read(MCP2515_CNF3, &reg_val);
    if(ret) {
        return ret;
    }
    DEBUG_PRINTF("CNF3: %2X \r\n", reg_val);

    return 0;
}