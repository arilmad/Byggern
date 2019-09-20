#ifndef __MENU_H__
#define __MENU_H__

#include <stdint.h>

#include "oled.h"


struct menu_t {
	struct menu_t* parent;
	struct menu_t* child;
	struct menu_t* sibling;
	char* name;
} *temp;

menu_t* menu_get_highlighted_node(void);
void menu_print(void);
void menu_init(void);


#endif