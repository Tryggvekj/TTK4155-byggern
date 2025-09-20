/** ***************************************************************************
 * @file adc.c
 * @author Magnuszew Carlsewitz Haalski
 * @brief ADC driver implementation file
 * @version 0.1
 * @date 2025-09-18
 * 
 * @copyright Copyright (c) 2025 Byggarane
 * 
 * @note Driver for the MAX156 8-bit 4 channel ADC
 *       https://www.analog.com/media/en/technical-documentation/data-sheets/max155-max156.pdf
 * 
*******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>
#define F_CPU 4915200 // Hz
#include <util/delay.h>

#include "adc.h"
#include "xmem.h"
#include "gpio.h"


/** ***************************************************************************
 * @brief Enable a CLK signal on PD5 for the ADC
 * 
 * @note The signal is toggled every clock cycle, resulting in f = F_CPU/2
*******************************************************************************/
void adc_clk_enable(void) {

    // Enable output
    gpio_init('D', 5, OUTPUT);
    
    // Set compare output mode to toggle
    TCCR1A = (1<<COM1A0);

    // Set clock prescaler to 1 and waveform generation mode to CTC
    TCCR1B = (1<<CS10) | (1<<WGM12);

    // Set compare values
    OCR1A = 0x0000;

}

/** ***************************************************************************
 * @brief Write data to the ADC
 * 
 * @param data Data byte to write
*******************************************************************************/
void adc_write(uint8_t data) {

    volatile uint8_t* adc_addr = (uint8_t*) ADC_BASE_ADDR;
	*adc_addr = data;
}

/** ***************************************************************************
 * @brief Read data from the ADC
 * 
 * @param[in] channel ADC channel to read from (0-7)
 * @return uint8_t The read ADC value
*******************************************************************************/
uint8_t adc_read(uint8_t channel) {

    // Set bits according to Table 1 in datasheet
    uint8_t config = channel | (1 << _ALL_BIT);
    adc_write(channel);
    _delay_us(20); // Wait for channel to settle

    // Read data
    volatile uint8_t* adc_addr = (uint8_t*) ADC_BASE_ADDR;
    uint8_t data = *adc_addr;

    return data;
}