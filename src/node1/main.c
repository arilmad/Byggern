#define F_CPU 4915200

#define FOSC 4915200 // Clock Speed
#define BAUD 9600
#define MYUBRR FOSC / 16 / BAUD - 1
#define TEN_MS 0.01 / 1 / FOSC / 1024;

#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>

#include "../../lib/uart/uart.h"
#include "../../lib/can/can_driver.h"
#include "../../assets/bitmaps.h"
#include "xmem.h"
#include "joystick.h"
#include "slider.h"
#include "oled.h"
#include "menu.h"

void T0delay();

int main()
{

    UART_init(MYUBRR);
    xmem_init();
    oled_init();
    _delay_ms(40);
    menu_init();
    joystick_init();

    can_init();

    joystick_dir_t joystick_x_dir, joystick_y_dir;
    joystick_pos_t joystick_current_pos, joystick_new_pos;

    can_message_t can_joystick_pos = {NULL, 8, NULL};

    can_message_t can_receive;

    const unsigned char *picture = harald;
    oled_print_bitmap(harald);

    oled_print_welcome_message();

    uint8_t enter_menu = 1;

    sei();

    joystick_current_pos = joystick_get_relative_pos();

    

    while (1)
    {
        
        /*
        joystick_new_pos = joystick_get_relative_pos();

        joystick_x_dir = joystick_get_x_dir();
        joystick_y_dir = joystick_get_y_dir();

        ms_elapsed = (int)(clock() - clock_start)/CLOCKS_PER_SEC/1000;

        if (joystick_y_dir != NEUTRAL && ms_elapsed > 10)
        {
            if (enter_menu)
            {
                oled_reset();
                menu_print_menu();
                enter_menu = 0;
            }
            else
            {
                menu_scroll_highlighted_node(joystick_y_dir);
            }

            clock_start = clock();
        }

        if (joystick_button_pressed)
        {
            menu_change_menu_level();
            joystick_button_pressed = 0;
        }
        */

        /*
            CAN_INTE vil du enable interrupts på 

            mottatt melding? Sjekk CAN_INTF register om melding mottatt 
                les melding få data
            sette CAN_INTF lav igjen

        */
        /*
        if (joystick_new_pos.x != joystick_current_pos.x)
        {
            can_joystick_pos.id = 1;
            can_joystick_pos.data[0] = (0xFF & joystick_new_pos.x);

            can_message_send(&can_joystick_pos);

            joystick_current_pos.x = joystick_new_pos.x;
            printf("%s\n\r", "Sent new x dir via CAN");
        }

        _delay_ms(10);
        */
        /*
        if (!(can_message_read(&can_receive)))
        {
            printf("%d\n\r", can_receive.data[0]);
        }

        _delay_ms(10);
        */
        /*
        if (joystick_new_pos.y != joystick_current_pos.y)
        {
            can_joystick_pos.id = 2; // id 2 for y pos
            can_joystick_pos.data[0] = (0xFF & joystick_new_pos.y);

            can_message_send(&can_joystick_pos);
            joystick_current_pos.y = joystick_new_pos.y;
            printf("%s\n\r", "Sent new y dir via CAN");
        }
        */
        /*
        _delay_ms(10);

        if (!(can_message_read(&can_receive)))
        {
            printf("%d\n\r", can_receive.data[0]);
        }
        */

       //_delay_ms(20);
       printf("%s\n\r", "Timer start");
       T0delay();
       printf("%s\n\r", "Timer end");
    }
    return 0;
}

void T0delay()
{
    TCCR0 = (1 << CS02) | (1 << CS00); /* Timer0, normal mode, /1024 prescalar */
    TCNT0 = TEN_MS; // no. of cycles in 10ms                /* Load TCNT0, count for 10ms */
    while ((TIFR & 0x01) == 0)
        ; /* Wait for TOV0 to roll over */
    TCCR0 = 0;
    TIFR = 0x1; /* Clear TOV0 flag */
}