#include <ncurses/curses.h>
#include <cstdlib>
#include <ctime>
#include <cstring> 
#include <windows.h>
#include <mmsystem.h>
#include <fstream>   
#include <iostream>  
#pragma comment(lib, "winmm.lib")
using namespace std;

#define JUMLAH_BEBEK 8
#define GRID_BARIS 12
#define GRID_KOLOM 15 

//pasangan warna
#define BLUE_ICON 1
#define RED_ICON 2
#define GREEN_ICON 3
#define YELLOW_ICON 4
#define HIGHLIGHT_HIT 5 
#define HIGHLIGHT_MISS 6 

struct Bebek {
    int baris;
    int kolom;
    char simbol[4];     
    int dx;         
    int dy;         
    bool hidup;
};

//wrap logic jd ga out of bound
int easyWrapKolom(int k) { 
    int max_k = GRID_KOLOM;
    return (k % max_k + max_k) % max_k;
}

int easyWrapBaris(int b) { 
    int max_b = GRID_BARIS;
    return (b % max_b + max_b) % max_b;
}

void mulaiGameEasy() {
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(0); 

    //inisialisasi warna
    if (has_colors()) {
        start_color();
        //&o
        init_pair(BLUE_ICON, COLOR_BLUE, COLOR_BLACK); 
        //<o
        init_pair(RED_ICON, COLOR_RED, COLOR_BLACK);
        //o>
        init_pair(GREEN_ICON, COLOR_GREEN, COLOR_BLACK);
        //o|
        init_pair(YELLOW_ICON, COLOR_YELLOW, COLOR_BLACK);
        
        //kalo kena dan gakena
        init_pair(HIGHLIGHT_HIT, COLOR_WHITE, COLOR_GREEN); 
        init_pair(HIGHLIGHT_MISS, COLOR_WHITE, COLOR_RED); 
    }
    
    refresh(); 
    srand(time(NULL));

    string namaPemain1, namaPemain2;
    char buffer1[16]; // cstyle
    char buffer2[16];
    int levelGame = 0; 

    int input_h = 10;
    int input_w = 40;
    int input_y = (LINES - input_h) / 2;
    int input_x = (COLS - input_w) / 2;

    WINDOW *inputWin = newwin(input_h, input_w, input_y, input_x);
    box(inputWin, 0, 0);

    //input input
    mvwprintw(inputWin, 2, 2, "Masukkan Nama Pemain 1:");
    mvwprintw(inputWin, 3, 2, " > ");
    wrefresh(inputWin);
    
    echo(); //biar ketikan keliatan
    mvwgetnstr(inputWin, 3, 5, buffer1, 15); curs_set(0);
    noecho(); 

    namaPemain1 = buffer1;
    
    // Input Nama Pemain 2
    mvwprintw(inputWin, 5, 2, "Masukkan Nama Pemain 2:");
    mvwprintw(inputWin, 6, 2, " > ");
    wrefresh(inputWin);
    
    echo();
    mvwgetnstr(inputWin, 6, 5, buffer2, 15); curs_set(0);
    noecho();

    namaPemain2 = buffer2;

    //bersihkan
    wclear(inputWin);
    delwin(inputWin);
    clear();
    refresh();
    curs_set(0); 
    
    //window set up biar kga nabrak nabrak, jd bisa menyesuaikan terminal
    int cell_w = 5;
    int grid_w = GRID_KOLOM * cell_w + 2;
    int grid_h = GRID_BARIS * 2 + 2; 
    
    int term_h = 30;
    int term_w = 130; 

    int grid_y_start = 2;
    int grid_x_start = 3; 

    int kanan_x = grid_w + grid_x_start + 3; 
    int kanan_w = term_w - kanan_x - 1; 

    int skor_h = 7;
    int info_h = 10; 
    int notif_h = term_h - skor_h - info_h - 3; 

    if (notif_h < 8) notif_h = 8; 

    WINDOW *gridWin  = newwin(grid_h, grid_w, grid_y_start, grid_x_start);
    WINDOW *skorWin  = newwin(skor_h, kanan_w, 1, kanan_x);
    WINDOW *infoWin  = newwin(info_h, kanan_w, 1 + skor_h, kanan_x);
    WINDOW *notifWin = newwin(notif_h, kanan_w, 1 + skor_h + info_h, kanan_x);

    //borderr
    box(gridWin, 0, 0);
    box(skorWin, 0, 0);
    box(notifWin, 0, 0);
    box(infoWin, 0, 0);

    mvwprintw(skorWin, 1, 2, "SKOR");
    mvwprintw(skorWin, 3, 2, "%s : 0", namaPemain1.c_str());
    mvwprintw(skorWin, 4, 2, "%s : 0", namaPemain2.c_str());

    mvwprintw(notifWin, 1, 2, "NOTIFIKASI");
    mvwprintw(infoWin, 1, 2, "PENJELASAN IKON (bebek)");
    
    // o& (Biru)
    wattron(infoWin, COLOR_PAIR(BLUE_ICON));
    mvwprintw(infoWin, 3, 2, "o&");
    wattroff(infoWin, COLOR_PAIR(BLUE_ICON));
    mvwprintw(infoWin, 3, 5, " (Biru)       = 2 kanan, 1 atas");
    // <o (Merah)
    wattron(infoWin, COLOR_PAIR(RED_ICON));
    mvwprintw(infoWin, 4, 2, "<o");
    wattroff(infoWin, COLOR_PAIR(RED_ICON));
    mvwprintw(infoWin, 4, 5, " (Merah)      = 1 kiri, 3 atas");
    // o> (Hijau)
    wattron(infoWin, COLOR_PAIR(GREEN_ICON));
    mvwprintw(infoWin, 5, 2, "o>");
    wattroff(infoWin, COLOR_PAIR(GREEN_ICON));
    mvwprintw(infoWin, 5, 5, " (Hijau)      = 3 kanan");
    // o| (Kuning)
    wattron(infoWin, COLOR_PAIR(YELLOW_ICON));
    mvwprintw(infoWin, 6, 2, "o|");
    wattroff(infoWin, COLOR_PAIR(YELLOW_ICON));
    mvwprintw(infoWin, 6, 6, " (Kuning)     = 2 kiri, 2 bawah");
    
    mvwprintw(infoWin, 8, 2, "Ikon bergerak setiap tembakan.");
    mvwprintw(infoWin, 9, 2, "Tekan Enter untuk tembak.");

    wrefresh(skorWin);
    wrefresh(infoWin);

    //untuk pengkoordinatan
    for (int r = 0; r < GRID_BARIS; r++) {
        int y_label = grid_y_start + 1 + r * 2; 
        mvprintw(y_label, 0, "%2d", r + 1); 
    }

    for (int c = 0; c < GRID_KOLOM; c++) {
        int x_label = grid_x_start + 1 + c * cell_w + cell_w / 2;
        mvprintw(grid_y_start - 1, x_label, "%c", 'A' + c);
    }
    
    refresh(); 

    Bebek bebek[JUMLAH_BEBEK];
    const char* simbolList[4] = {"o&","<o","o>","o|"}; 
    int dxList[4] = {2, -1, 3, -2};
    int dyList[4] = {-1, -3, 0, 2};

    for (int i = 0; i < JUMLAH_BEBEK; i++) {
        //salin copyan jadi total 8 bebek
        strcpy(bebek[i].simbol, simbolList[i % 4]); 
        bebek[i].dx = dxList[i % 4];
        bebek[i].dy = dyList[i % 4];
        bebek[i].hidup = true;
        bebek[i].baris = rand() % GRID_BARIS;
        bebek[i].kolom = rand() % GRID_KOLOM;
    }

   //fungsi gambar grid (paling ribet ga boong)
    auto gambarGrid = [&](bool reveal, int sel_r = -1, int sel_c = -1, int highlight_status = 0) {
        werase(gridWin);
        box(gridWin, 0, 0);

        for (int r = 0; r < GRID_BARIS; r++) {
            int y1 = 1 + r * 2;
            int y2 = y1 + 1;

            for (int c = 0; c < GRID_KOLOM; c++) {
                int x = 1 + c * cell_w;

                bool highlight_cursor = (r == sel_r && c == sel_c);
                bool highlight_shoot = (r == sel_r && c == sel_c && highlight_status != 0);
                
                int color_pair = 0; //ini ntar bakal diisi sesuai bebek
                //ikon di sel
                const char* isi = " "; 

                if (reveal) {
                    for (int b = 0; b < JUMLAH_BEBEK; b++) {
                        if (bebek[b].hidup &&
                            bebek[b].baris == r &&
                            bebek[b].kolom == c) {
                            
                            isi = bebek[b].simbol; //ambil simbol
                            //tentuin pasangan warna
                            if (strcmp(isi, "o&") == 0) color_pair = BLUE_ICON;
                            else if (strcmp(isi, "<o") == 0) color_pair = RED_ICON;
                            else if (strcmp(isi, "o>") == 0) color_pair = GREEN_ICON;
                            else if (strcmp(isi, "o|") == 0) color_pair = YELLOW_ICON;
                            break; 
                        }
                    }
                }

                if (highlight_shoot) {
                    int pair_id = (highlight_status == 1) ? HIGHLIGHT_HIT : HIGHLIGHT_MISS;
                    wattron(gridWin, COLOR_PAIR(pair_id));
                }
                
                //hihglight cursor
                if (highlight_cursor && !highlight_shoot) wattron(gridWin, A_REVERSE);
                
                //terapkan warna ikon kalau ada isinya
                if (strcmp(isi, " ") != 0 && color_pair != 0 && has_colors() && !highlight_shoot) {
                    wattron(gridWin, COLOR_PAIR(color_pair));
                }

                //cetak
                mvwprintw(gridWin, y1, x, "[%s%*s]", isi, (int)(3 - strlen(isi)), " ");
                mvwprintw(gridWin, y2, x, "-----");

               //matiin yang tadi diidupin
                if (strcmp(isi, " ") != 0 && color_pair != 0 && has_colors() && !highlight_shoot) {
                    wattroff(gridWin, COLOR_PAIR(color_pair));
                }
                
                if (highlight_cursor && !highlight_shoot) wattroff(gridWin, A_REVERSE);

                if (highlight_shoot) {
                    int pair_id = (highlight_status == 1) ? HIGHLIGHT_HIT : HIGHLIGHT_MISS;
                    wattroff(gridWin, COLOR_PAIR(pair_id));
                }
            }
        }

        wrefresh(gridWin);
    };

    //fase memori (reveal)
    gambarGrid(true); 
    
    int durasi_detik = 8 * 60;
    int ch;
    
    nodelay(stdscr, TRUE); //biar program tetep jalan walopun ngecek inputan

    for (int i = durasi_detik; i >= 0; --i) {
        ch = getch();
        if (ch == 'e' || ch == 'E') {
            i = -1;
            break;
        }

        int menit = i / 60;
        int detik = i % 60;
        
        werase(notifWin);
        box(notifWin, 0, 0);
        mvwprintw(notifWin, 1, 2, "NOTIFIKASI");
        mvwprintw(notifWin, 3, 2, "Fase Memori Bebek:");
        mvwprintw(notifWin, 4, 2, "%02d:%02d tersisa", menit, detik);
        mvwprintw(notifWin, 5, 2, "Tekan E/e untuk SKIP");
        wrefresh(notifWin);
        
        if (i > 0) {
            napms(1000); 
        }
    }
    curs_set(0);
    nodelay(stdscr, FALSE);
    
    werase(notifWin);
    box(notifWin, 0, 0);
    mvwprintw(notifWin, 1, 2, "NOTIFIKASI");
    
    //memori selesai
    mvwprintw(notifWin, 3, 2, "Fase Memori Selesai!");
    wrefresh(notifWin);

    //pergerakan bebek ke posisi awal game
    for (int i = 0; i < JUMLAH_BEBEK; i++) {
        bebek[i].kolom = easyWrapKolom(bebek[i].kolom + bebek[i].dx);
        bebek[i].baris = easyWrapBaris(bebek[i].baris + bebek[i].dy);
    }
    
    //gameplayyyyyy
    int pemain = 1;
    int skor1 = 0, skor2 = 0;
    int sel_r = 0, sel_c = 0;

    int totalTembakan = 0; 

    gambarGrid(false, sel_r, sel_c);

    mvwprintw(notifWin, 3, 2, "Giliran Pemain 1 (%s)", namaPemain1.c_str());
    mvwprintw(notifWin, 4, 2, "Tembakan ke-0 dari 14");
    wrefresh(notifWin);

    while ((ch = getch()) != 27) {

        if (ch == KEY_UP) {
            sel_r--;
            sel_r = easyWrapBaris(sel_r);
        }
        else if (ch == KEY_DOWN) {
            sel_r++;
            sel_r = easyWrapBaris(sel_r);
        }
        else if (ch == KEY_LEFT) {
            sel_c--;
            sel_c = easyWrapKolom(sel_c);
        }
        else if (ch == KEY_RIGHT) {
            sel_c++;
            sel_c = easyWrapKolom(sel_c);
        }
        //kalo enter
        else if (ch == KEY_ENTER ||ch == '\n') {
            bool kena = false;
            PlaySound(TEXT("C:\\projekUap\\shot.wav"), NULL, SND_FILENAME | SND_ASYNC); //play dorr
            for (int i = 0; i < JUMLAH_BEBEK; i++) {
                if (bebek[i].hidup &&
                    bebek[i].baris == sel_r &&
                    bebek[i].kolom == sel_c) {
                    bebek[i].hidup = false;
                    kena = true;
                }
            }

            //hihglight kalo kena
            int highlight_status = kena ? 1 : 2; 
            gambarGrid(true, sel_r, sel_c, highlight_status); 
            napms(3000); 

            //updet skorny sm notif
            werase(notifWin);
            box(notifWin, 0, 0);
            mvwprintw(notifWin, 1, 2, "NOTIFIKASI");
            
            totalTembakan++; 

            if (kena) {
                PlaySound(TEXT("C:\\projekUap\\bebek.wav"), NULL, SND_FILENAME | SND_ASYNC);
                mvwprintw(notifWin, 3, 2, "Tembakan mengenai bebek!");
                if (pemain == 1) skor1++;
                else skor2++;
            } else {
                mvwprintw(notifWin, 3, 2, "Tidak ada bebek di kotak ini.");
            }
            mvwprintw(notifWin, 4, 2, "Tembakan ke-%d dari 14", totalTembakan);
            wrefresh(notifWin);

            //perbaruin skornya
            werase(skorWin);
            box(skorWin, 0, 0);
            mvwprintw(skorWin, 1, 2, "SKOR");
            mvwprintw(skorWin, 3, 2, "%s : %d", namaPemain1.c_str(), skor1);
            mvwprintw(skorWin, 4, 2, "%s : %d", namaPemain2.c_str(), skor2);
            wrefresh(skorWin);

            //reveal current position bebek selama 6 detik
            gambarGrid(true); 
            mvwprintw(notifWin, 5, 2, "Melihat posisi bebek...");
            wrefresh(notifWin);
            napms(6000); 
            
            //gerakin bebek setelah tembakan
            for (int i = 0; i < JUMLAH_BEBEK; i++) {
                if (!bebek[i].hidup) continue;

                bebek[i].kolom = easyWrapKolom(bebek[i].kolom + bebek[i].dx);
                bebek[i].baris = easyWrapBaris(bebek[i].baris + bebek[i].dy);
            }

            //balik ke grid kosong
            gambarGrid(false, sel_r, sel_c);

            pemain = (pemain == 1 ? 2 : 1); //gantian

            //notif giliran
            werase(notifWin);
            box(notifWin, 0, 0);
            mvwprintw(notifWin, 1, 2, "NOTIFIKASI");
            mvwprintw(notifWin, 3, 2, "Giliran %s", (pemain == 1 ? namaPemain1.c_str() : namaPemain2.c_str()));
            mvwprintw(notifWin, 4, 2, "Tembakan ke-%d dari 14", totalTembakan);
            wrefresh(notifWin);

            //cek akhir game
            bool semuaMati = true;
            for (int i = 0; i < JUMLAH_BEBEK; i++)
                if (bebek[i].hidup)
                    semuaMati = false;

            if (totalTembakan >= 14 || semuaMati)
                break;
        }

        //gambar ulang grid
        gambarGrid(false, sel_r, sel_c);
    }

    if(ch!=27){    //end game yeayyy
    werase(notifWin);
    box(notifWin, 0, 0);
    mvwprintw(notifWin, 1, 2, "--- GAME SELESAI ---");
    PlaySound(TEXT("C:\\projekUap\\end.wav"), NULL, SND_FILENAME | SND_ASYNC);
    if (skor1 > skor2) {
        mvwprintw(notifWin, 3, 2, "Pemenang: %s!", namaPemain1.c_str());
    } else if (skor2 > skor1) {
        mvwprintw(notifWin, 3, 2, "Pemenang: %s!", namaPemain2.c_str());
    } else {
        mvwprintw(notifWin, 3, 2, "Hasil: Seri!");
    }
    
    mvwprintw(notifWin, 4, 2, "Skor Akhir:");
    mvwprintw(notifWin, 5, 2, "%s = %d | %s = %d (Total Bebek: %d)", 
              namaPemain1.c_str(), skor1, namaPemain2.c_str(), skor2, skor1 + skor2);
    
    //simpan ke score.txt
    ofstream fileSkor("score.txt", ios::app); 
    if (fileSkor.is_open()) {
        fileSkor << endl << namaPemain1 << " " << skor1 << " " << levelGame; 
        fileSkor << endl << namaPemain2 << " " << skor2 << " " << levelGame; 
        fileSkor.close();
        mvwprintw(notifWin, 7, 2, "Skor berhasil disimpan ke score.txt.");
    } else {
        mvwprintw(notifWin, 7, 2, "ERROR: Gagal membuat/membuka score.txt!");
    }
    
    mvwprintw(notifWin, 8, 2, "Tekan tombol apa saja untuk keluar...");
    wrefresh(notifWin);
    getch();
    }
    //apus
    delwin(gridWin);
    delwin(skorWin);
    delwin(notifWin);
    delwin(infoWin);

    clear(); 
    refresh();

}
