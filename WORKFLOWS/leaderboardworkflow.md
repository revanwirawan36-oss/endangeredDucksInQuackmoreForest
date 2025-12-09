# 📂 Alur Kerja Program `leaderboard.cpp`

Dokumen ini menjelaskan alur logika dan cara kerja dari file C++ `leaderboard.cpp`, yang bertugas mengelola dan menampilkan papan peringkat (leaderboard) sebuah game.

---

## 🛠️ Fungsi Utama

File ini berisi dua fungsi utama:

1.  **`sortLeaderboard(ScoreEntry leaderboard[], int count)`**: Untuk mengurutkan data skor yang sudah dibaca dari file.
2.  **`showLeaderboard()`**: Untuk membaca data, mengurutkan, dan menampilkannya di layar menggunakan library `ncurses`.

---

## 1. Fungsi `sortLeaderboard`

Fungsi ini bertanggung jawab untuk mengurutkan daftar skor (leaderboard) dari yang **tertinggi** ke yang **terendah**.

### Logika Pengurutan (Bubble Sort)

Fungsi ini menggunakan algoritma **Bubble Sort** untuk menukar posisi entri skor hingga terurut.

* **Tujuan:** Mengurutkan dari skor terbesar ke skor terkecil (descending).

* **Logika Perbandingan `if` dan `else if`:**
    1.  **Prioritas Utama (Skor):**
        ```c++
        if (leaderboard[j].skor < leaderboard[j+1].skor) {
            swap(leaderboard[j], leaderboard[j+1]);
        }
        ```
        * **Kondisi:** Jika skor entri saat ini (`leaderboard[j].skor`) **lebih kecil** dari skor entri berikutnya (`leaderboard[j+1].skor`), maka **tukar posisi** mereka. Ini memastikan skor yang lebih besar selalu berada di posisi depan (lebih tinggi peringkatnya).

    2.  **Prioritas Kedua (Level - Jika Skor Sama):**
        ```c++
        else if (leaderboard[j].skor == leaderboard[j+1].skor) {
             if (leaderboard[j].level < leaderboard[j+1].level) {
                 swap(leaderboard[j], leaderboard[j+1]);
             }
        }
        ```
        * **Kondisi:** Jika skor kedua entri **sama** (`==`), program akan memeriksa levelnya.
        * **Logika Level:** Diasumsikan Level **Sulit** memiliki nilai lebih besar (misalnya `1`) daripada Level **Mudah** (misalnya `0`).
        * Jika level entri saat ini (`leaderboard[j].level`) **lebih kecil** dari level entri berikutnya (`leaderboard[j+1].level`), artinya entri saat ini adalah **Mudah** dan entri berikutnya adalah **Sulit** (0 < 1).
        * Dalam kasus skor sama, entri dari level **Sulit** harus ditempatkan di atas entri dari level **Mudah**, sehingga mereka **ditukar** posisinya.

---

## 2. Fungsi `showLeaderboard`

Fungsi ini adalah inti dari tampilan papan peringkat. Alurnya dibagi menjadi tiga tahap utama: Membaca Data, Mengurutkan Data, dan Menampilkan Data.

### A. Membaca Data dari File (`score.txt`)

1.  **Inisialisasi:** Membuat array `leaderboard` dan penghitung `entryCount = 0`.
2.  **Membuka File:** Mencoba membuka file bernama `"score.txt"`.
3.  **Logika `if (file.is_open())`:**
    * **Kondisi `true` (File Terbuka):** Program masuk ke loop `while`.
    * **Loop `while`:**
        ```c++
        while (file >> nama_temp >> skor_temp >> level_temp && entryCount < MAX_ENTRIES) {
            // ... proses membaca data dan menyimpan ke array leaderboard
            entryCount++;
        }
        ```
        * **Tugas:** Membaca data dari file (nama, skor, level) secara berulang.
        * **Pengamanan:** Loop akan berhenti jika:
            * Data di file sudah habis.
            * Jumlah entri yang terbaca sudah mencapai batas maksimum (`MAX_ENTRIES`).
    * **Menyimpan Data:** Data yang dibaca disimpan ke dalam array `leaderboard` dan `entryCount` bertambah.
    * **Menutup File:** File ditutup dengan `file.close()`.

### B. Mengurutkan Data

1.  **Logika `if (entryCount > 0)`:**
    * **Kondisi:** Jika **ada** skor yang terbaca dari file (`entryCount` lebih dari 0), maka panggil fungsi **`sortLeaderboard`** untuk mengurutkan data.
    * **Kondisi `false`:** Jika tidak ada data, proses pengurutan dilewati.

### C. Menampilkan Leaderboard (Menggunakan `ncurses`)

Bagian ini berurusan dengan pembuatan *window* dan menampilkan teks di tengah layar.

1.  **Persiapan Layar:** Fungsi `clear()` dan `refresh()` dipanggil untuk membersihkan dan memperbarui layar utama `ncurses`.

2.  **Menghitung Ukuran Window:**
    * Program menghitung ukuran layar (`maxBaris`, `maxKolom`).
    * Dihitung ketinggian dan lebar *window* yang dibutuhkan (`requiredHeight`, `windowWidth`) dengan ukuran maksimum Lebar 60 dan Tinggi yang cukup untuk menampung konten (minimal 7 baris + baris skor).
    * Tinggi dan lebar *window* akhir disesuaikan agar tidak melebihi ukuran layar.

3.  **Logika Pengamanan Ukuran:**
    ```c++
    if (windowHeight < 5 || windowWidth < 20) return;
    ```
    * **Tujuan:** Jika layar terminal terlalu kecil (tinggi kurang dari 5 atau lebar kurang dari 20), fungsi **dihentikan** (`return`) untuk mencegah *crash* tampilan.

4.  **Membuat dan Mengatur Window:**
    * *Window* baru (`leaderWindow`) dibuat di tengah layar.
    * `keypad(leaderWindow, TRUE)`: Mengaktifkan *keypad* untuk *window*.
    * `box(leaderWindow, 0, 0)`: Membuat garis bingkai di sekeliling *window*.

5.  **Menampilkan Header (Judul dan Kolom):**
    * Judul seperti "**DAFTAR PERINGKAT**" dan nama kolom ("#", "NAMA", "SKOR", "LEVEL") dicetak di posisi yang sudah dihitung.
    * Garis pembatas horizontal (`ACS_HLINE`) dicetak.

6.  **Menampilkan Entri Skor (Logika `if/else` Entri):**
    * **Logika `if (entryCount == 0)`:**
        * **Kondisi `true`:** Jika tidak ada data skor, pesan "**Belum ada skor yang tercatat.**" ditampilkan di tengah.
    * **Logika `else` (Ada Entri):**
        * Program masuk ke loop `for` untuk menampilkan setiap entri skor yang sudah diurutkan.
        * **Pengamanan Tampilan:**
            ```c++
            if (barisAwal + i < windowHeight - 2) {
                // ... tampilkan data
            }
            ```
            * Loop hanya menampilkan data jika baris entri masih muat di dalam *window* (di atas baris pesan "Tekan tombol...").
        * **Pencetakan:** Nomor, Nama, Skor, dan Level (dikonversi dari `0` atau `1` menjadi "Mudah" atau "Sulit") dicetak.

7.  **Menampilkan Pesan Keluar:**
    * Pesan "**Tekan tombol apa saja untuk kembali...**" dicetak di bagian bawah *window*.

8.  **Pembaruan dan Input:**
    * `wrefresh(leaderWindow)`: Memperbarui tampilan *window*.
    * `getch(); getch();`: Menunggu dua kali input tombol dari pengguna. (Kemungkinan pertama untuk tombol apa saja, dan kedua untuk penekanan tombol lain/enter, atau ini adalah *double-check* untuk memastikan input terbaca).

9.  **Membersihkan:**
    * `delwin(leaderWindow)`: Menghapus *window* dari memori.
    * `clear(); refresh();`: Membersihkan dan memperbarui layar utama sebelum program kembali ke menu utama.

---

## 💡 Konsep Utama untuk Mahasiswa Awal

| Konsep | Penjelasan |
| :--- | :--- |
| **Bubble Sort** | Algoritma pengurutan yang berulang kali membandingkan dan menukar elemen yang berdekatan jika urutannya salah. |
| **File I/O (`fstream`)** | Cara program membaca data dari file (`ifstream`) di *disk* (dalam kasus ini, `score.txt`). |
| **`ncurses`** | Library yang digunakan untuk membuat antarmuka berbasis teks (seperti *window*, kotak, dan mengatur posisi teks) di terminal. |
| **Pengecekan *Boundary* (`getmaxyx`)** | Teknik untuk mengetahui ukuran terminal (`maxBaris`, `maxKolom`) sehingga program bisa menyesuaikan tampilan agar tidak melebihi layar. |
| **Logika *If-Else* Bersarang** | Digunakan pada `sortLeaderboard` untuk memberikan prioritas. Jika kondisi utama (`if`) tidak terpenuhi, barulah kondisi kedua (`else if`) yang lebih spesifik (membandingkan level) diperiksa. |
