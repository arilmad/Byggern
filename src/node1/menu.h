#ifndef __MENU_H__
#define __MENU_H__

#include "joystick.h"

#include <stdint.h>

typedef struct Menu *menu_t;

struct Menu
{
	char *name;
	struct Menu *parent;
	struct Menu *child;
	struct Menu *sibling;
};

uint8_t menu_get_new_highscore_flag(void);
uint8_t menu_reset_new_highscore_flag(void);

void menu_init(void);
void menu_init_highlighted_node(void);

menu_t menu_get_main_menu(void);
char *menu_get_highlighted_node_name();

void menu_generate_children(menu_t parent, char *arr[], uint8_t number_of_children);
void menu_scroll_highlighted_node(joystick_dir_t direction);
int8_t menu_change_menu_level(void);

void menu_print_menu(void);

void menu_update_highscores(char *score, char *highscore_node_name, uint8_t max_number_of_highscores);

#endif






