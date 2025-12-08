
# 💻 Workflow Internal Kode `hard.cpp` (Game Bebek Bersembunyi)

Ini adalah analisis langkah demi langkah mengenai cara kerja (alur fungsi dan logika) dari kode sumber C++ `hard.cpp`, yang dirancang untuk implementasi game konsol menggunakan *library* `ncurses`.

## 1\. ⚙️ Inisialisasi dan Konfigurasi Awal

Program dimulai dan dieksekusi melalui fungsi utama `mulaiGameHard()`.

### A. Persiapan Lingkungan & Input

1.  **Pengaturan Konsol (`ncurses`)**:
      * `initscr()`, `noecho()`, `cbreak()`: Mengaktifkan mode layar ncurses, mematikan tampilan input (*noecho*), dan memproses input segera (*cbreak*).
      * `keypad(stdscr, TRUE)`: Mengaktifkan pemrosesan tombol fungsi (Panah, Enter).
      * `curs_set(0)`: Menyembunyikan kursor.
2.  **Input Nama Pemain**:
      * `WINDOW *inputWin = newwin(...)`: Membuat *window* sementara untuk input.
      * `echo()` dan `mvwgetnstr()`: Digunakan untuk mengambil input nama pemain (maks. 15 karakter) ke *buffer* C-style, kemudian dikonversi ke `std::string`.
      * `delwin(inputWin)` dan `noecho()`: Membersihkan *window* input dan mengembalikan mode *noecho*.
3.  **Inisialisasi Warna**:
      * `if (has_colors())`: Memeriksa ketersediaan warna.
      * `start_color()` dan `init_pair()`: Mendefinisikan 8 pasangan warna, termasuk ikon bebek (`BLUE_ICON` hingga `CYAN_ICON`), **HIT** (`HIT_COLOR`), dan **MISS** (`MISS_COLOR`).

### B. Setup Tampilan Window

  * **Deklarasi Windows**: Program membuat dan mengatur posisi 4 *window* utama: `gridWin`, `skorWin`, `notifWin`, dan `infoWin` berdasarkan konstanta ukuran terminal.
  * **Koordinat Grid**: Label A-O dan 1-12 dicetak ke layar utama (`stdscr`) untuk memandu pemain.

### C. Penempatan Bebek Awal

1.  **Iterasi dan Penentuan Tipe**: *Loop* for mengisi *array* `Bebek bebek[12]`.
      * `int type_index = i % 6`: Menentukan jenis ikon, `dx`, dan `dy` (vektor pergerakan) dari 6 tipe yang tersedia.
2.  **Randomisasi Posisi**: `bebek[i].baris = rand() % GRID_BARIS;` dan `bebek[i].kolom = rand() % GRID_KOLOM;` memberikan posisi awal acak.
3.  **Penyimpanan Posisi Awal**: `initial_bebek[i] = bebek[i];` menyimpan status awal untuk fitur *reveal*.

-----

## 2\. 🎨 Fungsi *Lambda* `gambarGrid` (Rendering Grid)

Fungsi *lambda* ini adalah inti dari visualisasi game. Ia bertanggung jawab penuh untuk menggambar grid, ikon, dan *highlight*.

| Parameter | Tipe | Deskripsi |
| :--- | :--- | :--- |
| `reveal_mode` | `int` | **0**: Tersembunyi (Gameplay). **1**: Tampilkan semua (Fase Memori). **2**: Tampilkan bebek yang kena (Fase *Hit*). **3**: Tampilkan posisi awal (`initial_bebek`). |
| `sel_r, sel_c` | `int` | Posisi kursor saat ini (Baris, Kolom). |
| `hit_box` | `bool` | Status tembakan (True: Hit, False: Miss). |
| `highlight_shot` | `bool` | True jika kotak harus di-*highlight* hasil tembakan (Merah/Hijau). |

### Logika Rendering

1.  **Penentuan Sumber Data**: `Bebek *duck_list = (reveal_mode == 3) ? initial_bebek : bebek;` memilih antara posisi bebek saat ini atau posisi awal (untuk *reveal initial*).
2.  **Iterasi Grid**: *Loop* bersarang (`r` dan `c`) untuk setiap sel grid (12x15).
3.  **Pengecekan Bebek**: *Loop* dalam mencari bebek (`b`) yang posisinya cocok dengan sel grid saat ini (`r`, `c`) dan statusnya `hidup` (kecuali pada mode 3).
4.  **Penentuan Konten Sel**:
      * Jika bebek ditemukan dan mode memungkinkan (mode 1, 3, atau mode 2 di posisi *cursor*): `isi` diatur ke simbol bebek, dan `color_pair_icon` diatur sesuai simbol.
5.  **Penerapan *Highlight***:
      * **Kursor**: `if (highlight_cursor)` mengaktifkan `A_REVERSE` (warna terbalik).
      * **Tembakan**: `if (highlight_shoot_box)` mengaktifkan `COLOR_PAIR(HIT_COLOR)` (Hijau) atau `COLOR_PAIR(MISS_COLOR)` (Merah).
6.  **Pencetakan**: `mvwprintw(gridWin, ...)` mencetak konten sel (`[%s...]`).

-----

## 3\. 🧠 Fase Memori (Pre-Game)

1.  **Tampilan Awal**: `gambarGrid(1)` menampilkan semua bebek.
2.  **Timer Loop**:
      * `nodelay(stdscr, TRUE)`: Mengatur ncurses ke mode *non-blocking* agar *loop* tetap berjalan meskipun tidak ada input.
      * *Loop* berjalan selama `durasi_detik` (8 menit).
      * **Logika Skip**: `if (ch == 'e' || ch == 'E')` memecah *loop* jika tombol *skip* ditekan.
      * `napms(1000)`: Jeda 1 detik di setiap iterasi.
3.  **Pergerakan Awal**: Setelah *loop* selesai, bebek digerakkan sekali (posisi memori ke posisi game):
    ```cpp
    bebek[i].kolom = wrapKolom(bebek[i].kolom + bebek[i].dx);
    bebek[i].baris = wrapBaris(bebek[i].baris + bebek[i].dy);
    ```
4.  **Penataan Ulang Window**: `infoWin` dihapus dan `notifWin` diperluas ke bawah.

-----

## 4\. 🎯 Fase Permainan (Gameplay Loop)

Program memasuki *loop* utama (`while ((ch = getch()) != 27)`).

### A. Navigasi

  * `if (ch == KEY_UP/DOWN/LEFT/RIGHT)`: Mengubah `sel_r` atau `sel_c`.
  * **Logika *Wrap***: Posisi baru dilewatkan melalui `wrapBaris()` atau `wrapKolom()` untuk memastikan kursor tidak keluar batas.

### B. Fitur *Reveal Initial* (Tombol `R`)

1.  **Cek Status**: `if ((ch == 'r' || ch == 'R') && !reveal_used)`: Hanya diizinkan sekali.
2.  **Tampilan**: `gambarGrid(3)` menampilkan posisi dari `initial_bebek`.
3.  **Timer *Non-Blocking***: Menggunakan `time(NULL)` dan *loop* `while` 15 detik. `nodelay(stdscr, TRUE)` diaktifkan agar input (walaupun diabaikan) tetap dicek, dan jeda 100ms (`napms(100)`) digunakan untuk menjaga responsivitas.
4.  **Akhir *Reveal***: `reveal_used = true;` dan kembali ke `gambarGrid(0)`.

### C. Logika Tembakan (Tombol `KEY_ENTER` / `\n`)

1.  **Pengecekan *Hit***: Mengiterasi `bebek[i]` yang `hidup` dan mencatat indeks bebek yang posisinya cocok (`bebek[i].baris == sel_r && bebek[i].kolom == sel_c`) ke *array* `hits_indices`.
2.  **Update Skor**: `skor1` atau `skor2` ditambah `hits_count`.
3.  **Visualisasi Tembakan (3 Detik)**: `gambarGrid(0, sel_r, sel_c, kena, true)`: Menampilkan *highlight* hasil tembakan (Hijau/Merah) dan jeda 3 detik (`napms(3000)`).
4.  **Reveal Bebek Kena (Jika HIT)**:
      * `PlaySound(...)`: Memainkan suara bebek.
      * `gambarGrid(2, sel_r, sel_c, kena, false)`: Mengungkap simbol bebek di kotak yang ditembak. Jeda 2 detik.
      * **Penghilangan Bebek**: *Loop* for menggunakan `hits_indices` untuk mengatur `bebek[index].hidup = false;`.
5.  **Pergerakan Bebek (2 Detik)**:
      * *Loop* for menggerakkan semua bebek yang *hidup* menggunakan fungsi `wrapKolom()` dan `wrapBaris()`.
      * Jeda 2 detik (`napms(2000)`).
6.  **Ganti Giliran**: `pemain = (pemain == 1 ? 2 : 1);`
7.  **Cek Akhir Game**: `if (totalTembakan >= 14 || semuaMati)` memecah *loop* utama.

-----

## 5\. 🏁 Penyelesaian dan Penyimpanan

Setelah *loop* utama berakhir:

1.  **Penentuan Pemenang**: Logika `if (skor1 > skor2)` menentukan pemenang atau seri.
2.  **Penyimpanan Skor**:
      * `ofstream fileSkor("score.txt", ios::app);`: Membuka *file* dalam mode *append*.
      * `fileSkor << endl << namaPemainX << " " << skorX << " " << levelGame;`: Menulis data skor kedua pemain ke dalam *file*.
3.  **Pembersihan**:
      * `getch()`: Menunggu input pemain sebelum keluar.
      * `delwin(...)`: Menghapus semua *window*.
      * `endwin()`: Mengembalikan konsol ke mode normal (non-ncurses).
