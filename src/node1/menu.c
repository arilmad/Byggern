#include "menu.h"
#include <stdlib.h>
#include <string.h>
menu_t MainMenu;
menu_t HighlightedNode;
menu_t Back;


menu_t menu_create_menu_node(char* name)
{
	menu_t temp = (menu_t)malloc(sizeof(struct Menu));
	temp->name = name;
	temp->parent = NULL;
	temp->child = NULL;
	temp->sibling = NULL;

	return temp;
}

void menu_init()
{	
	MainMenu = menu_create_menu_node("Main Menu");
	Back = menu_create_menu_node("Back");
}

menu_t menu_get_main_menu() {
	return MainMenu;
}

void menu_init_highlighted_node()
{
	HighlightedNode = MainMenu->child;
}

void menu_generate_children(menu_t parent, char* arr[], uint8_t number_of_children)
{
	menu_t temp = menu_create_menu_node(arr[0]);
	parent->child = temp;
	temp->parent = parent;

	for (size_t i = 1; i < number_of_children ; i++)
	{
		menu_t sibling = menu_create_menu_node(arr[i]);
		sibling->parent = parent;

		temp->sibling = sibling;

		temp = sibling;	
	}
	if (parent != MainMenu)
	{
		temp->sibling = Back;
	}
	
}

void menu_scroll_highlighted_node(joystick_dir_t direction){
	if (direction == DOWN)
	{
		if (HighlightedNode->sibling == NULL)
		{
			HighlightedNode = HighlightedNode->parent->child;
		} else 
		{
			HighlightedNode = HighlightedNode->sibling;
		}
		
	} else
	{
		menu_t temp = HighlightedNode->parent->child;

		if (temp == HighlightedNode)
		{
			while (temp->sibling != NULL)
			{
				temp = temp->sibling;
			}
		} else
		{
			while (temp->sibling != HighlightedNode)
			{
				temp = temp->sibling;
			}
		}

		HighlightedNode = temp;
	}
	menu_print_menu();
}

int8_t menu_change_menu_level()
{
	if (HighlightedNode == Back)
	{
		HighlightedNode = HighlightedNode->parent;

	} else if (HighlightedNode->child != NULL)
	{
		Back->parent = HighlightedNode;
		HighlightedNode = HighlightedNode->child;

	} else { return 0; }
	oled_reset();
	menu_print_menu();
	return 1;
}

char* menu_get_highlighted_node_name()
{
	return HighlightedNode->name;
}


void menu_print_menu(){
	menu_t SiblingNode = HighlightedNode->parent->child;

	uint8_t page = 0;

	oled_pos(page,0);
	oled_printf(HighlightedNode->parent->name, 8, 0);

	page++;
	oled_pos(page,0);

	for(uint8_t i = 0; i < 16; i++)
	{
		oled_printf("-", 8, 0);
	}
	page++;
	while (1)
	{
		oled_pos(page,0);
		if (SiblingNode == HighlightedNode)
		{
			oled_printf(SiblingNode->name, 5, 1);
		} else {
			oled_printf(SiblingNode->name, 5, 0);
		}

		if(SiblingNode->sibling == NULL) { break; }

		SiblingNode = SiblingNode->sibling;
		page ++;
	}

}
