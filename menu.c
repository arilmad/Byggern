#include "menu.h"

menu_t* MainMenu;
menu_t* highlighted_node;


menu_t* menu_make_child(char *c, struct menu_t* parent){
	return (menu_t){c, parent, NULL, NULL};
}

void menu_update_parent(struct menu_t* parent, struct menu_t* child_){
	parent->child = child_;
}

menu_t* menu_make_sibling(char *c, struct menu_t* parent, struct menu_t* sibling) {

	return (menu_t){c, parent, NULL, sibling};

}

void menu_update_sibling(struct menu_t* left, struct menu_t* right){
	left->sibling = right;
}

menu_t* menu_get_highlighted_node() {
	return highlighted_node;
}

void menu_print(){
	menu_t* sibling_node = highlighted_node->parent->child;
	uint8_t page = 0;
	while (1)
	{
		oled_pos(page,0);
		oled_printf(sibling_node->name);
		if(sibling_node->sibling == NULL) { break; }
		sibling_node = sibling_node->sibling;
		page ++;
	}

}

void menu_init()
{	
	MainMenu = menu_make_child("Main Menu", NULL);

	menu_t* HighScores = menu_make_child("Highscores", MainMenu);
	menu_update_parent(MainMenu, HighScores);

	menu_t* PlayManual = menu_make_child("Play Manual Game", MainMenu);
	menu_update_sibling(HighScores, PlayManual);

	menu_t* PlayAuto = menu_make_child("Play Autonomously", MainMenu);
	menu_update_sibling(PlayManual, PlayAuto);

	menu_t* AllTimeHigh = menu_make_child("All Time Highs", HighScores);
	menu_update_parent(HighScores, AllTimeHigh);

	highlighted_node = HighScores;
}