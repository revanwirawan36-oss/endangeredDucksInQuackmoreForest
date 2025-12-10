# 📊 Workflow Program `leaderboard.cpp`

Dokumen ini menjelaskan alur logika dan cara kerja dari file C++ `leaderboard.cpp`, yang bertugas mengelola, mengurutkan, dan menampilkan papan peringkat (leaderboard) sebuah game menggunakan library **ncurses**.

---

## 🛠️ Fungsi Utama dalam Program

File ini berisi dua fungsi inti:

1.  **`sortLeaderboard(ScoreEntry leaderboard[], int count)`**: Mengurutkan data skor.
2.  **`showLeaderboard()`**: Mengatur pembacaan data, pemanggilan pengurutan, dan tampilan visual leaderboard.

---

## 1. Fungsi `sortLeaderboard` (Logika Pengurutan)

Fungsi ini menggunakan algoritma **Bubble Sort** untuk menukar posisi entri skor agar terurut dari yang **tertinggi** ke yang **terendah** (Descending).

### Logika Perbandingan dan Penukaran

Fungsi ini menggunakan logika `if` bersarang untuk menentukan kapan harus menukar posisi dua entri skor yang berdekatan.

* **Looping:** Terdapat dua loop `for` bersarang untuk membandingkan setiap pasangan entri skor.
* **Prioritas 1: Perbandingan Skor (`if`)**
    ```c++
    if (leaderboard[j].skor < leaderboard[j+1].skor) {
        swap(leaderboard[j], leaderboard[j+1]);
    }
    ```
    * **Kondisi:** Jika skor entri saat ini (`leaderboard[j].skor`) **lebih kecil** dari skor entri berikutnya (`leaderboard[j+1].skor`).
    * **Aksi:** **Tukar** posisi mereka (`swap`). Ini memastikan skor yang lebih besar (lebih tinggi peringkatnya) selalu bergerak ke posisi sebelumnya.
* **Prioritas 2: Perbandingan Level (Jika Skor Sama - `else if`)**
    ```c++
    else if (leaderboard[j].skor == leaderboard[j+1].skor) {
         if (leaderboard[j].level < leaderboard[j+1].level) {
             swap(leaderboard[j], leaderboard[j+1]);
         }
    }
    ```
    * **Kondisi Luar:** Hanya dieksekusi jika skor kedua entri **sama** (`==`).
    * **Kondisi Dalam (`if`):** Jika level entri saat ini **lebih kecil** dari level entri berikutnya (Diasumsikan Level **Sulit** memiliki nilai lebih besar dari **Mudah**, misal: $0 < 1$).
    * **Aksi:** **Tukar** posisi mereka. Ini berarti, jika skor sama, pemain yang bermain di level **Sulit** akan diposisikan di atas pemain yang bermain di level **Mudah**.

---

## 2. Fungsi `showLeaderboard` (Alur Program Utama)

Fungsi ini menjalankan tiga tahap utama: Membaca Data, Mengurutkan Data, dan Menampilkan Tampilan **ncurses**.

### A. Membaca Data dari File (`score.txt`)

1.  **Inisialisasi:** Deklarasi array `leaderboard` dan penghitung `entryCount = 0`.
2.  **Pembukaan File:** Mencoba membuka file `"score.txt"` menggunakan `ifstream file`.
3.  **Logika Pembukaan File (`if (file.is_open())`)**
    * **Kondisi `true`:** File berhasil dibuka, program masuk ke loop pembacaan.
    * **Loop Pembacaan (`while`):**
        ```c++
        while (file >> nama_temp >> skor_temp >> level_temp && entryCount < MAX_ENTRIES) {
            // ... menyimpan data ke array
            entryCount++;
        }
        ```
        * Program membaca (ekstraksi) `nama`, `skor`, dan `level` secara berurutan.
        * Loop berhenti jika: 1) Data di file habis, **ATAU** 2) Jumlah entri mencapai batas maksimum (`MAX_ENTRIES`).
    * **Penyimpanan Data:** Data disimpan ke dalam array `leaderboard`. Karena nama adalah *C-style string* (`char[]`), digunakan `strncpy` dan *null terminator* (`\0`).
    * **Penutupan File:** File ditutup dengan `file.close()`.

### B. Proses Pengurutan

1.  **Logika Pengurutan (`if (entryCount > 0)`)**
    * **Kondisi:** Jika `entryCount` **lebih dari 0** (artinya ada data skor yang terbaca), maka fungsi `sortLeaderboard` dipanggil.
    * **Kondisi `false`:** Jika tidak ada data, proses pengurutan dilewati.

### C. Menampilkan Tampilan (ncurses)

Bagian ini fokus pada penentuan ukuran dan pembuatan tampilan di terminal.

1.  **Persiapan Layar:** Layar `ncurses` utama dibersihkan (`clear(); refresh();`).
2.  **Menghitung Ukuran Window:**
    * `getmaxyx(stdscr, maxBaris, maxKolom)`: Mendapatkan ukuran layar terminal yang sedang aktif.
    * Ukuran *window* (`windowHeight`, `windowWidth`) dihitung. Lebar maksimum ditetapkan 60 kolom.
    * **Logika Pengamanan Ukuran (`if`):**
        ```c++
        if (windowHeight < 5 || windowWidth < 20) return;
        ```
        * Jika layar terminal terlalu kecil (tinggi kurang dari 5 **ATAU** lebar kurang dari 20), fungsi segera **dihentikan** (`return`) untuk mencegah *crash* atau tampilan yang rusak.
3.  **Pembuatan Window:**
    * `WINDOW *leaderWindow = newwin(...)`: Window baru dibuat, diposisikan **di tengah** layar terminal.
    * `box(leaderWindow, 0, 0)`: Membuat garis bingkai di sekeliling window.
4.  **Menampilkan Header:** Judul, garis pembatas (`ACS_HLINE`), dan label kolom ("#", "NAMA", "SKOR", "LEVEL") dicetak.
5.  **Menampilkan Entri Skor:**
    * **Logika Data Kosong (`if (entryCount == 0)`)**
        * **Kondisi `true`:** Pesan "Belum ada skor yang tercatat." ditampilkan di tengah window.
    * **Logika Data Ada (`else`)**
        * Loop `for` dijalankan dari $i=0$ hingga `entryCount-1`.
        * **Pengamanan Tampilan (`if`):**
            ```c++
            if (barisAwal + i < windowHeight - 2) {
                // ... tampilkan data
            }
            ```
            * Entri skor hanya ditampilkan jika barisnya **masih muat** di dalam *window* (yaitu, di atas baris pesan "Tekan tombol...").
        * **Konversi Level:** Variabel `namaLevel` disiapkan: jika `entri.level == 0` maka `namaLevel` adalah "Mudah", jika tidak (`else`) maka "Sulit".
        * Data (`#`, Nama, Skor, `namaLevel`) dicetak pada baris yang sesuai.
6.  **Pesan Keluar:** Pesan "Tekan tombol apa saja untuk kembali..." dicetak di bagian bawah *window*.
7.  **Input dan Bersihkan:**
    * `wrefresh(leaderWindow)`: Memperbarui tampilan window.
    * `getch(); getch();`: Menunggu input tombol dari pengguna.
    * `delwin(leaderWindow)`: Menghapus window dari memori.
    * Layar utama dibersihkan kembali (`clear(); refresh();`).

---

## 💡 Konsep Penting

* **Bubble Sort:** Algoritma pengurutan yang mudah dipahami (pertukaran elemen) namun kurang efisien untuk data dalam jumlah sangat besar.
* **ncurses:** Digunakan untuk membuat antarmuka pengguna berbasis teks, memungkinkan kontrol penuh atas posisi kursor dan pembuatan elemen seperti *window* dan kotak.
* **`fstream`:** Library untuk operasi file Input/Output. `ifstream` adalah singkatan dari *Input File Stream*, digunakan untuk **membaca** data dari file (`score.txt`).
* **Pengecekan Batas (Safeguard):** Pengecekan ukuran layar (`if (windowHeight < 5 || windowWidth < 20)`) adalah praktik pemrograman yang baik untuk memastikan program tidak *crash* atau menampilkan output yang tidak terbaca di terminal yang terlalu kecil.
  
