# 📖  `Workflow` Program `story.cpp`

Dokumen ini menjelaskan alur logika dan cara kerja dari file C++ `story.cpp`, yang bertanggung jawab menampilkan narasi cerita (cutscene) game menggunakan tampilan berbasis teks (`ncurses`) dan efek suara (`winmm.lib`).

---

## 🛠️ Variabel dan Konstanta Utama

| Nama Konstanta | Nilai / Deskripsi | Fungsi |
| :--- | :--- | :--- |
| `TUNGGU_SETELAH_GAMBAR` | `2000` (ms) | Durasi program akan berhenti setelah menampilkan setiap gambar (2 detik). |
| `DELAY_KETIK_MS` | `45` (ms) | Jeda waktu antara penulisan setiap karakter untuk menciptakan efek **animasi mengetik**. |
| `WIN_TINGGI` & `WIN_LEBAR` | `6` & `100` | Ukuran *window* (kotak) tempat teks cerita akan ditampilkan. |
| `gambar_ascii[9]` | Array `const char*` | Menyimpan 9 gambar **ASCII Art** yang akan ditampilkan di layar utama. |
| `partX_segmen_teks[]` | Array `const char*` | Berisi baris-baris teks narasi untuk setiap bagian cerita. |
| `partX_gambar_before[]` | Array `int` | Berisi **ID** gambar (indeks dari `gambar_ascii`) yang akan ditampilkan *sebelum* segmen teks yang sesuai. |

---

## 1. Fungsi `tampilkanGambarStdScr`

Fungsi ini bertugas menampilkan gambar ASCII Art di layar utama (`stdscr`).

### Alur Logika

1.  **Pengecekan ID Gambar (`if`):**
    ```c++
    if (id_gambar < 0 || id_gambar >= 8) return;
    ```
    * **Tujuan:** Memastikan ID gambar yang diminta valid (antara 0 hingga 7, karena array memiliki 9 elemen yang diindeks 0-8). Jika tidak valid, fungsi berhenti.
2.  **Penentuan Posisi:** Menghitung posisi *x* (kolom) awal agar gambar tampil **di tengah** layar, berdasarkan lebar layar terminal (`COLS`).
3.  **Membersihkan Area:** Menggunakan loop `for` untuk menimpa seluruh area gambar sebelumnya dengan spasi (`' '`) untuk membersihkan layar.
4.  **Menggambar (Loop `while`):**
    ```c++
    while (*p && r < PIC_HEIGHT) {
        // ... (membaca karakter baris per baris)
        mvprintw(PIC_STARTY + r, basex, "%s", baris);
        r++;
    }
    ```
    * Menggunakan *pointer* (`p`) untuk membaca string gambar.
    * Loop berjalan selama belum mencapai akhir string gambar (`*p`) dan belum melebihi tinggi maksimum gambar (`PIC_HEIGHT`).
    * Setiap kali bertemu karakter *newline* (`\n`), satu baris teks dibentuk dan dicetak menggunakan `mvprintw`, lalu pindah ke baris berikutnya (`r++`).
5.  **Pembaruan Layar:** `refresh()` dipanggil untuk menampilkan gambar yang baru dicetak.

---

## 2. Fungsi `tulisAnimasiKeWin`

Fungsi ini menulis teks ke dalam *window* (`win`) yang dibuat secara bertahap, memberikan efek mengetik.

### Alur Logika

1.  **Inisialisasi:** Menentukan posisi awal penulisan teks (`cur_y`, `cur_x`) dan mengambil ukuran maksimum *window* (`maks_y`, `maks_x`).
2.  **Loop Karakter (`while (*p)`):**
    * Memproses teks karakter demi karakter menggunakan *pointer* `p`.
    * **Logika `if (*p == '\n')`:**
        * Jika karakter adalah *newline* (`\n`), posisi baris (`cur_y`) ditambah, dan posisi kolom (`cur_x`) direset ke 2 (awal baris).
        * **Pengecekan Batas:** Jika `cur_y` sudah mencapai batas bawah *window* (`>= maks_y - 1`), fungsi segera berhenti (`return`).
    * **Penulisan Karakter:**
        * Karakter saat ini dicetak ke *window* menggunakan `mvwaddch(win, cur_y, cur_x, *p)`.
        * `wrefresh(win)` dipanggil untuk menampilkan karakter tersebut.
    * **Pindah Kolom dan Baris:**
        * Posisi kolom (`cur_x`) ditambah 1.
        * **Logika Pengecekan Tepi:** Jika `cur_x` mencapai batas tepi kanan *window* (`>= maks_x - 2`), maka pindah baris (`cur_y++`) dan reset kolom (`cur_x = 2`).
        * **Pengecekan Batas:** Pengecekan batas bawah kembali dilakukan.
    * **Delay Animasi:** `napms(DELAY_KETIK_MS)` menjeda program, membuat efek mengetik.
3.  **Nilai Kembali (*Return*):** Fungsi mengembalikan posisi baris (`cur_y + 1`) berikutnya yang kosong, yang akan digunakan sebagai posisi awal penulisan teks di segmen selanjutnya.

---

## 3. Fungsi `putarPartCerita`

Fungsi ini mengelola eksekusi satu bagian (Part) dari cerita (Part 1, Part 2, atau Part 3).

### Alur Logika

1.  **Persiapan Window:** Membersihkan *window* teks (`werase`), menggambar bingkai (`box`), dan mencetak judul "CERITA".
2.  **Loop Segmen (`for`):** Berjalan sebanyak jumlah segmen teks dalam Part tersebut.
    * **Langkah 1: Menulis Teks:**
        ```c++
        posisi_y = tulisAnimasiKeWin(win, posisi_y, segmen_teks[s]);
        ```
        * Memanggil fungsi penulisan animasi, menggunakan `posisi_y` yang didapat dari iterasi sebelumnya.
        * Nilai kembalian (`posisi_y`) dari fungsi akan menjadi posisi awal baris untuk segmen berikutnya.
    * **Langkah 2: Menampilkan Gambar:**
        ```c++
        tampilkanGambarStdScr(gambar_before[s]);
        ```
        * Memanggil fungsi untuk menampilkan gambar ASCII Art yang sesuai dengan segmen tersebut.
    * **Langkah 3: Jeda:**
        ```c++
        napms(TUNGGU_SETELAH_GAMBAR);
        ```
        * Jeda selama 2 detik untuk memberi waktu kepada pengguna melihat teks dan gambar.
3.  **Pembersihan Akhir:** Setelah semua segmen selesai, *window* teks dibersihkan kembali dan diperbarui, siap untuk Part cerita selanjutnya.

---

## 4. Fungsi `mainCerita`

Fungsi ini adalah orkestrator yang mengatur urutan penampilan seluruh cerita.

### Alur Logika

1.  **Inisialisasi Layar:** Membersihkan layar utama (`clear()`, `refresh()`) dan menyembunyikan kursor (`curs_set(0)`).
2.  **Penentuan Posisi Window Teks:** Menghitung posisi *x* dan *y* agar *window* teks muncul di **tengah bawah** layar.
3.  **Pembuatan Window:** Membuat *window* baru (`newwin`) untuk menampung teks cerita.
4.  **Eksekusi Part 1, 2, dan 3:**
    * Untuk setiap Part (1, 2, dan 3):
        * **Efek Suara:** `PlaySound` dipanggil untuk memutar file suara yang sesuai (e.g., `"revan.wav"`). `SND_ASYNC` memastikan suara diputar di *background* tanpa menghentikan program.
        * **Tampilan Cerita:** Fungsi `putarPartCerita` dipanggil dengan data teks dan gambar yang relevan untuk Part tersebut.
5.  **Akhir Cerita:**
    * Setelah semua Part selesai, pesan "Cerita selesai..." ditampilkan di *window* teks.
    * `wgetch(win)` menunggu input tombol dari pengguna.
6.  **Pembersihan:** *Window* teks dihapus (`delwin(win)`).
