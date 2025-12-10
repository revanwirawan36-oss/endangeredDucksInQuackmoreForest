# 📁 `workflow.md` - Alur Kerja Fungsi Menu (`menu.cpp`)

File `menu.cpp` berisi satu fungsi utama, `drawMenuWindow`, yang bertanggung jawab untuk membuat dan mengelola antarmuka menu interaktif di terminal menggunakan *library* `ncurses`.

## 1\. ⚙️ Fungsi Utama: `drawMenuWindow`

```c
int drawMenuWindow(const char *title, const char *items[], int count)
```

Fungsi ini menerima tiga parameter:

1.  `title`: Judul menu (misalnya, "MAIN MENU").
2.  `items[]`: Array dari string (pilihan menu, misal: {"Mulai", "Skor", "Keluar"}).
3.  `count`: Jumlah item dalam array `items`.

Fungsi ini akan mengembalikan **indeks (posisi)** dari item menu yang dipilih, atau **-1** jika pengguna menekan tombol **ESC**.

## 2\. 🏁 Inisialisasi dan Penempatan

1.  **Clear Screen:** `clear(); refresh();` untuk membersihkan layar terminal sebelum menu digambar.
2.  **Variabel Kontrol:**
      * `highlight = 0`: Menyimpan indeks item menu yang sedang disorot (awal diatur ke item pertama/indeks 0).
      * `ch`: Variabel untuk menyimpan *input* karakter dari pengguna.
3.  **Perhitungan Ukuran *Window*:**
      * Ukuran *window* dihitung agar cukup menampung judul, batas, dan semua item menu (`win_tinggi`, `win_lebar`).
4.  **Penempatan di Tengah:**
      * `starty` dan `startx` dihitung untuk menempatkan *window* di tengah layar terminal, menggunakan variabel global `LINES` (tinggi terminal) dan `COLS` (lebar terminal) dari `ncurses`.
5.  **Pembuatan *Window*:** `WINDOW *win = newwin(win_tinggi, win_lebar, starty, startx);`
6.  **`keypad(win, TRUE);`:** Mengaktifkan pemrosesan tombol khusus seperti `KEY_UP`, `KEY_DOWN`, dan `KEY_ENTER`.
7.  **Logika Judul Game:**
      * **`if (strcmp(title, "MAIN MENU") == 0)`:** Jika judul yang diberikan adalah "MAIN MENU", program mencetak judul game ("CATCH THE DUCK") sedikit di atas *window* menu.

## 3\. 🔁 *Loop* Utama Menu

```c
while (1) { ... }
```

Ini adalah *loop* tak terbatas yang terus menggambar menu dan menunggu *input* dari pengguna sampai menu dipilih atau dibatalkan.

### A. Menggambar *Window*

1.  **Bersihkan & Kotak:** `werase(win); box(win, 0, 0);` membersihkan konten *window* dan menggambar ulang batasnya.
2.  **Cetak Judul:** Judul dicetak di baris kedua *window*, diposisikan di tengah.
3.  ***Loop* Cetak Item (`for` loop):**
      * *Loop* berjalan sebanyak jumlah item menu (`count`).
      * **Logika *Highlight* (Sorotan):**
        ```c
        if (i == highlight)
            wattron(win, A_REVERSE); // Nyalakan efek warna terbalik

        mvwprintw(win, posy, ... , items[i]); // Cetak item

        if (i == highlight)
            wattroff(win, A_REVERSE); // Matikan efek warna terbalik
        ```
        Logika ini memastikan **hanya item yang sedang disorot (`i == highlight`)** yang dicetak dengan warna terbalik, memberikan efek visual bahwa item tersebut aktif/terpilih.

### B. Menerima *Input*

`ch = wgetch(win);` menunggu pengguna menekan tombol.

### C. Alur Logika Kontrol (`if-else if`)

| Tombol Ditekan | Variabel Cek | Logika Perubahan Sorotan (`highlight`) | Logika Keluar/Kembali |
| :--- | :--- | :--- | :--- |
| **Atas** | `ch == KEY_UP` | `highlight--`. Jika `highlight` di bawah 0 (paling atas), pindah ke item paling bawah (`highlight = count - 1`). | - |
| **Bawah** | `ch == KEY_DOWN` | `highlight++`. Jika `highlight` mencapai `count` (melebihi batas), pindah ke item paling atas (`highlight = 0`). | - |
| **Enter** | `ch == KEY_ENTER || ch == '\n'` | - | Hapus *window* (`delwin(win)`) dan **kembalikan `highlight`** (indeks item yang dipilih). |
| **ESC (Escape)** | `ch == 27` | - | Hapus *window* (`delwin(win)`) dan **kembalikan `-1`** (kode untuk Batal/Keluar). |

## 4\. 🧹 Pembersihan

Jika *loop* `while(1)` dihentikan (karena Enter atau ESC ditekan):

1.  *Window* menu dihapus (`delwin(win)`).
2.  Indeks yang dipilih (atau -1) dikembalikan ke fungsi pemanggil.
3.  Kursor disembunyikan lagi (`curs_set(0);`) dan layar dibersihkan.
