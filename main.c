#define F_CPU 4915200

#define FOSC 4915200// Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>

#include "uart.h"
#include "xmem.h"
#include "joystick.h"
#include "slider.h"

int main()
{
    UART_init( MYUBRR );
    xmem_init();
    joystick_init();
    slider_init();

    joystick_pos_t joystick_pos;
    slider_pos_t slider_pos;

    while(1)
    {
        joystick_pos = read_joystick_pos();
        //printf("Joystick x: %d\n\r", joystick_pos.x);
        _delay_ms(200);
        //printf("Joystick y: %d\n\r", joystick_pos.y);
        _delay_ms(200);
        slider_pos = read_slider_pos();
        printf("Slider left: %d\n\r", slider_pos.left_pos);
        _delay_ms(200);
        printf("Slider right: %d\n\r", slider_pos.right_pos);
        _delay_ms(200);
    }
    
    return 0;
}
