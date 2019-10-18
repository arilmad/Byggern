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

#include "../../lib/can/can_driver.h"
#include "../../lib/uart/uart.h"
#include "../../assets/bitmaps.h"
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
    joystick_init();

    can_init();

    joystick_dir_t current_x_dir = NEUTRAL;
    joystick_dir_t current_y_dir = NEUTRAL;

    joystick_dir_t new_x_dir;
    joystick_dir_t new_y_dir;

    menu_t node;

    const unsigned char *picture = harald;
    oled_print_bitmap(harald);
    oled_print_welcome_message();

    int num_loops = 0;
    uint8_t enter_menu = 1;


    sei();

    while (1)
    {

        joystick_get_relative_pos();

        new_x_dir = joystick_get_x_dir();
        new_y_dir = joystick_get_y_dir();

        if (new_y_dir != NEUTRAL && num_loops > 5)
        {
            if (enter_menu)
            {
                oled_reset();
                menu_print_menu();
                enter_menu = 0;
            }
            else
            {
                menu_scroll_highlighted_node(new_y_dir);
            }
            num_loops = 0;
        }

        if (joystick_button_pressed)
        {
            menu_change_menu_level();
            joystick_button_pressed = 0;
        }

        /*
            CAN_INTE vil du enable interrupts på 

            mottatt melding? Sjekk CAN_INTF register om melding mottatt 
                les melding få data
            sette CAN_INTF lav igjen

        */

       if (new_x_dir != current_x_dir)
       {
           can_message_t can_joystick_x_pos = {1, 3, (char *)(new_x_dir)}; // id 1 for x pos. // Fix declaration?
           can_message_send(&can_joystick_x_pos);
           current_x_dir = new_x_dir;
       }

       if (new_y_dir != current_y_dir)
       {
           can_message_t can_joystick_y_pos = {2, 3, (char *)(new_x_dir)}; // id 2 for y pos
           can_message_send(&can_joystick_y_pos);
           current_y_dir = new_y_dir;
       }

        num_loops++;
         _delay_ms(20);
    }
    return 0;
}
