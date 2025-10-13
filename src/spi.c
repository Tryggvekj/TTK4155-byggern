/** ***************************************************************************
 * @file spi.c
 * @author Byggve Klevstul-Jensen, Magnus Carlsen Haaland
 * @brief SPI driver to communicate with the IO-board
 * @version 0.1
 * @date 2025-09-25
 * 
 * @copyright Copyright (c) 2025
 * 
*******************************************************************************/



#include <errno.h>
#include <stdint.h>
#include <stdio.h>

#define F_CPU 4915200 // Hz
#include <util/delay.h>

#include <avr/io.h>

#include "gpio.h"
#include "spi.h"

// Local variables for SPI pins
static struct gpio_pin mosi_pin;
static struct gpio_pin miso_pin;
static struct gpio_pin sck_pin;






/** ***************************************************************************
 * @brief Initializes the SPI bus
 * 
 * @param[in] _mosi_pin GPIO pin for MOSI
 * @param[in] _miso_pin GPIO pin for MISO
 * @param[in] _sck_pin GPIO pin for SCK
 * 
 * @details Configures SPI pins for input/output and sets up SPI control registers
*******************************************************************************/
void spi_master_init(struct gpio_pin _mosi_pin, struct gpio_pin _miso_pin, struct gpio_pin _sck_pin)
{
	// Set MOSI and SCK as output, MISO as input
    mosi_pin = _mosi_pin;
    miso_pin = _miso_pin;
    sck_pin = _sck_pin;

    gpio_init(mosi_pin, OUTPUT);
    gpio_init(miso_pin, INPUT);
    gpio_init(sck_pin, OUTPUT);

	// Enable SPI, set as Master, set clock rate fck/4
	SPCR = (1 << SPE) | (1 << MSTR);

    SPSR |= (1 << SPI2X); // Set double speed
}

/** ***************************************************************************
 * @brief Initializes an SPI device
 * 
 * @param[in] device Pointer to the SPI device structure
 * @return int 0 on success, negative error code on failure
 * @details Sets the CS pin as output and deselects the device
 ******************************************************************************/
int spi_device_init(const struct spi_device* device) {
    if (!device) {
        return -ENXIO;
    }

    gpio_init(device->cs_pin, OUTPUT);
    gpio_set(device->cs_pin, HIGH); // Deselect device

    return 0;
}

/** ***************************************************************************
 * @brief Selects a slave device on the SPI bus for transmission
 * 
 * @param[in] device Pointer to the SPI device structure
 * @return 0 on success, negative error code on failure
*******************************************************************************/
int spi_select_device(const struct spi_device* device)
{
    if (!device) {
        return -ENXIO;
    }

    gpio_set(device->cs_pin, LOW);
    return 0;
}

/** ***************************************************************************
 * @brief Deselects a specific SPI slave device
 * 
 * @param[in] device Pointer to the SPI device structure
*******************************************************************************/
int spi_deselect_device(const struct spi_device* device)
{
    if (!device) {
        return -ENXIO;
    }

    gpio_set(device->cs_pin, HIGH);
    return 0;
}

/** ***************************************************************************
 * @brief Transmits a single data byte to an SPI device
 * 
 * @param[in] device Pointer to the SPI device structure
 * @param[in] data Data byte to be transmitted
 * @return 0 on success, negative error code on failure
*******************************************************************************/
int spi_master_transmit_single(const struct spi_device* device, uint8_t data) {
    int ret = spi_select_device(device);
    if(ret) {
        return ret;
    }
	
    // Start transmission
	SPDR = data;

	// Wait for transmission complete
	while (!(SPSR & (1 << SPIF))) {
        ;
	}

    spi_deselect_device(device);
    
    return 0;
}

/** ***************************************************************************
 * @brief Transmits multiple data bytes to an SPI device
 * 
 * @param[in] device Pointer to the SPI device structure
 * @param[in] data Array of data bytes to be transmitted
 * @param[in] size Number of bytes to transmit
 * @return 0 on success, negative error code on failure
*******************************************************************************/
int spi_master_transmit(const struct spi_device* device, uint8_t* data, uint8_t size) {

    int ret = spi_select_device(device);
    if(ret) {
        return ret;
    }

    for (uint8_t i = 0; i < size; i++) {

        SPDR = data [i];
        
        // Wait for transmission complete
	    while (!(SPSR & (1 << SPIF))) {
            ;
	    }   
    }

    spi_deselect_device(device);

    return 0;
}

/** ***************************************************************************
 * @brief Receives multiple data bytes from an SPI device
 * 
 * @param[in] device Pointer to the SPI device structure
 * @param[out] buffer Buffer to store received data bytes
 * @param[in] size Number of bytes to receive
 * @return bool True on success, false on failure
*******************************************************************************/
bool spi_receive(const struct spi_device* device, uint8_t* buffer, uint8_t size) {
    if (!device) {
        return false;
    }

    int ret = spi_select_device(device);
    if(ret) {
        return false;
    }

    for (uint8_t i = 0; i < size; i++) {

        SPDR = 0x00; // Send dummy byte to generate clock

        // Wait for reception complete
        while (!(SPSR & (1 << SPIF))) {
            ;
        }

        buffer[i] = SPDR;
        _delay_us(100);
    }
    
    spi_deselect_device(device);
    return true;
}

/** ***************************************************************************
 * @brief Performs a query operation (transmit then receive) on an SPI device
 * 
 * @param[in] device Pointer to the SPI device structure
 * @param[in] tx_data Array of data bytes to transmit
 * @param[in] tx_size Number of bytes to transmit
 * @param[out] rx_data Buffer to store received data bytes
 * @param[in] rx_size Number of bytes to receive
 * @return bool True on success, false on failure
*******************************************************************************/
bool spi_query(const struct spi_device* device, uint8_t* tx_data, uint8_t tx_size, uint8_t* rx_data, uint8_t rx_size) {
    int ret = spi_select_device(device);
    if(ret) {
        return false;
    }
    
    // Transmit data without managing CS (already selected)
    for (uint8_t i = 0; i < tx_size; i++) {
        SPDR = tx_data[i];
        while (!(SPSR & (1 << SPIF))) {
            ;
        }   
    }
    
    // Receive data without managing CS (already selected)
    for (uint8_t i = 0; i < rx_size; i++) {
        SPDR = 0x00; // Send dummy byte to generate clock
        while (!(SPSR & (1 << SPIF))) {
            ;
        }
        rx_data[i] = SPDR;
        _delay_us(100);
    }
    
    spi_deselect_device(device);
    return true;
}