/*
 *  This app is cuurently under development. Not usage-ready.
 */

#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

enum
{
    RED_FOREGROUND_NUM  = 1,
    GREEN_FOREGROUND_NUM = 2
};

char* LATIN_LETTERS = "abcdefghijklmnopqrstuvwxyz";

char get_rand_letter(char* arr, int arr_length)
{
    return arr[rand() % arr_length];
}

int simple_practice(int amount_of_symbols)
{
    for (int i = 0; i < amount_of_symbols; i++)
    {
        char rand_char = get_rand_letter(LATIN_LETTERS, 26);
        mvaddch(0, i, rand_char);
        refresh();

        char user_input = getch();

        int color_num;

        if (rand_char == user_input)
        {
            color_num = GREEN_FOREGROUND_NUM;
        }
        else
        {
            color_num = RED_FOREGROUND_NUM;
        }

        attrset(COLOR_PAIR(color_num));
        mvaddch(0, i, rand_char);
        refresh();
        attroff(COLOR_PAIR(color_num));
     }
}

int main()
{
    srand(time(NULL));

    initscr();                  // Starts ncurses

    cbreak();                   // Makes typed characters immediately available
    noecho();                   // Keyboard input not printed
    scrollok(stdscr, FALSE);    // Scroll disabled
    keypad(NULL, TRUE);         // Getch returns special stuff from arrow key presses
    start_color();              // Enables colors

    init_pair(RED_FOREGROUND_NUM, COLOR_RED, COLOR_BLACK);
    init_pair(GREEN_FOREGROUND_NUM, COLOR_GREEN, COLOR_BLACK);

    simple_practice(60);

    napms(1000);                // waits a second to appriciate what user has done

    endwin();                   // Ends ncurses mode

    return 0;
} 
