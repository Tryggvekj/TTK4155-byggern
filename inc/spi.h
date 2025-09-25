/** ***************************************************************************
 * @file spi.h
 * @author Byggve Klevstul-Jensen
 * @brief SPI driver to communicate witht the IO-board
 * @version 0.1
 * @date 2025-09-25
 * 
 * @copyright Copyright (c) 2025 Byggarane
 * 
*******************************************************************************/

#pragma once


void SPI_MasterInit(void);

void SPI_MasterTransmitSingle(char cData);

void SPI_StartTransmit(uint8_t device);

void SPI_EndTransmit();

void SPI_MasterTransmit(unsigned char* data, uint8_t device);