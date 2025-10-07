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

#include <avr/io.h>

#include "gpio.h"
#include "spi.h"

// Local variables for SPI pins
static struct gpio_pin mosi_pin;
static struct gpio_pin miso_pin;
static struct gpio_pin sck_pin;

// Hardcoded chip select pins for now
static struct gpio_pin cs_pins[NUM_DEVICES] = { {'D', 2}, {'D', 3} };


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
	// Set MOSI and SCK as output, all others as input
    mosi_pin = _mosi_pin;
    miso_pin = _miso_pin;
    sck_pin = _sck_pin;

    gpio_init(mosi_pin, OUTPUT);
    gpio_init(miso_pin, INPUT);
    gpio_init(sck_pin, OUTPUT);

    gpio_init(cs_pins[0], OUTPUT);
    gpio_init(cs_pins[1], OUTPUT);

	// Enable SPI, set as Master, set clock rate fck/4
	SPCR = (1 << SPE) | (1 << MSTR);

    SPSR |= (1 << SPI2X); // Set double speed

    spi_end_transmit();
}

/** ***************************************************************************
 * @brief Selects a slave device on the SPI bus for transmission
 * 
 * @param[in] device ID of the slave device
 * @return 0 on success, negative error code on failure
*******************************************************************************/
int spi_start_transmit(uint8_t device)
{
    // Ignore if device chosen is above allowed number
    if (device > NUM_DEVICES) {
        return -ENXIO;
    }

    // Set the selected device's CS pin low, others high
    switch (device) {
        case 0:
            gpio_set(cs_pins[0], LOW);
            gpio_set(cs_pins[1], HIGH);
            break;
        case 1:
            gpio_set(cs_pins[0], HIGH);
            gpio_set(cs_pins[1], LOW);
            break;
        default:
            return -ENXIO;
    }

    return 0;

}

/** ***************************************************************************
 * @brief Deselects all SPI slaves
 * 
 * @details Sets all CS pins high
*******************************************************************************/
void spi_end_transmit()
{
    // Set all CS pins high
    gpio_set(cs_pins[0], HIGH);
    gpio_set(cs_pins[1], HIGH);

}

/** ***************************************************************************
 * @brief Transmits a single data byte to an SPI device
 * 
 * @param[in] data Data byte to be transmitted
 * @param[in] device SPI slave device ID
 * @return 0 on success, negative error code on failure
*******************************************************************************/
int spi_master_transmit_single(uint8_t data, uint8_t device) {
    int ret = spi_start_transmit(device);
    if(ret) {
        return ret;
    }
	
    // Start transmission
	SPDR = data;

	// Wait for transmission complete
	while (!(SPSR & (1 << SPIF))) {
		;// Wait
	}

    spi_end_transmit();
    
    return 0;
}

/** ***************************************************************************
 * @brief Transmits multiple data bytes to an SPI device
 * Set all CS pins high
 * @param[in] data Array of data bytes to be transmitted
 * @param[in] size Number of bytes to transmit
 * @param[in] device SPI slave device ID
 * @return 0 on success, negative error code on failure
*******************************************************************************/
int spi_master_transmit(uint8_t* data, uint8_t size, uint8_t device) {

    int ret = spi_start_transmit(device);
    if(ret) {
        return ret;
    }

    for (uint8_t i = 0; i < size; i++) {

        SPDR = data [i];
        // Wait for transmission complete
	    while (!(SPSR & (1 << SPIF))) {
	    	;// Wait
	    }   
    }

    spi_end_transmit();

    return 0;
}

bool spi_receive(uint8_t* buffer, uint8_t size, uint8_t device) {
    int ret = spi_start_transmit(device);
    if(ret) {
        return false;
    }

    for (uint8_t i = 0; i < size; i++) {

        // Wait for reception complete
        while (!(SPSR & (1 << SPIF))) {
            ; // Wait
        }

        buffer[i] = SPDR;
    }

    spi_end_transmit();
    return true;
}

bool spi_query(uint8_t* tx_data, uint8_t tx_size, uint8_t* rx_data, uint8_t rx_size, uint8_t device) {
    spi_master_transmit(tx_data, tx_size, device);
    bool response = spi_receive(rx_data, rx_size, device);
    return response;
}