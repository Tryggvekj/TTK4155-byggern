/** ***************************************************************************
 * @file spi.h
 * @author Byggve Klevstul-Jensen
 * @brief spi driver to communicate witht the IO-board
 * @version 0.1
 * @date 2025-09-25
 * 
 * @copyright Copyright (c) 2025 Byggarane
 * 
*******************************************************************************/

#pragma once

#define NUM_DEVICES 2


void spi_master_init(void);

int spi_master_transmit_single(uint8_t data, uint8_t device);

int spi_master_transmit(uint8_t* data, uint8_t size, uint8_t device);

int spi_StartTransmit(uint8_t device);

void spi_EndTransmit();