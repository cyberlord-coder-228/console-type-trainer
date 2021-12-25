/*
 *  This app is cuurently under development. Not usage-ready.
 */

#include <ncurses.h>

int main()
{
    srand(time(NULL));

    initscr();                  // Starts ncurses

    cbreak();                   // Makes typed characters immediately available
    noecho();                   // Keyboard input not printed
    scrollok(stdscr, FALSE);    // Scroll disabled
    nodelay(stdscr, TRUE);      // Causes getch to be a non-blocking call
    //keypad(NULL, TRUE);         // Getch returns special stuff from arrow key presses
    start_color();              // Enables colors

    endwin();                   // Ends ncurses mode

    return 0;
} 
