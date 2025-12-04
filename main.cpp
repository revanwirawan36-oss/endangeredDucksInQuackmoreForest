#include <ncurses/curses.h>
#include "story.h"
#include "rules.h"
#include "menu.h"
#include "easy.h" 
#include "hard.h" 
#include "loading.h"
#include "leaderboard.h"
using namespace std;

int main() {
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    start_color();
    curs_set(0);

    while (1>0) {
        clear();
        refresh();
        const char *mainItems[4] = { "Play", "Story", "Rules", "Quit" };
        int mainChoice = drawMenuWindow("MAIN MENU", mainItems, 4);
        if (mainChoice == 0) {
            const char *playItems[2] = { "Easy", "Hard" }; 
            int playChoice = drawMenuWindow("PLAY MENU", playItems, 2);
            if (playChoice == 0) 
                clear();
                refresh();
                tampilkanLoadingBebek();
                clear(); refresh();
                mulaiGameEasy();
            }
            else if (playChoice == 1) {
                clear();
                refresh();
                tampilkanLoadingBebek();
                clear(); refresh();
                mulaiGameHard(); 
            }
        }
        else if (mainChoice == 1) {
            clear(); refresh();
            tampilkanLoadingBebek();
            mainCerita(); 
        }
        else if (mainChoice == 2) {
            showRules();
        }
        else if (mainChoice == 3 || mainChoice == -1) {
            break;
        }
    }
    endwin();
    return 0;
}
