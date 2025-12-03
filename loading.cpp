#include <ncurses/curses.h>
#include <cstdlib>
#include <ctime>
#include <unistd.h> 
#include <cstring> 
#include "loading.h"

#define DUCK_HEIGHT 4
#define DUCK_WIDTH 9 
#define ANIMATION_RANGE 40
#define DELAY_MS 100 


const char* DUCK_FRAME[] = {
    "     __ ",
    " ___( o)>",
    " \\ <_. )",
    "  `---'"
};

void tampilkanLoadingBebek() {
    
    initscr();
    noecho();
    cbreak(); 
    curs_set(0); 

    if (LINES < (DUCK_HEIGHT + 6) || COLS < (ANIMATION_RANGE + 10)) {
        endwin();
        printf("Ukuran terminal terlalu kecil. Minimum %dx%d\n", ANIMATION_RANGE + 10, DUCK_HEIGHT + 6);
        return;
    }

    
    int start_y = (LINES - DUCK_HEIGHT) / 2;
    int start_x = (COLS - ANIMATION_RANGE) / 2; 

    
    mvprintw(start_y - 2, start_x, "Loading...");
    refresh();

    
    char rail[ANIMATION_RANGE + 1];
    memset(rail, '=', ANIMATION_RANGE);
    rail[ANIMATION_RANGE] = '\0';
    mvprintw(start_y + DUCK_HEIGHT, start_x, "%s", rail);

    int ch;

    
    nodelay(stdscr, TRUE); 
    
    
    for (int current_x = start_x; current_x < (start_x + ANIMATION_RANGE - DUCK_WIDTH); current_x++) {
        
        
        int prev_x = current_x - 1;
        if (prev_x >= start_x) {
            for (int r = 0; r < DUCK_HEIGHT; r++) {
                
                mvprintw(start_y + r, prev_x, "         "); 
            }
        }

       
        for (int r = 0; r < DUCK_HEIGHT; r++) {
            mvprintw(start_y + r, current_x, DUCK_FRAME[r]);
        }
        
     
        refresh();
        
       
        getch(); 
        napms(DELAY_MS); 
    }
    
    
    curs_set(1); 
    endwin();
}
/*
int main() {
    tampilkanLoadingBebek();
    return 0;
}
    */
