/** ***************************************************************************
 * @file spi.c
 * @author Byggve Klevstul-Jensen
 * @brief SPI driver to communicate witht the IO-board
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

#include "user_io.h"
#include "xmem.h"
#include "gpio.h"
#include "adc.h"
#include "spi.h"

void SPI_MasterInit(void)
{
	// Set MOSI and SCK as output, all others as input
	DDRB |= ((1 << DDB5) | (1 << DDB7) | (1 << DDB4));

	// Enable SPI, set as Master, set clock rate fck/16
	SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
    
    gpio_init('D', 2, OUTPUT);
    gpio_init('D', 3, OUTPUT);
    gpio_init('D', 4, OUTPUT);

    SPI_EndTransmit();
}

void SPI_MasterTransmitSingle(char cData)
{
	// Start transmission
	SPDR = cData;

	// Wait for transmission complete
	while (!(SPSR & (1 << SPIF))) {
		;// Wait
	}
}

void SPI_StartTransmit(uint8_t device)
{

    // Ignore if device chosen is above allowed number
    if (device > 2) {
        return;
    }
    // Set the wanted combination of PD2, PD3 and PD4 low
    if (device == 0) {
        gpio_set('D', 2, false);
        gpio_set('D', 3, true);
        gpio_set('D', 4, true);
    }
    else if (device == 1) {
        gpio_set('D', 2, true);
        gpio_set('D', 3, false);
        gpio_set('D', 4, true);
    }
    else if (device == 2) {
        gpio_set('D', 2, true);
        gpio_set('D', 3, true);
        gpio_set('D', 4, false);
    }

}

void SPI_EndTransmit()
{
    // Set PD2, PD3 and PD4 high
    gpio_set('D', 2, true);
    gpio_set('D', 3, true);
    gpio_set('D', 4, true);

}

// DONT USE
void SPI_MasterTransmit(unsigned char* data, uint8_t device) 
{
    SPI_StartTransmit(device);
	if (data == NULL) {
		return;
	}
	while (*data != '\0') {
		SPI_MasterTransmitSingle(*data);
		data++;
	}
    SPI_EndTransmit();
}
