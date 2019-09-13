#ifndef UART_H
#define UART_H
#include <stdint.h>
#include <stdio.h>

void UART_init( int ubrr );
int UART_transmit( char data, FILE *stream );
int UART_receive( FILE *stream );

#endif