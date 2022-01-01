#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>


enum
{
    RED_BACKGROUND_NUM  = 1,
    GREEN_BACKGROUND_NUM = 2
};

char* LATIN_LETTERS = "abcdefghijklmnopqrstuvwxyz";             // has length 26
char* LEARNER_SEQUENCE = " fjdksla;ghrueiwoqptyvmc,x.z/bn\'";   // has length 32


char get_rand_letter(char* arr, int arr_length)
{
    return arr[rand() % arr_length];
}

float simple_exercise(
    int amount_of_symbols,      // Determines duration of an exercise
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
        
    return (float)(amount_of_symbols - mistakes) / amount_of_symbols;
}

void print_how_good_user_was(float correct_ratio, float time_sec)
{
    printw("\n\nYour accuracy was %.1f percent, ", correct_ratio*100);

    char* postfix;
    if (correct_ratio == 0.0)
    {
        postfix = "did you even try?";
    }
    else if (correct_ratio < 0.2)
    {
        postfix = "you can do better!";
    }
    else if (correct_ratio < 0.4)
    {
        postfix = "keep practicing!";
    }
    else if (correct_ratio < 0.6)
    {
        postfix = "you are half way here!";
    }
    else if (correct_ratio < 0.8)
    {
        postfix = "you have done quite good!";
    }
    else if (correct_ratio < 1)
    {
        postfix = "you have done great!";
    }
    else if (correct_ratio == 1.0)
    {
        postfix = ". Perfect!";
    }
    else
    {
        postfix = ". Wait what? How that`s even possible?";
    }

    printw(postfix);
    printw("\nIt took you %.1f seconds btw", time_sec);
    refresh();
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

    // Now let`s test the prigram
    for (int i = 1; i < 16; i++)
    {
        struct timeval start;
        struct timeval end;

        gettimeofday(&start, NULL);
        float ratio = simple_exercise(12, LEARNER_SEQUENCE, i * 2);
        gettimeofday(&end, NULL);

        int time_spent_seconds  = end.tv_sec  - start.tv_sec;

        print_how_good_user_was(ratio, (float)time_spent_seconds);
        printw("\n(Type in anything to continue.)");
        refresh();
        getch();
        clear();
    }

    endwin();                   // Ends ncurses mode

    return 0;
}
