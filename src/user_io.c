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
#include "gpio.h"
#include "adc.h"

uint8_t get_x_y_pos(void) {
    uint8_t x_val_analog = adc_read(X_CHANNEL);
    uint8_t y_val_analog = adc_read(Y_CHANNEL);

    

}