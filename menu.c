// that`s only a prototype, real menu will be better, I promise)

#include<ncurses.h>

#define AMOUNT_OF_MENU_OPTIONS 3
#define LONGEST_OPTION_LENGTH 20

#define IN_GAME_WINDOW_WIDTH 63
#define IN_GAME_WINDOW_HEIGHT 27

#define MY_KEY_ENTER 10

const char LIST[AMOUNT_OF_MENU_OPTIONS][LONGEST_OPTION_LENGTH] = {
	"Start basic exercise",
	"Select level",
	"Exit"
};

enum
{
	BASIC = 0,
	LEVEL_SELECT = 1,
	EXIT = 2
};


int main()
{
	initscr();						// starts ncurses

	WINDOW* in_game_window = newwin(
		IN_GAME_WINDOW_HEIGHT,
		IN_GAME_WINDOW_WIDTH,
		0,
		0
	);

	noecho();						// keyboard input not printed
	keypad(in_game_window, TRUE);	// getch returns special stuff from arrows
	curs_set(0);					// hide cursor

	// print all the menu items, highlighting the first one
	wattron(in_game_window, A_STANDOUT);
	for (int i = 0; i < AMOUNT_OF_MENU_OPTIONS; i++)
	{
		mvwprintw(in_game_window, i, 0, "%s", LIST[i]);
		wattroff(in_game_window, A_STANDOUT);
	}
	wrefresh(in_game_window);

	// execution
	int selected_line = 0;
	while(1)
	{
		int user_input = wgetch(in_game_window);

		// right pad with spaces to make the items appear with even width.
		mvwprintw(in_game_window, selected_line, 0, "%s", LIST[selected_line]);
		
		// use a variable to increment or decrement the value based on the input.
		switch(user_input)
		{
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
			case MY_KEY_ENTER:
			{
				switch (selected_line)
				{
					case BASIC:
					{
						break;
					}
					case LEVEL_SELECT:
					{
						break;
					}
					case EXIT:
					{
						delwin(in_game_window);
						endwin();
						return 0;
					}
				}
			}
		}

		// now highlight the next item in the list.
		wattron(in_game_window, A_STANDOUT);
		mvwprintw(in_game_window, selected_line, 0, "%s", LIST[selected_line]);
		wattroff(in_game_window, A_STANDOUT);

	}

	delwin(in_game_window);
	endwin();

	return 0;
}
