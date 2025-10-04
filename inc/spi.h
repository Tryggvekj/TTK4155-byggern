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


/** ***************************************************************************
 * @brief Initializes the SPI bus
 * 
*******************************************************************************/
void spi_master_init(void);

/** ***************************************************************************
 * @brief Transmits a single data byte to an SPI device
 * 
 * @param[in] data Data byte to be transmitted
 * @param[in] device SPI slave device ID
 * @return 0 on success, negative error code on failure
*******************************************************************************/
int spi_master_transmit_single(uint8_t data, uint8_t device);

/** ***************************************************************************
 * @brief Transmits multiple data bytes to an SPI device
 * 
 * @param[in] data Array of data bytes to be transmitted
 * @param[in] size Number of bytes to transmit
 * @param[in] device SPI slave device ID
 * @return 0 on success, negative error code on failure
*******************************************************************************/
int spi_master_transmit(uint8_t* data, uint8_t size, uint8_t device);

/** ***************************************************************************
 * @brief Selects a slave device on the SPI bus for transmission
 * 
 * @param[in] device ID of the slave device
 * @return 0 on success, negative error code on failure
*******************************************************************************/
int spi_start_transmit(uint8_t device);

/** ***************************************************************************
 * @brief Deselects all SPI slaves
 * 
*******************************************************************************/
void spi_end_transmit();