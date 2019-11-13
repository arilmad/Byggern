#define F_CPU 4915200

#define BAUD 9600
#define MYUBRR F_CPU / 16 / BAUD - 1
#define TEN_MS 19200/4
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

/*
ISR(TIMER0_OVF_vect)
{
    static uint64_t count = 0; //hold value of count between interrupts
    count++;
    static uint8_t toggle = 0;
    if (!(count % TEN_MS))
    {
        if (toggle)
        {
            PORTE |= (1<<PE0);
        } else
        {
            PORTE &= ~(1<<PE0);
        }
        toggle = !toggle;
    }
    TIFR |= (1 << TOV0);

} //TIMER0_OVF_vect

void init_timer();
*/
int main()
{
    cli(); 
    
    UART_init(MYUBRR);
    xmem_init();
    oled_init();
    _delay_ms(40);
    menu_init();
    joystick_init();

    can_init(MODE_NORMAL);

    DDRE |= (1<<PE0);

    DDRB &= (~(1<<PB2));

    joystick_dir_t joystick_y_dir;
    slider_pos_t slider_current_pos, slider_new_pos;

    can_message_t can_slider_pos = {NULL, 8, NULL};
    can_message_t can_button_pressed = {NULL, 8, NULL};

    can_message_t can_receive;

    const unsigned char *picture = harald;
    oled_print_bitmap(harald);

    oled_print_welcome_message();

    uint8_t enter_menu = 1;

    slider_current_pos = slider_get_relative_pos();

    //init_timer();

    sei();

    while (1)
    {
        
        joystick_get_relative_pos();
        slider_new_pos = slider_get_relative_pos();

        joystick_y_dir = joystick_get_y_dir();



        if (joystick_y_dir != NEUTRAL)
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

        }

        if (joystick_button_pressed)
        {
            menu_change_menu_level();
            joystick_button_pressed = 0;
        }
        

        
        if (abs(slider_new_pos.left_pos - slider_current_pos.left_pos) > 3)
        {
            can_slider_pos.id = 1;
            can_slider_pos.data[0] = (0xFF & slider_new_pos.left_pos);

            can_message_send(&can_slider_pos);

            slider_current_pos.left_pos = slider_new_pos.left_pos;
        }
        if (abs(slider_new_pos.right_pos - slider_current_pos.right_pos) > 3)
        {
            can_slider_pos.id = 2; // id 2 for y pos
            can_slider_pos.data[0] = (0xFF & slider_new_pos.right_pos);

            can_message_send(&can_slider_pos);
            slider_current_pos.right_pos = slider_new_pos.right_pos;
        }
        if (PINB & (1 << PB2)) {
            can_button_pressed.id = 3;
            can_button_pressed.data[0] = 0xFF & 1;
            can_message_send(&can_button_pressed);
        }
        

       _delay_ms(20);

    }
    return 0;
}

