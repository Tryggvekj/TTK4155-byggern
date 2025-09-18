/** ***************************************************************************
 * @file xmem.c
 * @author Magnuszsew Carlsewitz Haalski
 * @brief External memory driver implementation file
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

#include "xmem.h"


/** ***************************************************************************
 * @brief Initialize the external memory interface
 * 
*******************************************************************************/
void xmem_init(void) {
	// Set up the external memory interface
	MCUCR |= (1 << SRE); // Enable external memory
	SFIOR |= (1 << XMM0); // Use full address space for external memory
}

/** ***************************************************************************
 * @brief Write a data byte to the specified external memory address
 * 
 * @param[in] data The data byte to write
 * @param[in] addr The external memory address to write to
*******************************************************************************/
void xmem_write(uint8_t data, uint16_t addr) {

	volatile uint8_t* ext_ram = (uint8_t*) SRAM_BASE_ADDR;
	ext_ram[addr] = data;
}

/** ***************************************************************************
 * @brief Read a data byte from the specified external memory address
 * 
 * @param[in] addr The external memory address to read from
 * @return uint8_t The data byte read from the specified address
*******************************************************************************/
uint8_t xmem_read(uint16_t addr) {

	volatile uint8_t *ext_mem = (uint8_t *) SRAM_BASE_ADDR;
	uint8_t data = ext_mem[addr];
	return data;
}

/** ***************************************************************************
 * @brief Test the external SRAM
 * 
 * @details Writes a series of pseudo-random values and reads them back to verify
 *          SRAM functionality.
*******************************************************************************/
void SRAM_test(void)
{

	uint16_t write_errors = 0;
	uint16_t retrieval_errors = 0;
	printf("Starting SRAM test...\r\n");

	// rand() stores some internal state, so calling this function in a loop will
	// yield different seeds each time (unless srand() is called before this function)
	uint16_t seed = rand();

	// Write phase: Immediately check that the correct value was stored
	srand(seed);
	for (uint16_t i = 0; i < SRAM_SIZE; i++) {
		uint8_t some_value = rand();
		xmem_write(some_value, i);
		uint8_t retrieved_value = xmem_read(i);
		if (retrieved_value != some_value) {
			printf("Write phase error: address 0x%03X contains 0x%02X (should be 0x%02X)\r\n", i, retrieved_value, some_value);
			write_errors++;
		}
	}

	// Retrieval phase: Check that no values were changed during or after the write phase
	srand(seed); // reset the PRNG to the state it had before the write phase
	for (uint16_t i = 0; i < SRAM_SIZE; i++) {
		uint8_t some_value = rand();
		uint8_t retrieved_value = xmem_read(i);
		if (retrieved_value != some_value) {
			printf("Retrieval phase error: address 0x%03X contains 0x%02X (should be 0x%02X)\r\n", i, retrieved_value, some_value);
			retrieval_errors++;
		}
	}

	printf("SRAM test completed with\r\n%4d errors in write phase and\r\n%4d errors in retrieval phase\r\n\r\n", write_errors, retrieval_errors);
}