# 🦆 Duck Hunt Grid: Agen Bebek Maut (Hard Mode)

**Proyek Game Terminal C++ Menggunakan Ncurses**

Duck Hunt Grid adalah *game* tembak-menembak berbasis terminal untuk dua pemain yang menguji kemampuan **memori spasial, perhitungan taktis, dan fokus yang tajam** di bawah tekanan waktu.

---

## 📖 Latar Belakang Cerita (The Infiltration)

Di tengah hamparan jaringan komunikasi yang terenkripsi, tersembunyi sebuah *grid* yang menjadi titik kumpul para Bebek. Bebek-Bebek ini bukan Bebek biasa; mereka adalah **agen bergerak** yang menggunakan simbol-simbol aneh (`o&`, `<o`, `o>`, `o|`) sebagai penanda pergerakan mereka.

Tugas Anda, sebagai agen rahasia, adalah **mengeliminasi semua Bebek** ini sebelum mereka menyelesaikan misi infiltrasi mereka. Waktu berharga, dan setiap tembakan menyebabkan Bebek yang tersisa bergerak, mengubah seluruh peta permainan. **Memori adalah senjata Anda yang paling ampuh.** Gagal mengingat rute pergerakan mereka berarti tertinggal satu langkah dari kehancuran misi.

---

## 🧠 Tantangan Utama: Pertarungan Kognitif

Mode *Hard* ini menuntut **memori kerja spasial** yang tinggi, mengubah *game* dari sekadar tebak-tebakan menjadi arena pertarungan kognitif yang intens.

1.  **Fase Memori (8 Menit):** Permainan dimulai dengan semua 8 Bebek ditampilkan di posisi awal acak mereka selama 8 menit. **Ini adalah satu-satunya kesempatan Anda** untuk menghafal posisi awal dan—yang terpenting—**pola pergerakan** mereka berdasarkan ikon dan vektor kecepatan (`dx`/`dy`).
2.  **Dilema Setiap Tembakan:** Setiap kali pemain menembak, grid akan *reveal* selama **6 detik** (menunjukkan posisi Bebek *saat itu*), mengganggu alur memori Anda.
3.  **Kebutuhan Kalkulasi Mental:** Karena Bebek bergerak setelah setiap tembakan, pemain dituntut untuk **secara mental menghitung posisi masa depan Bebek** yang mereka targetkan, mengandalkan *pattern* pergerakan yang telah dihafal di awal.

---

## 🎮 Fitur Game & Mekanika

| Fitur | Deskripsi |
| :---: | :---: |
| **Pemain** | Dua pemain (*Pass-and-Play*), bergantian tembakan. |
| **Grid** | $12$ Baris $\times$ $15$ Kolom. |
| **Target** | 8 Bebek bergerak. |
| **Pergerakan** | Semua Bebek yang masih hidup bergerak satu langkah **setelah setiap tembakan**. Pergerakan bersifat *Wrapping* (melingkar) di batas grid. |
| **Batasan** | Maksimal 14 tembakan (7 per pemain), atau sampai semua Bebek mati. |
| **Fitur *Hard Mode*** | **Reveal Initial Position:** Pemain memiliki 1x kesempatan untuk menekan **R** dan melihat posisi Bebek di awal game (15 detik). |
| **Skor** | Skor disimpan ke `score.txt`. |

### 🔑 Input Kontrol

| Aksi | Tombol | Keterangan |
| :---: | :---: | :---: |
| **Navigasi Kursor** | **Tombol Panah** | Kursor menggunakan *wrapping* saat mencapai batas grid. |
| **Tembak** | **ENTER** / **Newline** (`\n`) | Mengakhiri giliran dan memicu pergerakan Bebek. |
| **Skip Fase Memori** | **E** / **e** | Melewati waktu tunggu 8 menit. |
| **Reveal Initial** | **R** / **r** | Hanya tersedia sekali (Hard Mode). |
| **Keluar Game** | **ESC** (`27`) | Keluar dari *game loop*. |

### 🎨 Daftar Ikon dan Vektor Pergerakan

| Ikon | Warna | Vektor ($\Delta x$, $\Delta y$) | Deskripsi |
| :---: | :---: | :---: | :---: |
| `o&` | Biru | $(+2, -1)$ | 2 Kanan, 1 Atas |
| `<o` | Merah | $(-1, -3)$ | 1 Kiri, 3 Atas |
| `o>` | Hijau | $(+3, 0)$ | 3 Kanan, 0 Atas (Horizontal) |
| `o|` | Kuning | $(-2, +2)$ | 2 Kiri, 2 Bawah |

---

## 🛠️ Persiapan dan Kompilasi

Proyek ini menggunakan pustaka **Ncurses** untuk tampilan terminal dan **Windows API** untuk suara.

### Ketergantungan

* Pustaka Ncurses (atau PDCurses di Windows).
* *Header* `<windows.h>` dan `<mmsystem.h>`.
* *Linker* `winmm.lib` untuk fungsi `PlaySound()`.

### Kompilasi (Contoh GCC/MinGW)

```bash
# Perintah kompilasi di Windows dengan MinGW
g++ main.cpp leaderboard.cpp story.cpp rules.cpp menu.cpp easy.cpp hard.cpp loading.cpp -lncurses -lwinmm -o game

# Menjalankan Game
./duck_hunt
