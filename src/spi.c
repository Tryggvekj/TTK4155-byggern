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

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>
#include <errno.h>

#include "user_io.h"
#include "xmem.h"
#include "gpio.h"
#include "adc.h"
#include "spi.h"


/** ***************************************************************************
 * @brief Initializes the SPI bus
 * 
*******************************************************************************/
void spi_master_init(void)
{
	// Set MOSI and SCK as output, all others as input
	DDRB |= ((1 << DDB5) | (1 << DDB7) | (1 << DDB4));

	// Enable SPI, set as Master, set clock rate fck/16
	SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
    
    gpio_init('D', 2, OUTPUT);
    gpio_init('D', 3, OUTPUT);

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
    // Set the wanted combination of PD2, PD3 and PD4 low
    if (device == 0) {
        gpio_set('D', 2, LOW);
        gpio_set('D', 3, HIGH);
    }
    else if (device == 1) {
        gpio_set('D', 2, HIGH);
        gpio_set('D', 3, LOW);
    }
    else if (device == 2) {
        gpio_set('D', 2, HIGH);
        gpio_set('D', 3, HIGH);
    }

    return 0;

}

/** ***************************************************************************
 * @brief Deselects all SPI slaves
 * 
*******************************************************************************/
void spi_end_transmit()
{
    // Set PD2, PD3 and PD4 high
    gpio_set('D', 2, HIGH);
    gpio_set('D', 3, HIGH);

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