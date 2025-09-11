/** ***************************************************************************
 * @file user_io.c
 * @author Magnuszszew Carlsewitz Haalski
 * @brief User I/O functions implementation file
 * @version 0.1
 * @date 2025-09-11
 * 
 * @copyright Copyright (c) 2025 Byggarane
 * 
*******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>

#include "user_io.h"
#include "xmem.h"


/** ***************************************************************************
 * @brief Read a value from the ADC via external memory
 * 
 * @return uint8_t The ADC value read from external memory
*******************************************************************************/
uint8_t adc_read(uint8_t channel) {
    DDR
    CLKPR = (1 << CLKPCE); // Enable change of the clock prescaler
    CLKPR = 0x0; // Set clock prescaler to 1 (no division)
    uint8_t ADC_CONFIG = 0x00; // Configure ADC for the specified channel (0-3)
    xmem_write(ADC_CONFIG, ADC_ADDR); // Write ADC configuration to ADC address
    uint8_t adc_value = xmem_read(ADC_ADDR); // Read ADC value from ADC
    return adc_value;
}