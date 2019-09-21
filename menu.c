#include "menu.h"

menu_t MainMenu;
menu_t HighlightedNode;


menu_t menu_create_menu_node(char* name)
{
	menu_t temp = (menu_t)malloc(sizeof(struct Menu));
	temp->name = name;
	temp->parent = NULL;
	temp->child = NULL;
	temp->sibling = NULL;

	return temp;
}

menu_t menu_get_highlighted_node() {
	return highlighted_node;
}

void menu_print_menu(){
	menu_t SiblingNode = HighlightedNode->parent->child;
	uint8_t page = 0;
	while (1)
	{
		oled_pos(page,0);
		oled_printf(SiblingNode->name);
		if(SiblingNode->sibling == NULL) { break; }
		SiblingNode = SiblingNode->sibling;
		page ++;
	}
}

void menu_init()
{	
	MainMenu = menu_create_menu_node("Main Menu");

	menu_t Highscores = menu_create_menu_node("Highscores");
	menu_t PlayManual = menu_create_menu_node("Play Manual Game");
	menu_t PlayAuto = menu_create_menu_node("Play Autonomously");

	menu_t AllTimeHigh = menu_create_menu_node("All Time Highs");

	MainMenu->child = Highscores;

	Highscores->parent = MainMenu;
	Highscores->child = AllTimeHigh;
	Highscores->sibling = PlayManual;

	PlayManual->parent = MainMenu;
	PlayManual->sibling = PlayAuto;
	
	PlayAuto->parent = MainMenu;
	
	AllTimeHigh->parent = Highscores;

	highlighted_node = Highscores;
}