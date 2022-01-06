#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

#define IN_GAME_WINDOW_WIDTH 63
#define IN_GAME_WINDOW_HEIGHT 27


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
    WINDOW* active_window,
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
        mvwaddch(active_window, start_line, i, rand_char);
        wrefresh(active_window);

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

        wattrset(active_window, COLOR_PAIR(color_num));
        mvwaddch(active_window, start_line, i, rand_char);
        wrefresh(active_window);
        wattroff(active_window, COLOR_PAIR(color_num));
    }
        
    return (float)(amount_of_symbols - mistakes) / amount_of_symbols;
}

void print_how_good_user_was(
    WINDOW* active_window,
    float correct_ratio,
    int wpm
)
{
    char* postfix;
    if (correct_ratio == 1.0)      postfix = ". Perfect!";
    else if (correct_ratio < 1.0)  postfix = "you have done great!";
    else if (correct_ratio < 0.8)  postfix = "you have done quite good!";
    else if (correct_ratio < 0.6)  postfix = "you are half way here!";
    else if (correct_ratio < 0.4)  postfix = "keep practicing!";
    else if (correct_ratio < 0.2)  postfix = "you can do better!";
    else if (correct_ratio == 0.0) postfix = "did you even try?";
    else postfix = ". Wait what? How that`s even possible?";

    wprintw(
        active_window,
        "\nYour accuracy was %.1f percent, %s",
        correct_ratio*100,
        postfix
    );
    wprintw(
        active_window,
        "\nYour average speed was %d words per minute",
        wpm
    );
    wrefresh(active_window);
}

void offer_next_screen(WINDOW* active_window)
{
    wprintw(active_window, "\n(Type in anything to continue.)");
    wrefresh(active_window);
    getch();
    clear();
}

void test(WINDOW* active_window)
{
    int symobols_in_exercise = 47;
    int word_length = 5;

    for (int i = 1; i < 16; i++)
    {
        struct TimeHolder timer;
        gettimeofday(&timer.start, NULL);

        float ratio = gibberish_words_exercise(
            active_window,
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

        print_how_good_user_was(active_window, ratio, user_speed_wpm);
        offer_next_screen(active_window);
    }
}

int main()
{
    srand(time(NULL));

    initscr();                      // Starts ncurses

    WINDOW* in_game_window = newwin(
        IN_GAME_WINDOW_HEIGHT,
        IN_GAME_WINDOW_WIDTH,
        0,
        0
    );

    cbreak();                       // Makes typed stuff immediately available
    noecho();                       // Keyboard input not printed
    scrollok(stdscr, FALSE);        // Scroll disabled
    keypad(in_game_window, TRUE);   // Getch returns special stuff from arrows

    start_color();                  // Enables colors
    init_pair(RED_BACKGROUND_NUM, COLOR_BLACK, COLOR_RED);
    init_pair(GREEN_BACKGROUND_NUM, COLOR_BLACK, COLOR_GREEN);

    test(in_game_window);

    delwin(in_game_window);
    endwin();                       // Ends ncurses mode

    return 0;
}
