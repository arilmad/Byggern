#define F_CPU 4915200

#define FOSC 4915200// Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "uart.h"
#include "xmem.h"

#include <stdlib.h>

int main()
{
    UART_Init( MYUBRR );
    xmem_init();
    /*
    DDRA = 0xFF;
    DDRE = 0xFF;
    PORTA = 0;
    PORTE = 0xFF;
    */

    while(1)
    {
        /* //Exercise 1
        printf("The value is %d \n", 5);
        _delay_ms(200);
        */
        
       /* //Exercise 2
        PORTA |= (1 << PA0) | (1 << PA2);
        _delay_ms(200);
        PORTE = 0;
        _delay_ms(200);
        PORTA = 0;
        _delay_ms(200);
        PORTE = 0xFF;
        _delay_ms(200);
        */
        _delay_ms(200);
    }
    
    return 0;
}