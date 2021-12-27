#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

enum
{
    RED_BACKGROUND_NUM  = 1,
    GREEN_BACKGROUND_NUM = 2
};

char* LATIN_LETTERS = "abcdefghijklmnopqrstuvwxyz";

char get_rand_letter(char* arr, int arr_length)
{
    return arr[rand() % arr_length];
}

float simple_exercise(
    int amount_of_symbols,  // determines duration of an exercise
    char* letters_to_practise,
    int arr_length
)
{
    int mistakes = 0;

    int start_line = 0;
    for (int i = 0; i < amount_of_symbols; i++)
    {
        char rand_char = get_rand_letter(letters_to_practise, arr_length);
        mvaddch(start_line, i, rand_char);
        refresh();

        char user_input = getch();

        int color_num;

        if (rand_char == user_input)
        {
            color_num = GREEN_BACKGROUND_NUM;
        }
        else
        {
            mistakes++;
            color_num = RED_BACKGROUND_NUM;
        }

        attrset(COLOR_PAIR(color_num));
        mvaddch(start_line, i, rand_char);
        refresh();
        attroff(COLOR_PAIR(color_num));
    }

    float ratio_of_correct_to_all =
        1.0 * (amount_of_symbols - mistakes) / amount_of_symbols;
    return ratio_of_correct_to_all;
}

int main()
{
    srand(time(NULL));

    initscr();                  // Starts ncurses

    cbreak();                   // Makes typed characters immediately available
    noecho();                   // Keyboard input not printed
    scrollok(stdscr, FALSE);    // Scroll disabled
    keypad(NULL, TRUE);         // Getch returns special stuff from arrow keys
    start_color();              // Enables colors

    init_pair(RED_BACKGROUND_NUM, COLOR_BLACK, COLOR_RED);
    init_pair(GREEN_BACKGROUND_NUM, COLOR_BLACK, COLOR_GREEN);

    float ratio = simple_exercise(
        60,     // 60 symbols will br generated until function
        "jf ",  // user will practice symbols 'j', 'f', and space
        3       // length of a symbols array
    );
    
    printw("\n\nYour accuracy was %.1f percent,", ratio*100);
    printw("you've done great!");
    printw("\nType in anything to finish practicing.");
    refresh();

    getch();                    // takes a second to appriciate what user has done

    endwin();                   // Ends ncurses mode

    return 0;
}
