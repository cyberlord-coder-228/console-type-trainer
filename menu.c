// that`s only a prototype, real menu will be better, I promise)

#include<ncurses.h>

#define AMOUNT_OF_MENU_OPTIONS 3
#define LONGEST_OPTION_LENGTH 20

#define TOP_PADDING 0
#define LEFT_PADDING 0
#define menu_WINDOW_WIDTH 63
#define menu_WINDOW_HEIGHT 27

#define MY_KEY_ENTER 10

const char LIST[AMOUNT_OF_MENU_OPTIONS][LONGEST_OPTION_LENGTH] = {
	"Start basic exercise",
	"Select level",
	"Exit"
};

enum
{
	BASIC_MENU_OPTION = 0,
	LEVEL_SELECT_MENU_OPTION = 1,
	EXIT_MENU_OPTION = 2
};

void print_menu(WINDOW* active_window)
{
	wattron(active_window, A_STANDOUT);
	for (int i = 0; i < AMOUNT_OF_MENU_OPTIONS; i++)
	{
		mvwprintw(active_window, i, LEFT_PADDING, "%s", LIST[i]);
		wattroff(active_window, A_STANDOUT);
	}
	wrefresh(active_window);
}

void loop_menu(WINDOW* active_window, int sel_line)
{
	int selected_line = sel_line;
	while(1)
	{
		int user_input = wgetch(active_window);
		
		// deselect previous selection
		wattroff(active_window, A_STANDOUT);
		mvwprintw(
			active_window,
			selected_line,
			LEFT_PADDING,
			"%s",
			LIST[selected_line]
		);
		wrefresh(active_window);

		switch(user_input)
		{
			case MY_KEY_ENTER:
			{
				switch (selected_line)
				{
					case BASIC_MENU_OPTION:
					{
						break;
					}
					case LEVEL_SELECT_MENU_OPTION:
					{
						break;
					}
					case EXIT_MENU_OPTION:
					{
						delwin(active_window);
						endwin();
						return;
					}
				}
			}
			case KEY_UP:
			{
				selected_line = (--selected_line >= 0)
					? selected_line : AMOUNT_OF_MENU_OPTIONS - 1;
				break;
			}
			case KEY_DOWN:
			{
				selected_line = (++selected_line < AMOUNT_OF_MENU_OPTIONS)
					? selected_line : 0;
				break;
			}
		}

		// highlight selected item
		wattron(active_window, A_STANDOUT);
		mvwprintw(
			active_window,
			selected_line, 
			LEFT_PADDING,
			"%s",
			LIST[selected_line]
		);
		wrefresh(active_window);
	}
}


int main()
{
	initscr();						// starts ncurses

	WINDOW* menu_window = newwin(
		menu_WINDOW_HEIGHT,
		menu_WINDOW_WIDTH,
		LEFT_PADDING,
		TOP_PADDING
	);

	noecho();						// keyboard input not printed
	keypad(menu_window, TRUE);		// getch returns special stuff from arrows
	curs_set(0);					// hide cursor

	print_menu(menu_window);
	loop_menu(menu_window, 0);

	delwin(menu_window);
	endwin();

	return 0;
}
