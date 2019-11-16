#define F_CPU 4915200

#define BAUD 9600
#define BT_BAUD 9600

#define MYUBRR F_CPU / 16 / BAUD - 1
#define BT_MYUBRR 31

#define MAX_NUMBER_OF_HIGHSCORES 5

#include <stdint.h>
#include <string.h>
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
#include "bluetooth.h"

char *main_menu_nodes[] = {
    "Play Game",
    "Highscores"};

char *play_game_nodes[] = {
    "Play Manual",
    "Play From Phone",
    "Play Voice"};

char *highscore_nodes[] = {};

void generate_menu()
{
    menu_t MainMenu = menu_get_main_menu();
    menu_generate_children(MainMenu, main_menu_nodes, 2);
    menu_t PlayGame = MainMenu->child;
    menu_generate_children(PlayGame, play_game_nodes, 2);
    menu_t Highscore = PlayGame->sibling;
}

int main()
{
    cli();

    UART_init(MYUBRR);
    bluetooth_init(BT_MYUBRR);
    xmem_init();
    oled_init();
    _delay_ms(40);
    menu_init();
    generate_menu();
    menu_init_highlighted_node();
    joystick_init();

    can_init(MODE_NORMAL);

    DDRE |= (1 << PE0);

    DDRB &= (~(1 << PB1));

    joystick_dir_t joystick_y_dir;
    slider_pos_t slider_current_pos, slider_new_pos;

    can_message_t can_msg_send = {0, 8, 0x00};

    can_message_t can_msg_receive;

    char *menu_select;

    const unsigned char *picture = harald;
    oled_print_bitmap(harald);

    oled_print_welcome_message();

    uint8_t enter_menu = 1;

    slider_current_pos = slider_read_pos();

    //init_timer();
    uint8_t active_game = 0;
    uint8_t control_bluetooth = 0;
    uint8_t control_iocard = 0;

    uint8_t update_motor = 0;
    uint8_t update_servo = 0;
    uint8_t update_solenoid = 0;

    uint8_t motor_value = -1;
    uint8_t servo_value = -1;

    uint16_t score = 0;
    uint8_t bt_int_val;

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
                if (!menu_change_menu_level()) // No child available
                {
                    menu_select = menu_get_highlighted_node_name();
                    if (menu_select == play_game_nodes[0])
                    {
                        control_iocard = 1;
                        active_game = 1;
                    }
                    else if (menu_select == play_game_nodes[1])
                    {
                        control_bluetooth = 1;
                        active_game = 1;
                    }
                    else
                        return;

                    can_msg_send.id = 4;
                    can_message_send(&can_msg_send);

                    oled_reset();
                    oled_print_centered_message("Game on!", 8, 1, 0);
                    oled_print_centered_message("Current score", 8, 2, 0);
                }
                joystick_button_pressed = 0;
            }
        }

        else
        {
            if ((can_message_read(&can_msg_receive)))
            {
                char tp[10];
                if (can_msg_receive.id == 0)
                {
                    score++;
                    sprintf(tp, "%d", score);
                    oled_print_centered_message(tp, 8, 4, 0);
                }
                else if (can_msg_receive.id == 1)
                {
                    active_game = 0;

                    oled_reset();
                    if (menu_update_highscores(strcpy((char *)malloc(strlen(tp) + 1), tp), main_menu_nodes[1], MAX_NUMBER_OF_HIGHSCORES))
                    {
                        oled_print_centered_message("NEW HIGHSCORE", 8, 7, 0);
                    }

                    oled_print_final_score(tp);
                    oled_reset();

                    menu_init_highlighted_node();
                    menu_print_menu();
                    score = 0;
                }
            }
        }

        if (!active_game)
            continue;

        if (control_iocard)
        {
            slider_new_pos = slider_read_pos();
            if (abs(slider_new_pos.left_pos - slider_current_pos.left_pos) > 5)
            {
                servo_value = (0xFF & slider_new_pos.left_pos);
                slider_current_pos.left_pos = slider_new_pos.left_pos;
                update_servo = 1;
            }
            if (abs(slider_new_pos.right_pos - slider_current_pos.right_pos) > 5)
            {
                motor_value = (0xFF & slider_new_pos.right_pos);
                slider_current_pos.right_pos = slider_new_pos.right_pos;
                update_motor = 1;
            }
            if (PINB & (1 << PB1))
            {
                update_solenoid = 1;
            }
        }

        if (control_bluetooth)
        {
            char *data;
            char tp_char;
            uint8_t tp_int;
            while (bluetooth_available())
            {
                cli();
                data = bluetooth_read();
                sscanf(data, "%c %d", &tp_char, &tp_int);
                sei();
                printf("Char: %c, Value: %d", tp_char, tp_int);
                /*
                if (tp_char == 'A')
                {
                    update_solenoid = 1;
                }
                else if ((tp_char == 'B') && (tp_int >= 0 && tp_int <= 255))
                {
                    servo_value = tp_int;
                    update_servo = 1;
                }
                else if (tp_char == 'C' && (tp_int >= 0 && tp_int <= 255))
                {
                    motor_value = tp_int;
                    update_motor = 1;
                }*/
            }
        }

        if (update_motor)
        {
            can_msg_send.id = 1;
            can_msg_send.data[0] = motor_value;
            can_message_send(&can_msg_send);
            update_motor = 0;
        }

        if (update_servo)
        {
            can_msg_send.id = 2;
            can_msg_send.data[0] = servo_value;
            can_message_send(&can_msg_send);
            update_servo = 0;
        }

        if (update_solenoid)
        {
            can_msg_send.id = 3;
            can_msg_send.data[0] = (0xFF & 1);
            can_message_send(&can_msg_send);
            update_solenoid = 0;
        }
    }
    return 0;
}
