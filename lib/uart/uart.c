#include "uart.h"

#include <stdint.h>
#include <stdio.h>
#include <avr/io.h>

/* UART_transmit()
    * Wait for empty transmit buffer.
    * Put data into buffer, sends the data.
*/
int UART_transmit(char data, FILE *stream)
{
    while (!(UCSR0A & (1 << UDRE0)))
        ;
    UDR0 = data;
    return 0;
}

/* UART_receive()
    * Wait for data to be received.
    * Get and return received data from buffer.
*/
int UART_receive(FILE *stream)
{
    while (!(UCSR0A & (1 << RXC0)))
        ;
    return UDR0;
}

/* UART_init()
    * Set baud rate as defined by main.
    * Enable receiver and transmitter.
    * Set frame format: 8 data, 2 stop bit.
    * Set up stream.
*/
void UART_init(int ubrr)
{
    UBRR0H = (unsigned char)(ubrr >> 8);
    UBRR0L = (unsigned char)ubrr;
    UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
    
    fdevopen(UART_transmit, UART_receive);

#if defined(__AVR_ATmega162__)
    UCSR0C |= (1 << URSEL0) | (1 << USBS0) | (3 << UCSZ00);
#elif defined(__AVR_ATmega2560__)
    UCSR0C |= (1 << USBS0) | (3 << UCSZ00);
#endif
}

