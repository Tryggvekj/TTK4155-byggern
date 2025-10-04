/** ***************************************************************************
 * @file adc.h
 * @author Magnuszsew Carlsewitz Haalski
 * @brief Header file for ADC driver
 * @version 0.1
 * @date 2025-09-18
 * 
 * @copyright Copyright (c) 2025 Byggarane
 * 
*******************************************************************************/

#pragma once

#define ADC_BASE_ADDR 0x1000
#define ADC_SIZE 0x800

#define ADC_BIT_RES 8
#define ADC_MAX_VAL ((1 << ADC_BIT_RES) - 1)
#define ADC_MIN_VAL 0

#define _ALL_BIT 7


/** ***************************************************************************
 * @brief Enable a CLK signal on PD5 for the ADC
 * 
 * @note The signal is toggled every clock cycle, resulting in f = F_CPU/2
*******************************************************************************/
void adc_clk_enable(void);

/** ***************************************************************************
 * @brief Write data to the ADC
 * 
 * @param[in] data Data byte to write
*******************************************************************************/
void adc_write(uint8_t data);

/** ***************************************************************************
 * @brief Read data from the ADC
 * 
 * @param[in] channel ADC channel to read from (0-7)
 * @return uint8_t The read ADC value
*******************************************************************************/
uint8_t adc_read(uint8_t channel);