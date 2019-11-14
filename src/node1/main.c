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
char* main_menu_nodes[] = {
    "Play Game",
    "Highscores"
};

char* play_game_nodes[] = {
    "Play Manual",
    "Play Auto",
    "Play Voice"
};

char* highscore_nodes[] = {
    "All time high"
};


void generate_menu(){
    menu_t MainMenu = menu_get_main_menu();
    menu_generate_children(MainMenu, main_menu_nodes, 2);
    menu_t PlayGame = MainMenu->child;
    menu_generate_children(PlayGame, play_game_nodes, 3);
    menu_t Highscore = PlayGame->sibling;
    menu_generate_children(Highscore, highscore_nodes, 1);
    //printf("%s\r\n", PlayGame->name);
}

int main()
{
    cli(); 
    
    UART_init(MYUBRR);
    xmem_init();
    oled_init();
    _delay_ms(40);
    menu_init();
    generate_menu();
    menu_init_highlighted_node();
    joystick_init();

    can_init(MODE_NORMAL);

    DDRE |= (1<<PE0);

    DDRB &= (~(1<<PB2));

    joystick_dir_t joystick_y_dir;
    slider_pos_t slider_current_pos, slider_new_pos;

    can_message_t can_msg_send = {0, 8, 0x00};
    
    can_message_t can_msg_receive;

    char* menu_select;

    const unsigned char *picture = harald;
    oled_print_bitmap(harald);

    oled_print_welcome_message();

    uint8_t enter_menu = 1;

    slider_current_pos = slider_get_relative_pos();

    //init_timer();
    uint8_t active_game = 0;
    uint16_t score = 0;
    sei();

    while (1)
    {
        if (!active_game)
        {        
            joystick_get_relative_pos();

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
                if(!menu_change_menu_level()) // No child available
                {
                    menu_select = menu_get_highlighted_node_name();
                    if ( menu_select == play_game_nodes[0] )
                    {
                        can_msg_send.id = 4;
                        can_message_send(&can_msg_send);
                        active_game = 1;
                        oled_reset();
                        oled_pos(1,32);
                        oled_printf("Game on!", 8, 0);
                        oled_pos(2,12);
                        oled_printf("Current score", 8 ,0);
                        
                    } else if (menu_select == play_game_nodes[1])
                    {
                        can_msg_send.id = 5;
                        can_message_send(&can_msg_send);
                    }
                    
                }
                joystick_button_pressed = 0;
            }
        } else
        {
            if((can_message_read(&can_msg_receive)))
            {
                if ( can_msg_receive.id == 0 )
                {
                    score ++;
                    char tp[10];
                    sprintf(tp, "%d", score);
                    oled_pos(4,52);
                    oled_printf(tp,8,0);
                } else if ( can_msg_receive.id == 1 )
                {
                    active_game = 0;
                    oled_reset();
                    menu_print_menu();
                }
            }
        }
        
        
        slider_new_pos = slider_get_relative_pos();
        if (abs(slider_new_pos.left_pos - slider_current_pos.left_pos) > 3)
        {
            can_msg_send.id = 1;
            can_msg_send.data[0] = (0xFF & slider_new_pos.left_pos);

            can_message_send(&can_msg_send);

            slider_current_pos.left_pos = slider_new_pos.left_pos;
        }
        if (abs(slider_new_pos.right_pos - slider_current_pos.right_pos) > 3)
        {
            can_msg_send.id = 2; // id 2 for y pos
            can_msg_send.data[0] = (0xFF & slider_new_pos.right_pos);

            can_message_send(&can_msg_send);
            slider_current_pos.right_pos = slider_new_pos.right_pos;
        }
        if (PINB & (1 << PB2)) {
            can_msg_send.id = 3;
            can_msg_send.data[0] = 0xFF & 1;
            can_message_send(&can_msg_send);
        }
        

       _delay_ms(20);

    }
    return 0;
}

