#ifndef __MENU_H__
#define __MENU_H__

#include <stdint.h>
#include <stdlib.h>

#include "oled.h"
#include "joystick.h"

typedef struct Menu *menu_t;

struct Menu {
	char* name;
	struct Menu* parent;
	struct Menu* child;
	struct Menu* sibling;
};

void menu_scroll_highlighted_node(joystick_dir_t direction);
void menu_change_menu_level(void);

void menu_print_menu(void);
void menu_init(void);


#endif