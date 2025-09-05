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
    UBRR0H = (unsigned char)(ubrr >> 8);
    UBRR0L = (unsigned char)ubrr;
    
    // Enable receiver and transmitter
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);
    
    // Set frame format: 8 data bits, 1 stop bit
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

/** ***************************************************************************
 * @brief Transmit a single byte of data via UART
 * 
 * @param[in] data The byte to be transmitted
 ******************************************************************************/
void uart_transmit(unsigned char data) {

    // Wait for empty transmit buffer
    while (!(UCSR0A & (1 << UDRE0))) {
        // Do nothing, just wait
    }
    // Put data into buffer, sends the data
    UDR0 = data;
}