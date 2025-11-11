/** ***************************************************************************
 * @file adc.h
 * @author Magnus Carlsen Haaland, Tryggve Klevstul-Jensen, Walter Brynildsen
 * @brief ADC driver
 * @version 0.1
 * @date 2025-11-06
 * 
 * @copyright Copyright (c) 2025 Byggarane
 * 
 *****************************************************************************/

#pragma once
#include <stdint.h>

int adc_init(void);

int adc_read(uint16_t* result);