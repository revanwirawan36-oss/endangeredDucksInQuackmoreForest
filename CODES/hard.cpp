#include <ncurses/curses.h>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <iostream> //utk string
#include <fstream> 
#include "hard.h"
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
using namespace std;

#define JUMLAH_BEBEK 12
#define GRID_BARIS 12
#define GRID_KOLOM 15 
#define MAX_HITS JUMLAH_BEBEK 

#define BLUE_ICON 1     // o&
#define RED_ICON 2      // <o
#define GREEN_ICON 3    // o>
#define YELLOW_ICON 4   // o|
#define MAGENTA_ICON 5  // +o
#define CYAN_ICON 6     // 0o
#define HIT_COLOR 7     
#define MISS_COLOR 8    
#define levelGame 1

struct Bebek {
    int baris;
    int kolom;
    char simbol[3];  
    int dx;         
    int dy;         
    bool hidup;
};

//wrap logic jaga biar ga out of bound
int wrapKolom(int k) {
    int max_k = GRID_KOLOM;
    return (k % max_k + max_k) % max_k;
}

int wrapBaris(int b) {
    int max_b = GRID_BARIS;
    return (b % max_b + max_b) % max_b;
}

void mulaiGameHard() {
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(0);
    
    string namaPemain1, namaPemain2;
    char buffer1[16]; // cstyle
    char buffer2[16];

    int input_h = 10;
    int input_w = 40;
    int input_y = (LINES - input_h) / 2;
    int input_x = (COLS - input_w) / 2;

    WINDOW *inputWin = newwin(input_h, input_w, input_y, input_x);
    box(inputWin, 0, 0);

    //input Nama Pemain 1
    mvwprintw(inputWin, 2, 2, "Masukkan Nama Pemain 1:");
    mvwprintw(inputWin, 3, 2, " > ");
    wrefresh(inputWin);
    
    echo(); //biar liat apa yang diketik
    mvwgetnstr(inputWin, 3, 5, buffer1, 15); curs_set(0);
    noecho();

    namaPemain1 = buffer1;
    
    mvwprintw(inputWin, 5, 2, "Masukkan Nama Pemain 2:");
    mvwprintw(inputWin, 6, 2, " > ");
    wrefresh(inputWin);
    
    echo();
    mvwgetnstr(inputWin, 6, 5, buffer2, 15); curs_set(0);
    noecho();

    namaPemain2 = buffer2;

    delwin(inputWin);
    curs_set(0); 
    clear(); refresh();
   
    //inisialisasi warna
    if (has_colors()) {
        start_color();
        init_pair(BLUE_ICON, COLOR_BLUE, COLOR_BLACK); 
        init_pair(RED_ICON, COLOR_RED, COLOR_BLACK);
        init_pair(GREEN_ICON, COLOR_GREEN, COLOR_BLACK);
        init_pair(YELLOW_ICON, COLOR_YELLOW, COLOR_BLACK);
        init_pair(MAGENTA_ICON, COLOR_MAGENTA, COLOR_BLACK);
        init_pair(CYAN_ICON, COLOR_CYAN, COLOR_BLACK);
        
        init_pair(HIT_COLOR, COLOR_GREEN, COLOR_WHITE); // kena
        init_pair(MISS_COLOR, COLOR_RED, COLOR_WHITE);  // gakena
    }
    
    refresh(); 
    srand(time(NULL));

    //window game
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
    int notif_h = 7;
    int info_h = term_h - skor_h - notif_h - 3; 

    WINDOW *gridWin  = newwin(grid_h, grid_w, grid_y_start, grid_x_start);
    WINDOW *skorWin  = newwin(skor_h, kanan_w, 1, kanan_x);
    WINDOW *notifWin = newwin(notif_h, kanan_w, 1 + skor_h, kanan_x);
    WINDOW *infoWin  = newwin(info_h, kanan_w, 1 + skor_h + notif_h, kanan_x);

    //border
    box(gridWin, 0, 0);
    box(skorWin, 0, 0);
    box(notifWin, 0, 0);
    box(infoWin, 0, 0);

    mvwprintw(skorWin, 1, 2, "SKOR");
    mvwprintw(skorWin, 3, 2, "%s : 0", namaPemain1.c_str());
    mvwprintw(skorWin, 4, 2, "%s : 0", namaPemain2.c_str()); 

    mvwprintw(notifWin, 1, 2, "NOTIFIKASI");
    //penjelasan bebek
    mvwprintw(infoWin, 1, 2, "PENJELASAN IKON (12 Bebek)");
    // o&
    wattron(infoWin, COLOR_PAIR(BLUE_ICON)); 
    mvwprintw(infoWin, 3, 2, "o&"); 
    wattroff(infoWin, COLOR_PAIR(BLUE_ICON));
    mvwprintw(infoWin, 3, 5, " = 2 kanan, 1 atas");
    // <o
    wattron(infoWin, COLOR_PAIR(RED_ICON)); 
    mvwprintw(infoWin, 4, 2, "<o"); 
    wattroff(infoWin, COLOR_PAIR(RED_ICON));
    mvwprintw(infoWin, 4, 5, " = 1 kiri, 3 atas");
    // o>
    wattron(infoWin, COLOR_PAIR(GREEN_ICON)); 
    mvwprintw(infoWin, 5, 2, "o>"); 
    wattroff(infoWin, COLOR_PAIR(GREEN_ICON));
    mvwprintw(infoWin, 5, 5, " = 3 kanan");
    // o|
    wattron(infoWin, COLOR_PAIR(YELLOW_ICON)); 
    mvwprintw(infoWin, 6, 2, "o|"); 
    wattroff(infoWin, COLOR_PAIR(YELLOW_ICON));
    mvwprintw(infoWin, 6, 5, " = 2 kiri, 2 bawah");
    // +o
    wattron(infoWin, COLOR_PAIR(MAGENTA_ICON)); 
    mvwprintw(infoWin, 7, 2, "+o"); 
    wattroff(infoWin, COLOR_PAIR(MAGENTA_ICON));
    mvwprintw(infoWin, 7, 5, " = 3 kanan, 2 bawah");
    // 0o
    wattron(infoWin, COLOR_PAIR(CYAN_ICON)); 
    mvwprintw(infoWin, 8, 2, "0o"); 
    wattroff(infoWin, COLOR_PAIR(CYAN_ICON));
    mvwprintw(infoWin, 8, 5, " = 2 kiri, 4 bawah");

    mvwprintw(infoWin, 10, 2, "PENJELASAN HANYA SEKALI PADA SAAT FASE MEMORI.");

    wrefresh(skorWin);
    wrefresh(infoWin);

    //koordinat 
    for (int r = 0; r < GRID_BARIS; r++) {
        int y_label = grid_y_start + 1 + r * 2; 
        mvprintw(y_label, 0, "%2d", r + 1); 
    }

    for (int c = 0; c < GRID_KOLOM; c++) {
        int x_label = grid_x_start + 1 + (c * cell_w) + (cell_w / 2);
        mvprintw(grid_y_start - 1, x_label, "%c", 'A' + c); //logika ascii character berdasarkan int
    }
    
    refresh(); 

    //deklarasi
    Bebek bebek[JUMLAH_BEBEK];
    Bebek initial_bebek[JUMLAH_BEBEK]; 
    bool reveal_used = false;

    //bebek bebek dan pergerakannya
    const char* simbolList_str[6] = {"o&","<o","o>","o|","+o","0o"}; 
    int dxList[6] = {2, -1, 3, -2, 3, -2};
    int dyList[6] = {-1, -3, 0, 2, 2, 4}; 

    //copy paste bebek
    for (int i = 0; i < JUMLAH_BEBEK; i++) {
        int type_index = i % 6; 
        strcpy(bebek[i].simbol, simbolList_str[type_index]); //masukkin simbollist_str ke bebek[i].simbol
        bebek[i].dx = dxList[type_index];
        bebek[i].dy = dyList[type_index];
        bebek[i].hidup = true;
        bebek[i].baris = rand() % GRID_BARIS; //randomizeeee
        bebek[i].kolom = rand() % GRID_KOLOM;

        initial_bebek[i] = bebek[i]; //simpan kondisi awal semua bebek, untuk ditampilkan di fase memori dan r
    }
    //auto biar tipe functionnya menyesuaikan, lambda biar pass by reference dan akses semua variable yg udh dibuat sblmnya
    auto gambarGrid = [&](int reveal_mode, int sel_r = -1, int sel_c = -1, bool hit_box = false, bool highlight_shot = false) {
        Bebek *duck_list = (reveal_mode == 3) ? initial_bebek : bebek;
        
        werase(gridWin);
        box(gridWin, 0, 0);

        for (int r = 0; r < GRID_BARIS; r++) {
            int y1 = 1 + r * 2;
            int y2 = y1 + 1;

            for (int c = 0; c < GRID_KOLOM; c++) {
                int x = 1 + c * cell_w;

                bool highlight_cursor = (r == sel_r && c == sel_c && reveal_mode <= 1);
                bool highlight_shoot_box = (r == sel_r && c == sel_c && highlight_shot);
                
                int color_pair_icon = 0;
            
                const char* isi = " "; //untuk simbol, defaultny space

                for (int b = 0; b < JUMLAH_BEBEK; b++) {
                    bool is_duck_visible = (reveal_mode == 3) || duck_list[b].hidup;

                    if (is_duck_visible && duck_list[b].baris == r && duck_list[b].kolom == c) {
                        if (reveal_mode == 1 || reveal_mode == 3 || (reveal_mode == 2 && r == sel_r && c == sel_c)) {
                            isi = duck_list[b].simbol;
                            //set color pair sesuai simbol
                            if (strcmp(isi, "o&") == 0) color_pair_icon = BLUE_ICON;
                            else if (strcmp(isi, "<o") == 0) color_pair_icon = RED_ICON;
                            else if (strcmp(isi, "o>") == 0) color_pair_icon = GREEN_ICON;
                            else if (strcmp(isi, "o|") == 0) color_pair_icon = YELLOW_ICON;
                            else if (strcmp(isi, "+o") == 0) color_pair_icon = MAGENTA_ICON;
                            else if (strcmp(isi, "0o") == 0) color_pair_icon = CYAN_ICON;
                        }
                        break; 
                    }
                }
                
                if (highlight_cursor) wattron(gridWin, A_REVERSE);
                
                //klo lo shot
                if (highlight_shoot_box) {
                    if (hit_box) {
                        wattron(gridWin, COLOR_PAIR(HIT_COLOR) | A_BOLD); 
                    } else {
                        wattron(gridWin, COLOR_PAIR(MISS_COLOR) | A_BOLD);
                    }
                }
                
                //idupin warana ikon kalo ada bebek
                if (strcmp(isi, " ") != 0 && color_pair_icon != 0 && has_colors() && !highlight_shoot_box) {
                    wattron(gridWin, COLOR_PAIR(color_pair_icon));
                }
                
                //nyetak isi box
                mvwprintw(gridWin, y1, x, "[%s%*s]", isi, (int)(3 - strlen(isi)), " ");
                mvwprintw(gridWin, y2, x, "-----");

                //matiin warna ikon
                if (strcmp(isi, " ") != 0 && color_pair_icon != 0 && has_colors() && !highlight_shoot_box) {
                    wattroff(gridWin, COLOR_PAIR(color_pair_icon));
                }
                
                //matiin highlight shot kalo tadi ngeshot
                if (highlight_shoot_box) {
                    if (hit_box) {
                        wattroff(gridWin, COLOR_PAIR(HIT_COLOR) | A_BOLD);
                    } else {
                        wattroff(gridWin, COLOR_PAIR(MISS_COLOR) | A_BOLD);
                    }
                }
                
                if (highlight_cursor) wattroff(gridWin, A_REVERSE);
            }
        }

        wrefresh(gridWin);
    };

    //fase memori 8 menit
    gambarGrid(1); 
    
    int durasi_detik = 8 * 60;
    int ch;
    
    nodelay(stdscr, TRUE); //biar tetep jalan walopun ada getch

    for (int i = durasi_detik; i >= 0; i--){
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
        
        if (i > 0) { //kalo dh 0 gausah napms 
            napms(1000);
        }
    }
    curs_set(0);
    nodelay(stdscr, FALSE); 

    werase(infoWin);
    delwin(infoWin);
    infoWin = NULL; 
    
    int skor_notif_y = 1 + skor_h; 
    for(int r = skor_notif_y; r < term_h; r++) {
        mvhline(r, kanan_x, ' ', kanan_w); //apus
    }
    //buat ulang notif win
    notifWin = newwin(term_h - skor_notif_y - 1, kanan_w, skor_notif_y, kanan_x);
    box(notifWin, 0, 0);
    refresh();

    werase(notifWin);
    box(notifWin, 0, 0);
    mvwprintw(notifWin, 1, 2, "NOTIFIKASI");
    
    //pas fase memori selesai
    mvwprintw(notifWin, 3, 2, "Fase Memori Selesai!");
    mvwprintw(notifWin, 4, 2, "Ikon bergerak ke posisi awal game...");
    wrefresh(notifWin);

    for (int i = 0; i < JUMLAH_BEBEK; i++) {
        bebek[i].kolom = wrapKolom(bebek[i].kolom + bebek[i].dx);
        bebek[i].baris = wrapBaris(bebek[i].baris + bebek[i].dy); //harus wrap ya biar ga out of bound
    }
    
    napms(2000); 

    //gameplay
    int pemain = 1;
    int skor1 = 0, skor2 = 0;
    int sel_r = 0, sel_c = 0;

    int totalTembakan = 0;

    gambarGrid(0, sel_r, sel_c); 

    werase(notifWin);
    box(notifWin, 0, 0);
    mvwprintw(notifWin, 1, 2, "NOTIFIKASI");
    mvwprintw(notifWin, 3, 2, "Giliran %s", namaPemain1.c_str()); // Tampilkan nama pemain 1
    mvwprintw(notifWin, 4, 2, "Tembakan ke-1 (Maks. 14)");
    mvwprintw(notifWin, 5, 2, "Tekan R untuk Reveal Initial (1x)");
    wrefresh(notifWin);

    while ((ch = getch()) != 27) {
    
        if (ch == KEY_UP) {
            sel_r--;
            sel_r = wrapBaris(sel_r);
        } 
        else if (ch == KEY_DOWN) {
            sel_r++;
            sel_r = wrapBaris(sel_r);
        }
        else if (ch == KEY_LEFT) {
            sel_c--;
            sel_c = wrapKolom(sel_c);
        }
        else if (ch == KEY_RIGHT) {
            sel_c++;
            sel_c = wrapKolom(sel_c);
        }

        //reveal initial
        else if ((ch == 'r' || ch == 'R') && !reveal_used) {
            reveal_used = true;
            
            werase(notifWin);
            box(notifWin, 0, 0);
            mvwprintw(notifWin, 1, 2, "NOTIFIKASI");
            mvwprintw(notifWin, 3, 2, "REVEAL INITIAL 15 detik!");
            mvwprintw(notifWin, 4, 2, "Hafalkan posisi awal Bebek!");
            wrefresh(notifWin);

            gambarGrid(3); // Mode 3: Reveal Initial Position

            // Tunggu 15 detik (15000 ms)
            nodelay(stdscr, TRUE);
            time_t start_time = time(NULL);
            while (time(NULL) - start_time < 15) {
                 if (getch() != ERR) break; //agar bisa ngecek inputan dan keluar lebih cepat (dcek every 100ms)
                 napms(100); 
            }
            curs_set(0);
            nodelay(stdscr, FALSE);

            //kembali ke persembunyian
            gambarGrid(0, sel_r, sel_c);
            
            //update notif 
            werase(notifWin);
            box(notifWin, 0, 0);
            mvwprintw(notifWin, 1, 2, "NOTIFIKASI");
            mvwprintw(notifWin, 3, 2, "Giliran %s", (pemain == 1 ? namaPemain1.c_str() : namaPemain2.c_str()));
            mvwprintw(notifWin, 4, 2, "Tembakan ke-%d (Maks. 14)", totalTembakan + 1);
            mvwprintw(notifWin, 5, 2, "Reveal sudah digunakan.");
            wrefresh(notifWin);
        }

        //tembakkk
        else if (ch == KEY_ENTER || ch == '\n') {
            PlaySound(TEXT("C:\\projekUap\\shot.wav"), NULL, SND_FILENAME | SND_ASYNC);
            // cek hit
            int hits_indices[MAX_HITS]; //array utk sipen indeks bebek yan kena
            int hits_count = 0;

            for(int i = 0; i < JUMLAH_BEBEK; i++) {
                if (bebek[i].hidup && bebek[i].baris == sel_r && bebek[i].kolom == sel_c) {
                    hits_indices[hits_count++] = i;
                    if (hits_count >= MAX_HITS) break;
                }
            }

            bool kena = (hits_count > 0);
            
            //update skor
            if (kena) {
                if (pemain == 1) skor1 += hits_count;
                else skor2 += hits_count;
            }

            //update skor window
            werase(skorWin);
            box(skorWin, 0, 0);
            mvwprintw(skorWin, 1, 2, "SKOR");
            mvwprintw(skorWin, 3, 2, "%s : %d", namaPemain1.c_str(), skor1);
            mvwprintw(skorWin, 4, 2, "%s : %d", namaPemain2.c_str(), skor2);
            wrefresh(skorWin);

            //highlight tembakan
            gambarGrid(0, sel_r, sel_c, kena, true); 
            
            werase(notifWin);
            box(notifWin, 0, 0);
            mvwprintw(notifWin, 1, 2, "NOTIFIKASI");
            if (kena) {
                mvwprintw(notifWin, 3, 2, "HIT! (%d Bebek)", hits_count);
                mvwprintw(notifWin, 4, 2, "HIGHLIGHT HIJAU: 3.0 detik...");
            } else {
                mvwprintw(notifWin, 3, 2, "MISS! Kotak kosong.");
                mvwprintw(notifWin, 4, 2, "HIGHLIGHT MERAH: 3.0 detik...");
            }
            wrefresh(notifWin);

            napms(3000); 

            //reveal kalo kena
            
            if (kena) {
                PlaySound(TEXT("C:\\projekUap\\bebek.wav"), NULL, SND_FILENAME | SND_ASYNC);
                gambarGrid(2, sel_r, sel_c, kena, false); 
                mvwprintw(notifWin, 4, 2, "REVEAL: 2.0 detik...");
                mvwprintw(notifWin, 5, 2, "Bebek bergerak: 2.0 detik...");
                wrefresh(notifWin);
                napms(2000); 
            
                for (int i = 0; i < hits_count; i++) {
                    bebek[hits_indices[i]].hidup = false;
                }
            } else {
                gambarGrid(0, sel_r, sel_c); 
                 mvwprintw(notifWin, 4, 2, "Bebek bergerak: 2.0 detik...");
                 wrefresh(notifWin);
                 napms(2000); 
            }
            

            //pergerakan bebek setelah tembakan
            for (int i = 0; i < JUMLAH_BEBEK; i++) {
                if (!bebek[i].hidup) continue;

                bebek[i].kolom = wrapKolom(bebek[i].kolom + bebek[i].dx);
                bebek[i].baris = wrapBaris(bebek[i].baris + bebek[i].dy);
            }

            totalTembakan++;
            
            //kembali ke grid tersembunyi
            gambarGrid(0, sel_r, sel_c);

            //gilirannya ganti
            pemain = (pemain == 1 ? 2 : 1);

            //update notif
            werase(notifWin);
            box(notifWin, 0, 0);
            mvwprintw(notifWin, 1, 2, "NOTIFIKASI");
            mvwprintw(notifWin, 3, 2, "Giliran %s", (pemain == 1 ? namaPemain1.c_str() : namaPemain2.c_str()));
            mvwprintw(notifWin, 4, 2, "Tembakan ke-%d (Maks. 14)", totalTembakan + 1);
            if (!reveal_used) {
                 mvwprintw(notifWin, 5, 2, "Tekan R untuk Reveal Initial (1x)");
            } else {
                 mvwprintw(notifWin, 5, 2, "Reveal sudah digunakan.");
            }
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
        gambarGrid(0, sel_r, sel_c);
    }

    if(ch != 27){
    //klo game end
    werase(notifWin);
    box(notifWin, 0, 0);
    mvwprintw(notifWin, 1, 2, "--- GAME SELESAI ---");
    PlaySound(TEXT("C:\\projekUap\\end.wav"), NULL, SND_FILENAME | SND_ASYNC);
    string pemenangNama;
    if (skor1 > skor2) {
        pemenangNama = namaPemain1;
    mvwprintw(notifWin, 3, 2, "Pemenang: %s!", namaPemain1.c_str());
    } else if (skor2 > skor1) {
        pemenangNama = namaPemain2;
    mvwprintw(notifWin, 3, 2, "Pemenang: %s!", namaPemain2.c_str());
    } else {
        pemenangNama = "Seri";
    mvwprintw(notifWin, 3, 2, "Hasil: Seri!");
    }
    mvwprintw(notifWin, 4, 2, "Skor Akhir:");
    mvwprintw(notifWin, 5, 2, "%s = %d | %s = %d (Total Bebek: %d)", namaPemain1.c_str(), skor1, namaPemain2.c_str(), skor2, skor1 + skor2);
    mvwprintw(notifWin, 6, 2, "Tekan tombol apa saja untuk kembali...");
    wrefresh(notifWin);

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
    
    delwin(gridWin);
    delwin(skorWin);
    delwin(notifWin);

    endwin();
}
