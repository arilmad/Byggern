#define F_CPU 4915200

#define FOSC 4915200 // Clock Speed
#define BAUD 9600
#define MYUBRR FOSC / 16 / BAUD - 1


#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>

#include "uart.h"
#include "xmem.h"
#include "joystick.h"
#include "slider.h"
#include "oled.h"
#include "menu.h"


int main()
{



    UART_init(MYUBRR);
    xmem_init();
    oled_init();
    _delay_ms(40);
	menu_init();
	menu_print();

    while (1)
    {
    	


        _delay_ms(20);
    }
    return 0;
}
