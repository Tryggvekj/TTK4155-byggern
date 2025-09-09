/** ***************************************************************************
 * @file uart.c
 * @author Magnus Carlsen Haaland
 * @brief UART driver implementation file
 * @version 0.1
 * @date 2025-08-05
 * 
 * @copyright Copyright (c) 2025 Byggarane
 * 
 ******************************************************************************/

#include "uart.h"

/** ***************************************************************************
 * @brief Initialize the UART with the given UBRR value
 * 
 * @param[in] ubrr The value written to the UBBR register to set the baud rate
 ******************************************************************************/
void uart_init(unsigned int ubrr) {

    // Set baud rate
    UBRR0H = (uint8_t)(ubrr >> 8);
    UBRR0L = (uint8_t)ubrr;

    // Enable receiver and transmitter
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);
    
    // Set frame format: 8 data bits, 1 stop bit
    UCSR0C = (1 << URSEL0) | (1 << USBS0) | (3 << UCSZ00);
}

/** ***************************************************************************
 * @brief Transmit a single byte of data via UART
 * 
 * @param[in] data The byte to be transmitted
 ******************************************************************************/
void uart_transmit(uint8_t data) {

    // Wait for empty transmit buffer
    while (!(UCSR0A & (1 << UDRE0))) {
        // Do nothing, just wait
    }
    // Put data into buffer, sends the data
    UDR0 = data;
}

/** ***************************************************************************
 * @brief Receive a single byte of data via UART
 * 
 * @return uint8_t The received byte
 ******************************************************************************/
uint8_t uart_receive(void) {
    // Wait for data to be received
    while (!(UCSR0A & (1 << RXC0))) {
        // Do nothing, just wait
    }
    // Get and return received data from buffer
    return UDR0;
}