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
char* LEARNER_SEQUENCE = "fjdksla;ghrueiwoqptyvmc,x.z/bn";      // has length 30


struct TimeHolder
{
    struct timeval start;
    struct timeval end;
};

char get_rand_letter(char* arr, int arr_length)
{
    return arr[rand() % arr_length];
}

float gibberish_words_exercise(
    int amount_of_symbols,
    int word_length,
    char* letters_to_practise,
    int letters_arr_length
)
{
    int mistakes = 0;
    int start_line = 0;

    for (
        int i = 0;
        i < amount_of_symbols;
        i += (++i % (word_length + 1) == word_length)
    )
    {
        char rand_char = get_rand_letter(
            letters_to_practise,
            letters_arr_length
        );
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

void print_how_good_user_was(float correct_ratio, int wpm)
{
    char* postfix;
    if (correct_ratio == 1.0) postfix = ". Perfect!";
    if (correct_ratio < 1.0)  postfix = "you have done great!";
    if (correct_ratio < 0.8)  postfix = "you have done quite good!";
    if (correct_ratio < 0.6)  postfix = "you are half way here!";
    if (correct_ratio < 0.4)  postfix = "keep practicing!";
    if (correct_ratio < 0.2)  postfix = "you can do better!";
    if (correct_ratio == 0.0) postfix = "did you even try?";
    if (correct_ratio > 1.0 || correct_ratio < 0.0)
        postfix = ". Wait what? How that`s even possible?";

    printw("\nYour accuracy was %.1f percent, %s", correct_ratio*100, postfix);
    printw("\nYour average speed was %d words per minute", wpm);
    refresh();
}

void offer_next_screen()
{
    printw("\n(Type in anything to continue.)");
    refresh();
    getch();
    clear();
}

void test()
{
    int symobols_in_exercise = 47;
    int word_length = 5;

    for (int i = 1; i < 16; i++)
    {
        struct TimeHolder timer;
        gettimeofday(&timer.start, NULL);

        float ratio = gibberish_words_exercise(
            symobols_in_exercise,
            word_length,
            LEARNER_SEQUENCE,
            i * 2
        );
        
        gettimeofday(&timer.end, NULL);

        int time_spent_seconds = timer.end.tv_sec - timer.start.tv_sec;
        
        // 12 is a constant from wpm equation
        // wpm = typed_symbols / time (sec) * 60 (sec) / 5 (average word length)
        int user_speed_wpm =
            (int)(12 * (float)symobols_in_exercise / (float)time_spent_seconds);

        print_how_good_user_was(ratio, user_speed_wpm);
        offer_next_screen();
    }
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

    test();

    endwin();                   // Ends ncurses mode

    return 0;
}
