#include<ncurses/curses.h>
#include "rules.h"

void showRules() {
    curs_set(0);
    clear(); refresh();

    const char *rulesText =
    "Aturan Permainan:\n"
    "\n"
    "* Permainan menggunakan grid berukuran 15 x 12.\n"
    "* Beberapa bebek bergerak secara tak terlihat di dalam grid.\n"
    "* Bebek bergerak dalam garis lurus (horizontal, vertikal, diagonal).\n"
    "* Pemain tidak dapat melihat posisi bebek saat mereka bergerak.\n"
    "* Pemain memilih kotak untuk ditembak.\n"
    "* Tujuan: mengenai sebanyak mungkin bebek.\n"
    "* Permainan berakhir ketika semua bebek tertembak atau \n  kedua pemain sudah menggunakan jatah tembak.\n"
    "* Bebek tidak dapat keluar dari grid. Jika mencapai batas:\n"
    "    - Jika naik ke atas -> muncul kembali dari bawah.\n"
    "    - Jika ke kanan     -> muncul dari kiri.\n"
    "    - Jika ke kiri      -> muncul dari kanan.\n"
    "    - Jika turun        -> muncul dari atas.\n"
    "\n"
    "               (Tekan apapun untuk keluar)\n";

    int win_tinggi = 23;
    int win_lebar  = 80;

    int starty = (LINES - win_tinggi) / 2;
    int startx = (COLS  - win_lebar)  / 2;

    WINDOW *win = newwin(win_tinggi, win_lebar, starty, startx);
    box(win, 0, 0);
    keypad(win, TRUE);

    mvwprintw(win, 1, (win_lebar - 5) / 2, "%s", "RULES");

    int y = 3;
    const char *p = rulesText;
    char line[200];

    //print baris per baris
    while (*p) {
        int i = 0;
        while (*p && *p != '\n') line[i++] = *p++; //masukin karakter ke line sampe ketemu \n atau newline
        line[i] = '\0';
        p++;

        mvwprintw(win, y++, 2, "%s", line);
    }

    wrefresh(win);
    wgetch(win);
    delwin(win);
}





