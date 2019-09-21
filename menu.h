#ifndef __MENU_H__
#define __MENU_H__

#include <stdint.h>
#include <stdlib.h>

#include "oled.h"

typedef struct Menu *menu_t;

struct Menu {
	char* name;
	struct Menu* parent;
	struct Menu* child;
	struct Menu* sibling;
};

menu_t menu_get_highlighted_node(void);
void menu_print(void);
void menu_init(void);


#endif