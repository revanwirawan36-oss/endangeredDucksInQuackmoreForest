# ⏳ Alur Kerja Program `loading.cpp`

Dokumen ini menjelaskan alur logika dan cara kerja dari file C++ `loading.cpp`, yang bertanggung jawab menampilkan animasi loading sederhana berupa bebek yang bergerak (ASCII Art) menggunakan library **ncurses** dan memutar suara latar.

---

## 🛠️ Variabel dan Konstanta Utama

| Nama Konstanta | Nilai / Deskripsi | Fungsi |
| :--- | :--- | :--- |
| `DUCK_HEIGHT` & `DUCK_WIDTH` | `4` & `9` | Ukuran (tinggi dan lebar) dari gambar ASCII Bebek. |
| `ANIMATION_RANGE` | `40` | Jarak total (dalam kolom) yang akan ditempuh oleh animasi bebek. Ini adalah panjang "rel" animasinya. |
| `DELAY_MS` | `100` (ms) | Jeda waktu antara setiap langkah gerakan bebek, mengatur kecepatan animasi. |
| `DUCK_FRAME[]` | Array `const char*` | Array 4 baris yang menyimpan representasi gambar Bebek (ASCII Art). |

---

## 1. Fungsi `tampilkanLoadingBebek` (Alur Program Utama)

Fungsi ini adalah satu-satunya fungsi yang mengatur inisialisasi, pemutaran suara, dan loop animasi.

### A. Inisialisasi Lingkungan ncurses dan Suara

1.  **Inisialisasi ncurses:**
    * `initscr()`: Memulai mode `ncurses`.
    * `noecho()`: Menonaktifkan tampilan karakter yang diketik pengguna.
    * `cbreak()`: Mengatur terminal agar input keyboard tersedia segera (tanpa menunggu Enter).
    * `curs_set(0)`: Menyembunyikan kursor (pointer) terminal.
2.  **Pemutaran Suara:**
    * `PlaySound(TEXT("C:\\projekUap\\loading.wav"), NULL, SND_FILENAME | SND_ASYNC);`
    * Memainkan file suara loading. `SND_ASYNC` memastikan program tidak berhenti dan langsung melanjutkan ke langkah berikutnya.
    * `napms(500)`: Jeda selama 500 ms (setengah detik) setelah suara dipanggil.
3.  **Penentuan Posisi:**
    * Menghitung posisi awal `start_y` (baris) dan `start_x` (kolom) agar animasi **berada di tengah layar** terminal.

### B. Persiapan Tampilan Awal

1.  **Teks "Loading...":** Dicetak 2 baris di atas posisi animasi.
2.  **Pembuatan "Rel" Animasi:**
    * Array `rail` dibuat dengan panjang `ANIMATION_RANGE` (40).
    * `memset(rail, '=', ANIMATION_RANGE)`: Mengisi seluruh array `rail` dengan karakter `'='`.
    * Rel dicetak di bawah posisi Bebek, berfungsi sebagai jalur visual pergerakan.
3.  **Pengaturan Input Non-Blokir:**
    * `nodelay(stdscr, TRUE)`: Ini adalah pengaturan penting. Ini membuat `getch()` **tidak akan menunggu** input dari pengguna. Jika tidak ada tombol yang ditekan, `getch()` akan langsung mengembalikan nilai tanpa menahan jalannya animasi.

### C. Loop Animasi Pergerakan Bebek

Loop `for` ini adalah inti dari animasi, yang membuat bebek bergerak dari kiri ke kanan.

* **Loop Kontrol:**
    ```c++
    for (int current_x = start_x; current_x < (start_x + ANIMATION_RANGE - DUCK_WIDTH); current_x++) {
        // ... kode pergerakan
    }
    ```
    * Loop berjalan selama posisi kolom Bebek saat ini (`current_x`) belum mencapai batas kanan "rel" dikurangi lebar Bebek (`DUCK_WIDTH`), memastikan Bebek tidak keluar dari area animasi.

* **Logika Pergerakan (Di dalam Loop):**

    1.  **Menghapus Posisi Sebelumnya (`if`):**
        ```c++
        int prev_x = current_x - 1; 
        if (prev_x >= start_x) {
            for (int r = 0; r < DUCK_HEIGHT; r++) {
                mvprintw(start_y + r, prev_x, "         "); // Hapus Bebek di posisi lama
            }
        }
        ```
        * **Kondisi `if`:** Hanya menghapus gambar sebelumnya jika Bebek **sudah mulai bergerak** (`prev_x` masih di dalam batas `start_x` atau lebih besar).
        * **Aksi:** Mencetak 9 spasi (`"         "`) di posisi sebelumnya. Lebar 9 spasi ini sama dengan lebar Bebek (`DUCK_WIDTH`), sehingga gambar Bebek lama hilang.

    2.  **Mencetak Posisi Baru:**
        ```c++
        for (int r = 0; r < DUCK_HEIGHT; r++) {
            mvprintw(start_y + r, current_x, DUCK_FRAME[r]);
        }
        ```
        * Menggunakan loop `for` untuk mencetak 4 baris (`DUCK_HEIGHT`) dari `DUCK_FRAME` di posisi kolom Bebek yang baru (`current_x`).

    3.  **Pembaruan Layar dan Jeda:**
        * `refresh()`: Menampilkan semua perubahan yang baru saja dicetak (penghapusan dan pencetakan Bebek baru) ke layar.
        * `getch()`: Membaca input, tetapi karena `nodelay` diaktifkan, ini tidak akan menahan program.
        * `napms(DELAY_MS)`: Menjeda selama 100 ms untuk menghasilkan efek animasi pergerakan.

### D. Penutup Program

1.  **`endwin()`:** Setelah loop selesai, fungsi ini dipanggil untuk mengakhiri mode `ncurses` dan mengembalikan terminal ke keadaan normal.
   
