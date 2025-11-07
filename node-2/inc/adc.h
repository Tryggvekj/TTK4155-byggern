#pragma once
#include <stdint.h>

int adc_init(void);

int adc_read(uint16_t* result);