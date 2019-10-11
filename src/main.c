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

#include "../lib/can/can_driver.h"
#include "uart.h"
#include "xmem.h"
#include "joystick.h"
#include "slider.h"
#include "oled.h"
#include "menu.h"
#include "../assets/bitmaps.h"

int main()
{

    UART_init(MYUBRR);
    xmem_init();
    oled_init();
    _delay_ms(40);
    menu_init();
    joystick_init();

    can_init();

    joystick_dir_t dir;

    menu_t node;

    const unsigned char *picture = harald;
    oled_print_bitmap(harald);
    oled_print_welcome_message();

    int num_loops = 0;
    uint8_t enter_menu = 1;

    can_message_t message1 = {1, 2, "M1"};
    can_message_t message2 = {2, 2, "M2"};
    can_message_t message3 = {3, 2, "M3"};

    can_message_t response;

    char status;
    //   printf("%s\n\r", message1.data);
    sei();
    while (1)
    {

        joystick_get_relative_pos();
        dir = joystick_get_y_dir();

        if (dir != NEUTRAL && num_loops > 5)
        {
            if (enter_menu)
            {
                oled_reset();
                menu_print_menu();
                enter_menu = 0;
            }
            else
            {
                menu_scroll_highlighted_node(dir);
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

        can_message_send(&message2);
        _delay_ms(10);
        can_message_read(&response);
        printf("%s\n\r", response.data);
        _delay_ms(10);
        can_message_send(&message1);
        _delay_ms(10);

        can_message_read(&response);
        printf("%s\n\r", response.data);
        _delay_ms(10);

        can_message_send(&message3);
        _delay_ms(10);
        can_message_read(&response);
        printf("%s\n\r", response.data);
        _delay_ms(10);

        num_loops++;
        // status = mcp2515_read(MCP_CANSTAT);
         _delay_ms(20);
        // printf("%x\n\r", status);
    }
    return 0;
}
