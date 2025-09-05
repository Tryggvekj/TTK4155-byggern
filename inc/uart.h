/** ***************************************************************************
 * @file uart.h
 * @author Magnus Carlsen Haaland
 * @brief UART driver header file
 * @version 0.1
 * @date 2025-08-05
 * 
 * @copyright Copyright (c) 2025 Byggarane
 * 
 ******************************************************************************/


#include <avr/io.h>


/** ***************************************************************************
 * @brief Initialize the UART with the given UBRR value
 * 
 * @param[in] ubrr The value written to the UBBR register to set the baud rate
 ******************************************************************************/
void uart_init(uint32_t ubrr);

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