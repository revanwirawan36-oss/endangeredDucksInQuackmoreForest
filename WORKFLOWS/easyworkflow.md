# 📂 `WorkFlow` Program Game `easy.cpp`

Program ini adalah implementasi sederhana dari game 'menembak bebek tersembunyi' (Duck Hunt Memory Game) menggunakan *library/pustaka* `ncurses` untuk antarmuka berbasis terminal dan `windows.h` / `mmsystem.h` untuk memutar suara di sistem operasi Windows.

## 1\. ⚙️ Struktur Data dan Konstanta

  * **Konstanta Utama:**
      * `JUMLAH_BEBEK` (8): Total bebek yang harus ditembak.
      * `GRID_BARIS` (12) / `GRID_KOLOM` (15): Ukuran papan permainan.
  * **Warna (`init_pair`):** Mendefinisikan pasangan warna untuk ikon bebek dan *highlight* tembakan (kena/melenceng).
  * **Struktur `Bebek`:**
    ```c
    struct Bebek {
        int baris;
        int kolom;      // Posisi di grid
        char simbol[4]; // Ikon, misal "o&"
        int dx;         // Pergeseran kolom (horizontal)
        int dy;         // Pergeseran baris (vertikal)
        bool hidup;     // Status hidup/mati
    };
    ```

## 2\. 📝 Fungsi Pembantu (`easyWrapKolom`, `easyWrapBaris`)

Fungsi ini memastikan pergerakan bebek tidak keluar dari batas *grid* (papan permainan). Jika bebek bergerak melewati batas, ia akan **muncul kembali** dari sisi yang berlawanan (*wrapping*).

  * **Logika *Wrap*:** Menggunakan operator modulus (`%`) untuk membatasi nilai.
    $$result = (k \pmod{max\_k} + max\_k) \pmod{max\_k}$$
    (Penambahan `+ max_k` memastikan hasil tetap positif meskipun $k$ negatif, kemudian dioperasikan modulus lagi.)

## 3\. 🏁 Fungsi Utama (`mulaiGameEasy`)

Ini adalah inti dari seluruh program.

### A. Inisialisasi Terminal & Warna

1.  **`initscr()`/`start_color()`:** Memulai lingkungan `ncurses` dan mengaktifkan dukungan warna.
2.  **`noecho()`/`cbreak()`/`curs_set(0)`:** Mengatur *input* terminal:
      * `noecho()`: Karakter yang diketik tidak ditampilkan.
      * `cbreak()`: *Input* karakter segera diproses tanpa menunggu tombol **Enter**.
      * `curs_set(0)`: Menyembunyikan kursor.

### B. Input Nama Pemain

1.  Program membuat *window* kecil (`inputWin`) di tengah layar.
2.  Menggunakan `echo()` sementara dan `mvwgetnstr` untuk mengambil nama pemain 1 dan pemain 2.
3.  Setelah selesai, `inputWin` dihapus (`delwin`) dan pengaturan *input* dikembalikan ke `noecho()` dan `curs_set(0)`.

### C. Pengaturan *Window* Utama

Program membagi terminal menjadi 4 jendela utama (`WINDOW`):

1.  `gridWin`: Papan permainan (tempat bebek bergerak).
2.  `skorWin`: Menampilkan skor kedua pemain.
3.  `infoWin`: Menampilkan legenda ikon bebek dan pergerakannya.
4.  `notifWin`: Menampilkan notifikasi, giliran, dan waktu.

### D. Inisialisasi Bebek

1.  *Array* `bebek[JUMLAH_BEBEK]` dibuat.
2.  Setiap bebek diinisialisasi dengan:
      * `simbol`, `dx`, `dy`: Diambil dari `simbolList`, `dxList`, dan `dyList` secara berulang (total 8 bebek, 4 jenis pergerakan).
          * **Contoh Logika Ikon/Pergerakan:**
              * `"o&"` (Biru): $\Delta x = 2$ (kanan), $\Delta y = -1$ (atas).
              * `"<o"` (Merah): $\Delta x = -1$ (kiri), $\Delta y = -3$ (atas).
      * `hidup = true`.
      * Posisi awal `baris`/`kolom` diatur secara acak (`rand()`).

### E. 🖼️ Fungsi `gambarGrid` (Logika Kunci Tampilan)

Ini adalah fungsi kompleks (*lambda function* / *auto*) yang bertanggung jawab menggambar seluruh papan:

  * **Parameter:**
      * `reveal`: `true` (tampilkan posisi bebek), `false` (sembunyikan/fase bermain).
      * `sel_r`, `sel_c`: Posisi kursor/tembakan.
      * `highlight_status`: `1` (Hit/Hijau), `2` (Miss/Merah), `0` (Normal).
  * **Logika Tampilan Sel:**
    1.  Cek apakah sel yang sedang digambar adalah posisi kursor (`highlight_cursor`) atau posisi tembakan (`highlight_shoot`).
    2.  **`if (reveal)`:** Cari apakah ada bebek hidup di koordinat sel tersebut. Jika ada, ambil `simbol` dan `color_pair`-nya.
    3.  **Logika Pewarnaan:**
          * `if (highlight_shoot)`: Gunakan warna `HIGHLIGHT_HIT` (Hijau) atau `HIGHLIGHT_MISS` (Merah).
          * `else if (highlight_cursor)`: Gunakan *highlight* `A_REVERSE` (kebalikan warna latar/depan).
          * `else if (bebek ada)`: Gunakan `COLOR_PAIR` bebek (misal, `BLUE_ICON`).

### F. Fase Memori (8 Menit)

1.  **`gambarGrid(true)`:** Bebek ditampilkan di posisi awal.
2.  **`nodelay(stdscr, TRUE)`:** Mengatur *input* agar program tidak menunggu penekanan tombol.
3.  *Loop* berjalan selama `durasi_detik` (480 detik/8 menit) dan diperbarui setiap 1 detik (`napms(1000)`).
4.  Pemain dapat **SKIP** fase ini dengan menekan **E/e** (`if (ch == 'e' || ch == 'E')`).
5.  Setelah waktu habis atau di-skip, bebek **bergerak satu kali** ke posisi awal permainan:
    ```c
    bebek[i].kolom = easyWrapKolom(bebek[i].kolom + bebek[i].dx);
    bebek[i].baris = easyWrapBaris(bebek[i].baris + bebek[i].dy);
    ```

### G. Fase Bermain (Gameplay)

1.  **Inisialisasi:** `pemain = 1`, `skor1/skor2 = 0`, `sel_r/sel_c = 0` (posisi kursor awal), `totalTembakan = 0`.
2.  **`while ((ch = getch()) != 27)`:** *Loop* utama permainan, terus berjalan sampai tombol **ESC (27)** ditekan atau permainan berakhir.
3.  **Logika Gerak Kursor (`if (ch == KEY_...)`):**
      * Tombol **Atas/Bawah/Kiri/Kanan** menggeser `sel_r`/`sel_c`.
      * Fungsi *wrap* (`easyWrapBaris`/`easyWrapKolom`) dipanggil agar kursor tidak keluar batas.
      * Setelah kursor bergerak, **`gambarGrid(false, sel_r, sel_c)`** dipanggil untuk memperbarui posisi kursor (tanpa menampilkan bebek).
4.  **Logika Tembak (`else if (ch == KEY_ENTER || ch == '\n')`):**
      * Memutar suara tembakan (`PlaySound(TEXT("...shot.wav")`).
      * **Pengecekan Kena/Melenceng:**
        ```c
        // Loop ini mencari bebek di posisi (sel_r, sel_c)
        for (int i = 0; i < JUMLAH_BEBEK; i++) {
            if (bebek[i].hidup && bebek[i].baris == sel_r && bebek[i].kolom == sel_c) {
                bebek[i].hidup = false; // Bebek mati
                kena = true;
            }
        }
        ```
      * **Hasil Tembakan (Feedback):**
          * Grid di-gambar ulang dengan *highlight* tembakan (`gambarGrid(true, sel_r, sel_c, highlight_status)`).
          * Program berhenti sejenak (`napms(3000)` = 3 detik) untuk menunjukkan hasilnya.
          * Skor diperbarui, dan notifikasi ditampilkan.
      * **Fase *Reveal* (6 Detik):**
          * `gambarGrid(true)` dipanggil untuk menampilkan posisi semua bebek hidup.
          * Program berhenti sejenak (`napms(6000)` = 6 detik).
      * **Pergerakan Bebek:**
          * Semua bebek yang `hidup` bergerak satu langkah sesuai `dx` dan `dy` mereka.
      * **Giliran Berikutnya:**
          * Grid kembali disembunyikan (`gambarGrid(false, sel_r, sel_c)`).
          * `pemain` diganti (1 menjadi 2, atau 2 menjadi 1).
      * **Cek Akhir Game:**
          * Permainan berakhir jika `totalTembakan >= 14` **ATAU** `semuaMati` (`semuaMati` adalah `true` jika semua bebek `hidup = false`).

### H. Akhir Game

1.  Memutar suara akhir (`PlaySound(TEXT("...end.wav")`).
2.  Menentukan **Pemenang** berdasarkan perbandingan `skor1` dan `skor2`.
3.  **Penyimpanan Skor (`score.txt`):**
      * Menggunakan `ofstream` dengan mode `ios::app` (menambahkan ke akhir file).
      * Format penyimpanan: `[NamaPemain] [Skor] [LevelGame]`.
4.  Program menunggu input tombol dari pemain sebelum keluar (`getch()`).
5.  Semua *window* dihapus (`delwin`) dan `ncurses` diakhiri (`endwin` - meskipun tidak eksplisit di kode yang diunggah, biasanya dilakukan di fungsi terpisah atau bagian akhir program).

-----

## 4\. 🧠 Logika Kunci Tambahan

| Logika | Keterangan |
| :--- | :--- |
| **Gantian Pemain** | `pemain = (pemain == 1 ? 2 : 1);` (Operator Ternary). Jika `pemain` saat ini adalah 1, ganti ke 2; jika tidak, ganti ke 1. |
| **Pembaruan Skor** | `if (kena) { if (pemain == 1) skor1++; else skor2++; }` Hanya pemain yang sedang giliran dan berhasil menembak yang mendapat skor. |
| **`c_str()`** | Digunakan saat mencetak variabel `string` C++ (`namaPemain1`, `namaPemain2`) ke fungsi `mvwprintw` yang merupakan fungsi *style* C. |
