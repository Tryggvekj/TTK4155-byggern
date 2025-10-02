/** ***************************************************************************
 * @file uart.h
 * @author Magnuszszew Carlsewitz Haalski
 * @brief UART driver header file
 * @version 0.1
 * @date 2025-08-05
 * 
 * @copyright Copyright (c) 2025 Byggarane
 * 
 ******************************************************************************/

#pragma once

#include <stdio.h>
#include <avr/io.h>



/** ***************************************************************************
 * @brief Initialize the UART with the given UBRR value
 * 
 * @param[in] ubrr The value written to the UBBR register to set the baud rate
 ******************************************************************************/
void uart_init(uint16_t ubrr);

/** ***************************************************************************
 * @brief Transmit a single byte of data via UART
 * 
 * @param[in] data The byte to be transmitted
 ******************************************************************************/
void uart_transmit(uint8_t data);

/** ***************************************************************************
 * @brief Receive a single byte of data via UART
 * 
 * @return uint8_t The received byte
 ******************************************************************************/
uint8_t uart_receive(void);

/** ***************************************************************************
 * @brief stdio wrapper for uart_transmit
 * 
 * @param[in] c Byte to be written
 * @param[in] stream IO-stream object
 * @return int 0
*******************************************************************************/
int uart_transmit_stdio(char c, FILE *stream);

/** ***************************************************************************
 * @brief stdio wrapper for uart_receive
 * 
 * @param[in] stream IO-stream object
 * @return int Received byte
*******************************************************************************/
int uart_receive_stdio(FILE *stream);