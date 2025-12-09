


//buat menu
int drawMenuWindow(const char *title, const char *items[], int count) {
    clear(); refresh();
    int highlight = 0;
    int ch;

    int win_tinggi = 4 + count * 2 + 2;
    int win_lebar  = 30;

    int starty = (LINES - win_tinggi) / 2;
    int startx = (COLS  - win_lebar)  / 2;
    
    //buat window
    WINDOW *win = newwin(win_tinggi, win_lebar, starty, startx);
    keypad(win, TRUE);

    //kalo main menu, tampilin judul game
    if (strcmp(title, "MAIN MENU") == 0) {
    mvprintw(starty - 3, (COLS - 15) / 2, "CATCH THE DUCK");
    refresh();
    }
    
    //looping menu
    while (1) {
        werase(win);
        box(win, 0, 0);

        //strlen kek .length(); tapi versi c
        mvwprintw(win, 1, (win_lebar - strlen(title)) / 2, "%s",title);

        for (int i = 0; i < count; i++) {
            int posy = 3 + (i * 2);

            if (i == highlight)
                wattron(win, A_REVERSE);

            mvwprintw(win, posy, (win_lebar - strlen(items[i])) / 2, items[i]);

            if (i == highlight)
                wattroff(win, A_REVERSE);
        }

        wrefresh(win);

        ch = wgetch(win);

        if (ch == KEY_UP) {
            highlight--;
            if (highlight < 0)
                highlight = count - 1;
        }
        else if (ch == KEY_DOWN) {
            highlight++;
            if (highlight >= count)
                highlight = 0;
        }
        else if (ch == KEY_ENTER || ch =='\n') {
            delwin(win);
            return highlight;
        }
        else if (ch == 27) { // escape kwy
            delwin(win);
            return -1;
        }
    }
    curs_set(0);
    clear(); refresh();
}

