#ifndef __UART_H__
#define __UART_H__
#include <stdint.h>
#include <stdio.h>
#include <avr/io.h>

void UART_init( int ubrr );
int UART_transmit( char data, FILE *stream );
int UART_receive( FILE *stream );

#endif