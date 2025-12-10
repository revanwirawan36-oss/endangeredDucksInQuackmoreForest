#Alur Kerja Fungsi Aturan (`rules.cpp`)

File `rules.cpp` hanya berisi satu fungsi, `showRules()`, yang berfungsi untuk menampilkan teks aturan permainan kepada pengguna dalam antarmuka terminal yang rapi menggunakan *library* `ncurses`.

## 1\. ⚙️ Fungsi Utama: `showRules()`

Fungsi ini tidak menerima parameter dan tidak mengembalikan nilai (`void`), karena tugas utamanya hanyalah menampilkan informasi dan menunggu konfirmasi pengguna.

### A. Inisialisasi Tampilan

1.  **Sembunyikan Kursor:** `curs_set(0);` agar kursor pengetikan tidak mengganggu tampilan aturan.
2.  **Bersihkan Layar:** `clear(); refresh();` untuk memastikan layar terminal bersih sebelum *window* aturan digambar.

### B. Konten Aturan (`rulesText`)

Aturan permainan disimpan sebagai konstanta string (`const char *rulesText`) yang berisi semua teks aturan dan diakhiri dengan instruksi keluar.

> **Catatan:** Karakter `\n` (newline) digunakan dalam string ini untuk menandai akhir dari setiap baris, yang kemudian akan digunakan dalam logika pencetakan di bawah.

## 2\. 🏞️ Pengaturan *Window* Aturan

1.  **Tentukan Ukuran:** `win_tinggi` (23) dan `win_lebar` (80) ditetapkan untuk ukuran *window*.
2.  **Hitung Posisi Tengah:**
      * `starty` dan `startx` dihitung berdasarkan ukuran total terminal (`LINES` dan `COLS`) agar *window* muncul tepat di tengah layar.
3.  **Buat *Window*:** `WINDOW *win = newwin(win_tinggi, win_lebar, starty, startx);`
4.  **Gambar Batas:** `box(win, 0, 0);` menambahkan bingkai di sekitar *window*.
5.  **Aktifkan *Keypad*:** `keypad(win, TRUE);` memastikan *window* dapat menerima input tombol.

## 3\. ✍️ Logika Pencetakan Teks

Teks aturan tidak bisa dicetak sekaligus, karena `ncurses` perlu diposisikan di dalam *window* baris demi baris, dan ada pemformatan seperti *bullet points* yang harus dipertahankan.

1.  **Cetak Judul:** `mvwprintw(win, 1, ... , "RULES");` mencetak judul "RULES" di baris kedua *window*, diposisikan di tengah.
2.  **Variabel Kontrol Teks:**
      * `y = 3`: Posisi baris awal di dalam *window* untuk mulai mencetak konten aturan.
      * `p = rulesText`: *Pointer* (`*p`) yang menunjuk ke awal string aturan.
      * `line[200]`: *Buffer* (penampung sementara) untuk menyimpan satu baris teks.
3.  ***Loop* Pencetakan Baris (`while (*p)`):**
      * *Loop* ini terus berjalan selama *pointer* `*p` belum mencapai akhir string (karakter `\0`).
      * **Pemisahan Baris:**
        ```c
        while (*p && *p != '\n') line[i++] = *p++; // Salin karakter ke buffer 'line'
        line[i] = '\0'; // Tambahkan null-terminator
        p++; // Lompati karakter '\n'
        ```
          * **Logika:** Karakter dari `rulesText` disalin satu per satu ke *buffer* `line` hingga ditemukan karakter *newline* (`\n`). Setelah satu baris disalin, `*p` digeser melewati `\n`.
      * **Pencetakan Baris:** `mvwprintw(win, y++, 2, "%s", line);`
          * Baris yang sudah dipisahkan (`line`) dicetak di *window*.
          * Variabel `y` di-*increment* (`y++`) agar baris berikutnya dicetak di baris yang lebih rendah.
      * **Ulangi:** Proses ini berlanjut sampai semua karakter di `rulesText` telah diproses.

## 4\. 🚪 Menunggu Konfirmasi dan Keluar

1.  **Perbarui Tampilan:** `wrefresh(win);` memastikan semua teks dan batas yang baru dicetak terlihat oleh pengguna.
2.  **Tunggu Input:** `wgetch(win);` fungsi ini akan menghentikan eksekusi dan menunggu pengguna menekan **tombol apa saja** di *window* tersebut.
3.  **Hapus *Window*:** Setelah tombol ditekan, `delwin(win);` menghapus *window* aturan dari memori `ncurses`.

Fungsi pun selesai, dan kontrol kembali ke program utama (biasanya ke fungsi yang memanggil `showRules()`).
