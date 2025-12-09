#include "leaderboard.h"
#include <fstream>
#include <iostream>
#include <cstring>
#include <algorithm> //utk swap krn mgr mk temp
#include <ncurses/curses.h>
using namespace std;

void sortLeaderboard(ScoreEntry leaderboard[], int count) {
    //bubble sort 
    for (int i = 0; i < count - 1; ++i) {
        for (int j = 0; j < count - 1 - i; ++j) {
            if (leaderboard[j].skor < leaderboard[j+1].skor) {
                swap(leaderboard[j], leaderboard[j+1]);
            } 
            //hard diatas easy kalo sama
            else if (leaderboard[j].skor == leaderboard[j+1].skor) {
                 if (leaderboard[j].level < leaderboard[j+1].level) {
                     swap(leaderboard[j], leaderboard[j+1]);
                 }
            }
        }
    }
}

void showLeaderboard() {
    ScoreEntry leaderboard[MAX_ENTRIES];
    int entryCount = 0;
    
    ifstream file("score.txt");
    //baca
    if (file.is_open()) {
        char nama_temp[21];
        int skor_temp;
        int level_temp; 

        while (file >> nama_temp >> skor_temp >> level_temp && entryCount < MAX_ENTRIES) {
            strncpy(leaderboard[entryCount].nama, nama_temp, 20); //c style
            leaderboard[entryCount].nama[20] = '\0'; // c style
            leaderboard[entryCount].skor = skor_temp;
            leaderboard[entryCount].level = level_temp;
            entryCount++;
        }
        file.close();
    } 

    if (entryCount > 0) {
        sortLeaderboard(leaderboard, entryCount); //sort berdasarkan skor dan level
    }

    
    clear(); 
    refresh(); 

    //display leaderboard
    int maxBaris, maxKolom;
    getmaxyx(stdscr, maxBaris, maxKolom);

    int barisKonten = (entryCount > 0) ? entryCount : 1;
    int requiredHeight = 7 + barisKonten; 

    int windowHeight = (maxBaris < requiredHeight) ? maxBaris : requiredHeight; 
    int windowWidth = (maxKolom < 60) ? maxKolom : 60; 

    //ini minimumnya ya klo kekecilan gsh
    if (windowHeight < 5 || windowWidth < 20) return; 

    WINDOW *leaderWindow = newwin(windowHeight, windowWidth, (maxBaris - windowHeight) / 2, (maxKolom - windowWidth) / 2);
    if (!leaderWindow) return; 

    keypad(leaderWindow, TRUE); 
    box(leaderWindow, 0, 0);


    mvwprintw(leaderWindow, 1, (windowWidth - 17) / 2, "** DAFTAR PERINGKAT **");
    mvwhline(leaderWindow, 2, 1, ACS_HLINE, windowWidth - 2); //pembatas


    mvwprintw(leaderWindow, 3, 3, "#");
    mvwprintw(leaderWindow, 3, 10, "NAMA");
    mvwprintw(leaderWindow, 3, 35, "SKOR");
    mvwprintw(leaderWindow, 3, 45, "LEVEL");
    mvwhline(leaderWindow, 4, 1, ACS_HLINE, windowWidth - 2); //pembatas

    int barisAwal = 5;
    if (entryCount == 0) {
    mvwprintw(leaderWindow, barisAwal, (windowWidth - 25) / 2, "Belum ada skor yang tercatat.");
    } else {
        for (int i = 0; i < entryCount; ++i) {
            if (barisAwal + i < windowHeight - 2) {
                ScoreEntry entri = leaderboard[i];
                const char *namaLevel = (entri.level == 0) ? "Mudah" : "Sulit";
            
                mvwprintw(leaderWindow, barisAwal + i, 3, "%d.", i + 1);  //buat nomor
                mvwprintw(leaderWindow, barisAwal + i, 10, "%s", entri.nama);
                mvwprintw(leaderWindow, barisAwal + i, 35, "%d", entri.skor);
                mvwprintw(leaderWindow, barisAwal + i, 45, "%s", namaLevel);
            }
        }
    }

    mvwprintw(leaderWindow, windowHeight - 2, (windowWidth - 25) / 2, "Tekan tombol apa saja untuk kembali...");

    wrefresh(leaderWindow); 

    getch(); 
    getch();

    delwin(leaderWindow);
    clear();
    refresh();

}


