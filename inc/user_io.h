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

#define X_CHANNEL 0
#define Y_CHANNEL 1
#define V_MAX 4
#define V_MIN 1
#define ADC_OUTP_MAX 149
#define ADC_OUTP_MIN 49

/** ***************************************************************************
 * @brief Get the x y pos object
 * 
 * @return uint16_t 
*******************************************************************************/
uint8_t get_x_y_pos(void);