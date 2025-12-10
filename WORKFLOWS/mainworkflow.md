# 📂 'Workflow' dan Logika Program Utama (`main.cpp`)

Dokumen ini menjelaskan alur kerja dan logika utama dari program C++ yang Anda buat, yang merupakan aplikasi berbasis konsol menggunakan library **ncurses**.

---

## 🚀 Pendahuluan

Program ini adalah sebuah permainan yang menyediakan **Main Menu** sebagai titik awal. Dari menu utama, pengguna dapat memilih untuk **Bermain** (dengan dua tingkat kesulitan), melihat **Cerita** permainan, membaca **Aturan** permainan, melihat **Papan Peringkat** (**Leaderboard**), atau **Keluar** dari program.

### 🛠️ Inisialisasi Program

Bagian awal fungsi `main()` menyiapkan lingkungan konsol menggunakan **ncurses**:

* `initscr();`: Memulai layar ncurses (lingkungan konsol).
* `noecho();`: Mematikan *echo* (karakter yang diketik tidak ditampilkan).
* `cbreak();`: Mengatur mode input ke *cbreak* (input diproses segera, tanpa menunggu tombol *Enter*).
* `keypad(stdscr, TRUE);`: Memungkinkan program mendeteksi tombol fungsi seperti panah atau *Esc*.
* `start_color();`: Mengaktifkan penggunaan warna.
* `curs_set(0);`: Menyembunyikan kursor.


---

## 🔁 Logika Loop Utama (`while (1)`)

Seluruh program berjalan di dalam sebuah *infinite loop* (`while (1)`). Ini memastikan program terus menampilkan menu utama dan menunggu input pengguna setelah selesai menjalankan aksi (seperti bermain game atau melihat cerita), sampai pengguna memilih untuk keluar.

### 1. **Pembersihan Layar dan Menu Utama**

* `clear();` dan `refresh();`: Membersihkan dan menggambar ulang layar untuk menghilangkan konten sebelumnya.
* **Menu Utama Dibuat**:
    ```cpp
    const char *mainItems[5] = { "Play", "Story", "Rules", "Leaderboard", "Quit" }; 
    int mainChoice = drawMenuWindow("MAIN MENU", mainItems, 5);
    ```
    Fungsi `drawMenuWindow` akan menampilkan menu dengan opsi **Play, Story, Rules, Leaderboard, Quit** dan mengembalikan indeks pilihan pengguna (0 sampai 4).

### 2. **Alur Logika Keputusan (`if-else if`)**

Program menggunakan serangkaian `if-else if` untuk menentukan tindakan yang harus dilakukan berdasarkan nilai dari variabel `mainChoice`.

| `mainChoice` | Kondisi / Opsi yang Dipilih | Deskripsi Tindakan |
| :---: | :---: | :--- |
| **0** | `"Play"` | Masuk ke **Sub-Menu Play** untuk memilih tingkat kesulitan. |
| **1** | `"Story"` | Menjalankan fungsi untuk menampilkan **Cerita** permainan. |
| **2** | `"Rules"` | Menjalankan fungsi untuk menampilkan **Aturan** permainan. |
| **3** | `"Leaderboard"` | Menjalankan fungsi untuk menampilkan **Papan Peringkat**. |
| **4** atau **-1** | `"Quit"` atau Tombol *Esc* | Keluar dari loop utama dan mengakhiri program. |

---

## 🕹️ Detail Logika Cabang

### 1. **Cabang "Play" (`mainChoice == 0`)**

Jika pengguna memilih "Play", program akan masuk ke sub-menu dengan alur logika bertingkat:

* **Sub-Menu Play Dibuat**:
    ```cpp
    const char *playItems[2] = { "Easy", "Hard" }; 
    int playChoice = drawMenuWindow("PLAY MENU", playItems, 2); 
    ```
    Pengguna diminta memilih antara **Easy** (Mudah) atau **Hard** (Sulit).

* **Logika Tingkat Kesulitan**:
    * **Jika `playChoice == 0` (Easy)**:
        1.  Layar dibersihkan.
        2.  Fungsi `tampilkanLoadingBebek()` dijalankan (menampilkan layar *loading*).
        3.  Fungsi **`mulaiGameEasy()`** dijalankan untuk memulai permainan tingkat mudah.
    * **Jika `playChoice == 1` (Hard)**:
        1.  Layar dibersihkan.
        2.  Fungsi `tampilkanLoadingBebek()` dijalankan.
        3.  Fungsi **`mulaiGameHard()`** dijalankan untuk memulai permainan tingkat sulit.

### 2. **Cabang "Story" (`mainChoice == 1`)**

Jika pengguna memilih "Story":
1.  Layar dibersihkan.
2.  Fungsi `tampilkanLoadingBebek()` dijalankan.
3.  Fungsi **`mainCerita()`** dijalankan untuk menampilkan cerita permainan.

### 3. **Cabang "Rules" (`mainChoice == 2`)**

Jika pengguna memilih "Rules":
* Fungsi **`showRules()`** dijalankan untuk menampilkan aturan permainan.

### 4. **Cabang "Leaderboard" (`mainChoice == 3`)**

Jika pengguna memilih "Leaderboard":
* Fungsi **`showLeaderboard()`** dijalankan untuk menampilkan papan peringkat.

### 5. **Cabang "Quit" (`mainChoice == 4` atau `mainChoice == -1`)**

* `mainChoice == 4`: Pengguna memilih opsi "Quit" pada menu.
* `mainChoice == -1`: Pengguna menekan tombol *Esc* saat berada di menu.

Jika salah satu kondisi ini terpenuhi, perintah **`break;`** dijalankan. Ini akan **menghentikan** *infinite loop* (`while (1)`).

---

## 🛑 Penutup Program

Setelah loop utama berhenti (karena pengguna memilih **Quit** atau menekan *Esc*):

* `endwin();`: Fungsi ini mengembalikan terminal ke keadaan semula (sebelum ncurses diaktifkan).
* `return 0;`: Mengakhiri eksekusi fungsi `main()`, yang berarti program selesai berjalan.
