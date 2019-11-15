#ifndef __MENU_H__
#define __MENU_H__

#include <stdint.h>
#include <stdlib.h>

#include "oled.h"
#include "joystick.h"

#include "../../lib/uart/uart.h"

typedef struct Menu *menu_t;

struct Menu
{
	char *name;
	struct Menu *parent;
	struct Menu *child;
	struct Menu *sibling;
};


menu_t menu_get_main_menu(void);

void menu_generate_children(menu_t parent, char *arr[], uint8_t number_of_children);
uint8_t menu_update_highscores(char *score, char *highscore_node_name, uint8_t max_number_of_highscores);

void menu_scroll_highlighted_node(joystick_dir_t direction);
int8_t menu_change_menu_level(void);
char *menu_get_highlighted_node_name();
void menu_print_menu(void);
void menu_init(void);

#endif






