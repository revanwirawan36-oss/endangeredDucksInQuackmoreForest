#include <ncurses/curses.h>
#include <cstdlib>
#include <ctime>
#include <cstring> 
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
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
    PlaySound(TEXT("C:\\projekUap\\loading.wav"), NULL, SND_FILENAME | SND_ASYNC);
    napms(500); //delay suara loading 

    int start_y = (LINES - DUCK_HEIGHT) / 2;
    int start_x = (COLS - ANIMATION_RANGE) / 2; 

    mvprintw(start_y - 2, start_x, "Loading...");
    refresh();

    // buat relny di bawah bebek
    char rail[ANIMATION_RANGE + 1];
    memset(rail, '=', ANIMATION_RANGE); //masukin = ke rel 40 kali
    rail[ANIMATION_RANGE] = '\0';
    mvprintw(start_y + DUCK_HEIGHT, start_x, "%s", rail);

    //antisipasi kepencet keyboard
    int ch;
    curs_set(0);
    nodelay(stdscr, TRUE); 
    
    // loopinh animasi
    for (int current_x = start_x; current_x < (start_x + ANIMATION_RANGE - DUCK_WIDTH); current_x++) {
        int prev_x = current_x - 1; //posisi sblm
        if (prev_x >= start_x) {
            for (int r = 0; r < DUCK_HEIGHT; r++) {
                mvprintw(start_y + r, prev_x, "         "); //apus
            }
        }

        for (int r = 0; r < DUCK_HEIGHT; r++) {
            mvprintw(start_y + r, current_x, DUCK_FRAME[r]);
        }
        
        refresh();
        getch(); 
        napms(DELAY_MS); //delay buat efek animasi
    }

    endwin();
}
