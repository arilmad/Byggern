#include "menu.h"
#include "oled.h"

#include <stdlib.h>
#include <string.h>

menu_t MainMenu;
menu_t HighlightedNode;
menu_t Back;

uint8_t new_highscore_flag = 0;

uint8_t menu_get_new_highscore_flag()
{
	return new_highscore_flag;
}

uint8_t menu_reset_new_highscore_flag()
{
	new_highscore_flag = 0;
}

menu_t menu_create_menu_node(char *name)
{
	menu_t tp = (menu_t)malloc(sizeof(struct Menu) + 1);
	tp->name = name;
	tp->parent = NULL;
	tp->child = NULL;
	tp->sibling = NULL;

	return tp;
}

void menu_init()
{
	MainMenu = menu_create_menu_node("Main Menu");
	Back = menu_create_menu_node("Back");
}

void menu_init_highlighted_node()
{
	HighlightedNode = MainMenu->child;
}

menu_t menu_get_main_menu()
{
	return MainMenu;
}

char *menu_get_highlighted_node_name()
{
	return HighlightedNode->name;
}

void menu_generate_children(menu_t parent, char *arr[], uint8_t number_of_children)
{
	menu_t temp = menu_create_menu_node(arr[0]);
	parent->child = temp;
	temp->parent = parent;

	for (size_t i = 1; i < number_of_children; i++)
	{
		menu_t sibling = menu_create_menu_node(arr[i]);
		sibling->parent = parent;

		temp->sibling = sibling;

		temp = sibling;
	}
	if (parent != MainMenu)
		temp->sibling = Back;
}

void menu_scroll_highlighted_node(joystick_dir_t direction)
{
	if (direction == DOWN)
	{
		if (HighlightedNode->sibling == NULL)
			HighlightedNode = HighlightedNode->parent->child;

		else
			HighlightedNode = HighlightedNode->sibling;
	}
	else
	{
		menu_t tp = HighlightedNode->parent->child;

		if (tp == HighlightedNode)
			while (tp->sibling != NULL)
				tp = tp->sibling;
		else
			while (tp->sibling != HighlightedNode)
				tp = tp->sibling;

		HighlightedNode = tp;
	}
	menu_print_menu();
}

int8_t menu_change_menu_level()
{
	if (HighlightedNode == Back)
		HighlightedNode = HighlightedNode->parent;

	else if (HighlightedNode->child != NULL)
	{
		Back->parent = HighlightedNode;
		HighlightedNode = HighlightedNode->child;
	}
	else
		return 0;

	oled_reset();
	menu_print_menu();
	return 1;
}


void menu_print_menu()
{
	menu_t SiblingNode = HighlightedNode->parent->child;

	uint8_t page = 0;

	oled_pos(page, 0);
	oled_printf(HighlightedNode->parent->name, 8, 0);

	page++;

	oled_pos(page, 0);

	for (uint8_t i = 0; i < 16; i++)
		oled_printf("-", 8, 0);

	page++;

	while (SiblingNode)
	{
		oled_pos(page++, 0);

		if (SiblingNode == HighlightedNode)
			oled_printf(SiblingNode->name, 5, 1);

		else
			oled_printf(SiblingNode->name, 5, 0);

		SiblingNode = SiblingNode->sibling;
	}
}

void menu_update_highscores(char *score, char *highscore_node_name, uint8_t max_number_of_highscores)
{
	menu_t NewScore = menu_create_menu_node(score);

	menu_t tp = MainMenu->child;
	menu_t tp2;

	uint8_t node_counter = 0;

	// Fetch the Highscore node
	while (tp->name != highscore_node_name)
		tp = tp->sibling;

	NewScore->parent = tp;

	// If no highscore exists
	if (tp->child == NULL)
	{
		tp->child = NewScore;
		NewScore->sibling = Back;
		tp = tp->child;
		new_highscore_flag = 1;
	}

	else
	{
		tp = tp->child;

		while (1)
		{
			if (atoi(score) > atoi(tp->name))
			{
				// If new highscore
				if (tp == tp->parent->child)
				{
					tp->parent->child = NewScore;
					NewScore->sibling = tp;
					new_highscore_flag = 1;
				}
				else
				{
					tp2->sibling = NewScore;
					NewScore->sibling = tp;
				}

				break;
			}

			else if (tp->sibling == Back)
			{
				NewScore->sibling = Back;
				tp->sibling = NewScore;
				break;
			}
			tp2 = tp;
			tp = tp->sibling;
		}
	}

	// Free up any Highscore nodes below top 5 (or other max)
	tp = tp->parent->child;
	while (tp->sibling != Back)
	{
		node_counter++;

		if (node_counter == max_number_of_highscores)
		{
			free(tp->sibling->name);
			free(tp->sibling);
			tp->sibling = Back;
			break;
		}

		tp = tp->sibling;
	}
}

