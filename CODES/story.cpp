#include <ncurses/curses.h>
#include "story.h"
#include <cstring>
#include <ctime>
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
using namespace std;

const int TUNGGU_SETELAH_GAMBAR = 2000; 
const int DELAY_KETIK_MS = 45;         

const int WIN_TINGGI = 6;
const int WIN_LEBAR  = 100;

const int PIC_STARTY = 1;
const int PIC_HEIGHT = 23;
const int PIC_LEBAR  = 100; // lebar art

//buat gambar gambar dulu
const char *gambar_ascii[9] = {
"               *            ___( o)>  ___( o)>    \n"
"              ***           \\ <_. )   \\ <_. )   \n"
"             *****           `---'     `---'      \n"
"            *******                             \n"
"           *********         ___( o)>  ___( o)>  \n"
"          ***********        \\ <_. )   \\ <_. ) \n"
"         *************        `---'     `---'      \n"
"        ***************                          \n"
"       *****************                          \n"
"      *******************                          \n"
"     *********************                       \n"
"    ***********************                        \n"
"   *************************                       \n"
"             |||||                                \n"
"             |||||                              \n"
"             |||||                               \n",

"    __                       __                           __                    \n"
"___( o)>                 ___( o)>                     ___( o)>                 \n"
"\\ <_. )                  \\ <_. )                      \\ <_. )                  \n"
" `---'                    `---'                        `---'                   \n"
"                                                                                \n"
"            __                           __                        __          \n"
"        ___( o)>                     ___( o)>                  ___( o)>        \n"
"         \\ <_. )                     \\ <_. )                   \\ <_. )         \n"
"          `---'                       `---'                     `---'          \n"
"                                                                                 \n"
"                                                                                 \n"
"                                                                                 \n"
"                                                                                 \n"
"                                                                                 \n"
"                                                                                 \n"
"                                                                                 \n"
"                                                                                 \n"
"                                                                                 \n"
,
"Value                                                          \n"
"10 | #                                                         \n"
" 9 | # #                                                     \n"
" 8 | # # #                                                   \n"
" 7 | # # # #                                                    \n"
" 6 | # # # # #                                                  \n"
" 5 | # # # # # #                                                \n"
" 4 | # # # # # # #                                                 \n"
" 3 | # # # # # # # #                                               \n"
" 2 | # # # # # # # # #                                               \n"
" 1 | # # # # # # # # # #                                            \n"
"    +------------------------------------------------         \n"
"      A   B   C   D   E   F   G   H   I   J                   \n"
"                                                                                 \n"
"                                                                                 \n"
"                                                                                 \n"
"                                                                                 \n"
"                                                                                 \n"
,
"    __                       __                              \n"
"___( o)>                 ___( o)>                            \n"
"\\ <_. )                  \\ <_. )                            \n"
" `---'                    `---'                              \n"
"      ()    ()      ()                 ()        ()          \n"
"                                                              \n"
"                               __                             \n"
"                           ___( o)>                           \n"
"                           \\ <_. )                           \n"
"                            `---'                             \n"
"  ()     ()        ()                 ()        ()      ()    \n"
"                                                              \n"
" ()          ()        ()                                     \n"
"                                                                                 \n"
"                                                                                 \n"
"                                                                                 \n"
"                                                                                 \n"
"                                                                                 \n"
,
"   ~        ()          ~              ()        ~           ()       \n"
"        ()        ~            ()           ~        ()              \n"
"   ~            ()       ~            ()        ~                   \n"
"        ~   ()        ~        ()          ~        ()              \n"
"   ()        ~            ()        ~            ()        ~        \n"
"        ~        ()           ~        ()           ~        ()     \n"
"   ~        ()        ~            ()        ~            ()        \n"
"                                                                                 \n"
"                                                                                 \n"
"                                                                                 \n"
"                                                                                 \n"
"                                                                                 \n"
"                                                                                 \n"
"                                                                                 \n"
"                                                                                 \n"
"                                                                                 \n"
"                                                                                 \n"
,
"       __         __                                   \n"
"   ___( o)>   ___( o)>                                \n"
"  //  \\\\     //  \\\\                                   \n"
" <<    >>   <<    >>                                  \n"
"  \\\\__//     \\\\__//                                   \n"
"   `---'      `---'                                   \n"
"                                                      \n"
"            __         __                             \n"
"        ___( o)>   ___( o)>                          \n"
"       //  \\\\     //  \\\\                             \n"
"      <<    >>   <<    >>                            \n"
"       \\\\__//     \\\\__//                             \n"
"        `---'      `---'                             \n"
"                                                                                 \n"
"                                                                                 \n"
"                                                                                 \n"
"                                                                                 \n"
"                                                                                 \n"
, //help pegel ngetik
"             ________                     \n"
"            /        \\                    \n"
"           /  O    O  \\                   \n"
"          |     __     |                  \n"
"          |    (__)    |                  \n"
"           \\          /                   \n"
"            \\________/                    \n"
"               |  |                       \n"
"          _____|  |_____                  \n"
"         /             \\                 \n"
"        /   [====]      \\                \n"
"       |    [====]       |               \n"
"        \\             __/                \n"
"         \\___________/                   \n"
"            |     |                      \n"
"            |     |                      \n"
"            |     |                      \n"
"           /       \\                     \n"
,
"    __                       __                           __                    \n"
"___( o)>                 ___( o)>                     ___( o)>                 \n"
"\\ <_. )                  \\ <_. )                      \\ <_. )                  \n"
" `---'                    `---'                        `---'                   \n"
"                                                                                \n"
"            __                           __                        __          \n"
"        ___( o)>                     ___( o)>                  ___( o)>        \n"
"         \\ <_. )                     \\ <_. )                   \\ <_. )         \n"
"          `---'                       `---'                     `---'          \n"
"                                                                                 \n"
"                                                                                 \n"
"                                                                                 \n"
"                                                                                 \n"
"                                                                                 \n"
"                                                                                 \n"
"                                                                                 \n"
"                                                                                 \n"
"                                                                                 \n"
,

"       *            *            *            *       \n"
"      ***          ***          ***          ***      \n"
"     *****        *****        *****        *****     \n"
"    *******      *******      *******      *******    \n"
"   *********    *********    *********    *********   \n"
"  ***********  ***********  ***********  ***********  \n"
"     |||||        |||||        |||||        |||||     \n"
"     |||||        |||||        |||||        |||||     \n"
"     |||||        |||||        |||||        |||||     \n"
"                                                                                 \n"
"       *            *            *            *       \n"
"      ***          ***          ***          ***      \n"
"     *****        *****        *****        *****     \n"
"    *******      *******      *******      *******    \n"
"   *********    *********    *********    *********   \n"
"  ***********  ***********  ***********  ***********  \n"
"     |||||        |||||        |||||        |||||     \n"
"     |||||        |||||        |||||        |||||     \n"
"     |||||        |||||        |||||        |||||     \n",
};

//tampilin gambar diatas window
void tampilkanGambarStdScr(int id_gambar) {
    if (id_gambar < 0 || id_gambar >= 8) return;

    int kolom_tengah = COLS / 2;
    int basex = kolom_tengah - (PIC_LEBAR / 2);
    if (basex < 0) basex = 0; //klo layar sempit

    // bersihin dl
    for (int r = 0; r < PIC_HEIGHT; r++) {
        move(PIC_STARTY + r, basex);
        for (int c = 0; c < PIC_LEBAR; c++) {
            addch(' ');
        }
    }

    // baru gambar
    const char *s = gambar_ascii[id_gambar]; //ini kt milih gambar yg mana sesuai id_gambar
    const char *p = s; //points ke gambar yg kepilih itu
    int r = 0;
    char baris[128];

    //print baris per baris
    while (*p && r < PIC_HEIGHT) {
        int i = 0;
        while (*p && *p != '\n' && i < 127) {
            baris[i++] = *p++; //masukin char 1 per 1
        }
        baris[i] = '\0'; //null terminator, c style bgt 
        if (*p == '\n') p++; //klo kena \n, majuin p, klo kena \0 loopny berenti
        mvprintw(PIC_STARTY + r, basex, "%s", baris);
        r++;
    }
    //pokoknya setiap p kena \n, loop dalem stop, jadi char baris untuk di line segitu dh kebentuk, 
    //jadi naikin r nya buat pindah ke baris berikutnya (p nya juga naikin, soalnya pas didalem loop p nya ga ikut naik)

    refresh();
}

int tulisAnimasiKeWin(WINDOW *win, int y_mulai, const char *teks) {
    int cur_y = y_mulai;
    int cur_x = 2;
    int maks_y, maks_x;
    getmaxyx(win, maks_y, maks_x);

    const char *p = teks;
    while (*p) {
        if (*p == '\n') { //klo kena \n skip, tp ganti beberapa var
            cur_y++;
            cur_x = 2;
            p++;
            if (cur_y >= maks_y - 1) return maks_y - 1; // klo gacukup berhenti
            continue;
        }
        // tulis karakter
        mvwaddch(win, cur_y, cur_x, *p);
        wrefresh(win);

        cur_x++;
        // klo kena tepi ganti baris
        if (cur_x >= maks_x - 2) {
            cur_y++;
            cur_x = 2; //ulang x lagi
            if (cur_y >= maks_y - 1) return maks_y - 1; // klo gacukup berhenti
        }
        //delay animas ketik
        napms(DELAY_KETIK_MS);
        p++;
    }

    //balikin y posisi next
    return cur_y + 1;
}


// PART 1
const char *part1_segmen_teks[] = {
    "Hutan Quackmur menyimpan rahasia: spesies bebek langka ",
    "yang hanya muncul beberapa kali dalam setahun. Populasinya menurun drastis",
    "dan para peneliti berjuang menyelamatkan mereka melalui program pembiakan"
};
int part1_gambar_before[3]={0,2,3}; //hutan, statistik, bebek+telur
const int part1_count = 3;

// PART 2
const char *part2_segmen_teks[] = {
    "Untuk menjalankan program itu, mereka membutuhkan sampel bulu dan telur, ",
    "tetapi mengambilnya tidak mudah. Bebek-bebek ini bergerak cepat, sulit terlihat, ",
    "dan hanya keluar dari hutan lindung dalam waktu singkat."
};
int part2_gambar_before[3] = {4, 5, 8 }; //bulu, bebekterbang, hutan
const int part2_count = 3;

// PART 3
const char *part3_segmen_teks[] = {
    "Sebagai pemain, kamu adalah anggota baru dalam tim penelitian. ",
    "Tugasmu menangkap bebek tanpa menyakiti mereka, mengumpulkan sampel sebanyak mungkin, ",
    "dan memastikan keberlangsungan hidup spesies yang terancam punah ini."
};
int part3_gambar_before[3] = { 6, 7, 7 }; //peneliti, bebek, bebek
const int part3_count = 3;


void putarPartCerita(WINDOW *win,
                     const char **segmen_teks,
                     const int *gambar_before,
                     int jumlah_segmen)
{
    curs_set(0);
    int posisi_y = 1;

    //bersihkan window cerite
    werase(win);
    box(win, 0, 0); //border
    mvwprintw(win, 0, (WIN_LEBAR - 6) / 2, "CERITA");
    wrefresh(win);

    for (int s = 0; s < jumlah_segmen; s++) {
        //teks
        posisi_y = tulisAnimasiKeWin(win, posisi_y, segmen_teks[s]); /*masukkin ke posisi y, bc the function gonna return 
        posisi y selanjutnya, yang akan digunakan function ini di iterasi setelah ini*/
        //tampilin gmbr
        tampilkanGambarStdScr(gambar_before[s]);
        napms(TUNGGU_SETELAH_GAMBAR);
    }
    //clean window text (bukan stdscr)
    werase(win);
    box(win, 0, 0);
    wrefresh(win);
}

void mainCerita() {
    curs_set(0);
    clear();
    refresh();
    //posisi window
    int starty = LINES - WIN_TINGGI - 2;
    if (starty < 0) starty = 1;
    int startx = (COLS - WIN_LEBAR) / 2;
    if (startx < 0) startx = 1;

    WINDOW *win = newwin(WIN_TINGGI, WIN_LEBAR, starty, startx);
    keypad(win, TRUE);
    box(win, 0, 0);
    mvwprintw(win, 0, (WIN_LEBAR - 6) / 2, "CERITA");
    wrefresh(win);
    
    //part 1
    PlaySound(TEXT("C:\\projekUap\\revan.wav"), NULL, SND_FILENAME | SND_ASYNC);
    napms(500); //delay suara revan
    putarPartCerita(win,
                    part1_segmen_teks,
                    part1_gambar_before, 
                    part1_count);


    //part 2
    PlaySound(TEXT("C:\\projekUap\\ratu.wav"), NULL, SND_FILENAME | SND_ASYNC);
    putarPartCerita(win,
                    part2_segmen_teks,
                    part2_gambar_before,
                    part2_count);


    //part 3
    PlaySound(TEXT("C:\\projekUap\\nasya.wav"), NULL, SND_FILENAME | SND_ASYNC);
    putarPartCerita(win,
                    part3_segmen_teks,
                    part3_gambar_before,
                    part3_count);
                
    mvwprintw(win, 1, 2, "Cerita selesai. Tekan apapun untuk kembali ke menu.");
    wrefresh(win);
    wgetch(win);

    delwin(win);

}

