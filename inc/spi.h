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

#include "gpio.h"

#define NUM_DEVICES 3
#define timeout_loops 100
#define timeout_delay_us 100


/** ***************************************************************************
 * @brief Structure representing an SPI device
 ******************************************************************************/
struct spi_device {
    uint8_t id;               /**< Unique ID for the device */
    struct gpio_pin cs_pin;   /**< Chip select pin for the device */
};


/** ***************************************************************************
 * @brief Initializes the SPI bus
 * 
*******************************************************************************/
void spi_master_init(struct gpio_pin _mosi_pin, struct gpio_pin _miso_pin, struct gpio_pin _sck_pin);

/** ***************************************************************************
 * @brief Initializes an SPI device by configuring its chip select pin
 * 
 * @param[in] device Pointer to the SPI device structure
 * @return 0 on success, negative error code on failure
*******************************************************************************/
int spi_device_init(const struct spi_device* device);

/** ***************************************************************************
 * @brief Transmits a single data byte to an SPI device
 * 
 * @param[in] device Pointer to the SPI device structure
 * @param[in] data Data byte to be transmitted
 * @return 0 on success, negative error code on failure
*******************************************************************************/
int spi_master_transmit_single(const struct spi_device* device, uint8_t data);

/** ***************************************************************************
 * @brief Transmits multiple data bytes to an SPI device
 * 
 * @param[in] device Pointer to the SPI device structure
 * @param[in] data Array of data bytes to be transmitted
 * @param[in] size Number of bytes to transmit
 * @return 0 on success, negative error code on failure
*******************************************************************************/
int spi_master_transmit(const struct spi_device* device, uint8_t* data, uint8_t size);

/** ***************************************************************************
 * @brief Receives multiple data bytes from an SPI device
 * 
 * @param[in] device Pointer to the SPI device structure
 * @param[out] buffer Buffer to store received data bytes
 * @param[in] size Number of bytes to receive
 * @return int 0 on success, negative error code on failure
*******************************************************************************/
int spi_receive(const struct spi_device* device, uint8_t* buffer, uint8_t size);

/** ***************************************************************************
 * @brief Performs a query operation (transmit then receive) on an SPI device
 * 
 * @param[in] device Pointer to the SPI device structure
 * @param[in] tx_data Array of data bytes to transmit
 * @param[in] tx_size Number of bytes to transmit
 * @param[out] rx_data Buffer to store received data bytes
 * @param[in] rx_size Number of bytes to receive
 * @return int 0 on success, negative error code on failure
*******************************************************************************/
int spi_query(const struct spi_device* device, uint8_t* tx_data, uint8_t tx_size, uint8_t* rx_data, uint8_t rx_size);