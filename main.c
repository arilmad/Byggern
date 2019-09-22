#define F_CPU 4915200

#define FOSC 4915200 // Clock Speed
#define BAUD 9600
#define MYUBRR FOSC / 16 / BAUD - 1


#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>


#include "uart.h"
#include "xmem.h"
#include "joystick.h"
#include "slider.h"
#include "oled.h"
#include "menu.h"
#include "bitmaps.h"


int main()
{

    UART_init(MYUBRR);
    xmem_init();
    oled_init();
    _delay_ms(40);
	menu_init();
    joystick_init();

    joystick_dir_t dir;

    menu_t node;

    const unsigned char* picture = harald;
    oled_print_bitmap(harald);
    oled_print_welcome_message();


    int num_loops = 0;
    uint8_t enter_menu = 1;
    while (1)
    {

        joystick_get_relative_pos();
        dir = joystick_get_y_dir();

        if (dir != NEUTRAL && num_loops > 10)
        {
            if (enter_menu)
            {
                oled_reset();
                menu_print_menu();
                enter_menu = 0;
            }

            menu_scroll_highlighted_node(dir);
            num_loops = 0;
        }

        if (joystick_button_pressed)
        {
            menu_change_menu_level();
            joystick_button_pressed = 0;
        }


        num_loops++;
        _delay_ms(20);
    }
    return 0;
}
