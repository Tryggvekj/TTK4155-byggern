/** ***************************************************************************
 * @file user_io.h
 * @author Magnuszszew Carlsewitz Haalski
 * @brief User I/O functions for interacting with the user
 * @version 0.1
 * @date 2025-09-11
 * 
 * @copyright Copyright (c) 2025 Byggarane
 * 
*******************************************************************************/


#pragma once

#define ADC_ADDR 0x1000

#define ADC_BIT_RES 8
#define ADC_MAX_VAL ((1 << ADC_BIT_RES) - 1)
#define ADC_MIN_VAL 0

uint8_t adc_read(uint8_t channel);