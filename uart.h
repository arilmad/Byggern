#ifndef UART_H
#define UART_H
#include <stdint.h>
#include <stdio.h>

void UART_Init( int ubrr );
int UART_Transmit( char data, FILE *stream );
int UART_Receive( FILE *stream );

#endif