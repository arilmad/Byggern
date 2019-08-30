#define F_CPU 1600000

#define FOSC 1843200// Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

#include <avr/io.h>
#include <util/delay.h>
#include "uart.h"

int main()
{
    
    UART_Init( MYUBRR );

    while(1)
    {
        UART_Transmit('A');
        _delay_ms(200);
    }
    
    return 0;
}