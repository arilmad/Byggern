#define F_CPU 4915200

#define BAUD 9600
#define BT_BAUD 9600

#define UBRR F_CPU / 16 / BAUD - 1
#define BT_UBRR F_CPU / 16 / BT_BAUD - 1

#define MAX_NUMBER_OF_HIGHSCORES 5

#include "../../lib/uart/uart.h"
#include "../../lib/can/can_driver.h"
#include "../../lib/can/MCP2515.h"
#include "../../assets/bitmaps.h"
#include "xmem.h"
#include "joystick.h"
#include "io_button.h"
#include "slider.h"
#include "oled.h"
#include "menu.h"
#include "bluetooth.h"

#include <stdint.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>

/*  * Main defines what nodes the
    * menu shall consist of and
    * their parent / sibling
    * relationship.
*/
char *main_menu_nodes[] = {
    "Play Game",
    "Highscores"};

char *play_game_nodes[] = {
    "Play From Board",
    "Play From Phone",
    "Play Voice"};

char *highscore_nodes[] = {};

/* generate_menu()
    * Call menu.c build fns to create
    * desired menu structure.
*/
void generate_menu()
{
    menu_t MainMenu = menu_get_main_menu();
    menu_generate_children(MainMenu, main_menu_nodes, 2);
    menu_t PlayGame = MainMenu->child;
    menu_generate_children(PlayGame, play_game_nodes, 2);
    menu_t Highscore = PlayGame->sibling;
}

/* main()
    * The node1 main fn starts by printing 
    * a welcome message and a bitmap picture
    * of King Harald of Norway.
    * 
    * One purpose of the main loop is to allow
    * the user to navigate in the menu by using
    * the joystick on the I/O board. 
    * 
    * During active game, it supplies node2 with 
    * sensor data from the I/O card or the BT 
    * module, and receives from node2 periodic
    * messages signalling score shall be in-
    * cremented.
    * 
    * Node1 main also works with menu.c to keep
    * track of the top highscores.
*/
int main()
{
    cli();

    UART_init(UBRR);
    bluetooth_init(BT_UBRR);
    can_init(MODE_NORMAL);

    xmem_init();
    oled_init();

    _delay_ms(40);

    menu_init();
    generate_menu();
    menu_init_highlighted_node();

    io_button_init();
    joystick_init();

    joystick_dir_t joystick_y_dir;
    slider_pos_t slider_current_pos, slider_new_pos;

    can_message_t can_msg_send = {0, 8, 0x00};
    can_message_t can_msg_receive;

    uint8_t enter_menu = 1;
    uint8_t active_game = 0;
    uint8_t control_bluetooth = 0;
    uint8_t control_iocard = 0;

    uint8_t bt_int_val;
    uint8_t tp_int;

    uint8_t update_motor = 0;
    uint8_t update_servo = 0;
    uint8_t update_solenoid = 0;

    int16_t motor_value = -1;
    int16_t servo_value = -1;

    uint16_t score = 0;

    char *data;
    char *menu_select;

    char tp[10];
    char tp_char;

    const unsigned char *picture = harald;

    sei();

    oled_print_bitmap(harald);
    oled_print_welcome_message();
    slider_current_pos = slider_read_pos();

    while (1)
    {
        if (!active_game)
        {
            joystick_get_relative_pos();

            joystick_y_dir = joystick_get_y_dir();

            if (joystick_y_dir != NEUTRAL)
            {
                _delay_ms(150);

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

            if (joystick_get_button_pressed_flag())
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
                    if (active_game)
                    {
                        can_msg_send.id = 4;
                        can_message_send(&can_msg_send);

                        oled_reset();
                        oled_print_centered_message("Game on!", 8, 1, 0);
                        oled_print_centered_message("Current score", 8, 2, 0);
                    }
                }
                joystick_reset_button_pressed_flag();
            }
        }

        else
        {
            if ((can_message_read(&can_msg_receive)))
            {

                if (can_msg_receive.id == 0)
                {
                    score++;
                    sprintf(tp, "%d", score);
                    oled_print_centered_message(tp, 8, 4, 0);
                }
                else if (can_msg_receive.id == 1)
                {
                    cli();
                    active_game = 0;
                    control_bluetooth = 0;
                    control_iocard = 0;

                    char *tp_copy = strcpy((char *)malloc(strlen(tp) + 1), tp);
                    menu_update_highscores(tp_copy, main_menu_nodes[1], MAX_NUMBER_OF_HIGHSCORES);

                    oled_reset();

                    if (menu_get_new_highscore_flag())
                    {
                        menu_reset_new_highscore_flag();
                        oled_print_centered_message("NEW HIGHSCORE", 8, 7, 0);
                    }

                    oled_print_final_score(tp);
                    oled_reset();

                    menu_init_highlighted_node();
                    menu_print_menu();
                    score = 0;
                    sei();
                }
            }


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
                if (io_button_get_left_button_press())
                {
                    update_solenoid = 1;
                }
            }

            else if (control_bluetooth)
            {
                if (bluetooth_msg_available())
                {
                    cli();
                    data = bluetooth_read();

                    sscanf(data, "%c %d", &tp_char, &tp_int);
                    sei();

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
                    }
                }
            }

            if (update_servo)
            {
                can_msg_send.id = 1;
                can_msg_send.data[0] = servo_value;
                can_message_send(&can_msg_send);
                update_servo = 0;
            }

            if (update_motor)
            {
                can_msg_send.id = 2;
                can_msg_send.data[0] = motor_value;
                can_message_send(&can_msg_send);
                update_motor = 0;
            }

            if (update_solenoid)
            {
                can_msg_send.id = 3;
                can_msg_send.data[0] = (0xFF & 1);
                can_message_send(&can_msg_send);
                update_solenoid = 0;
            }
        }
    }
    return 0;
}
