#include "uart.h"

#include <stdint.h>
#include <stdio.h>
#include <avr/io.h>

int UART_transmit(char data, FILE *stream)
{
    /* Wait for empty transmit buffer */
    while (!(UCSR0A & (1 << UDRE0)))
        ;
    /* Put data into buffer, sends the data */
    UDR0 = data;

    return 0;
}

int UART_receive(FILE *stream)
{
    /* Wait for data to be received */
    while (!(UCSR0A & (1 << RXC0)))
        ;
    /* Get and return received data from buffer */
    return UDR0;
}

void UART_init(int ubrr)
{
    /* Set baud rate */
    UBRR0H = (unsigned char)(ubrr >> 8);
    UBRR0L = (unsigned char)ubrr;
    /* Enable receiver and transmitter */
    UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
    /* Set frame format: 8data, 2stop bit */

#if defined(__AVR_ATmega162__)
    UCSR0C |= (1 << URSEL0) | (1 << USBS0) | (3 << UCSZ00);
#elif defined(__AVR_ATmega2560__)
    UCSR0C |= (1 << USBS0) | (3 << UCSZ00);
#endif
    fdevopen(UART_transmit, UART_receive);
}

