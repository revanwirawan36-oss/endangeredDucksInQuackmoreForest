# 🏆 Tugas Akhir Matkul Pengenalan Pemrograman (PP)
# 🎯 Catch The Duck: Program Konservasi Bebek

**Proyek Game Terminal C++ Menggunakan Ncurses**


Catch The Duck adalah *game* berbasis terminal untuk dua pemain yang mensimulasikan misi penelitian dan konservasi spesies bebek langka. Permainan ini dirancang untuk menguji **strategi, perhitungan, dan daya ingat** pemain melalui dua mode kesulitan yang berbeda.



---



## 🧭 Daftar Isi
1.  [Latar Belakang Cerita (Misi Konservasi)](#-latar-belakang-cerita-misi-konservasi)
2.  [Fungsi Game: Pelatihan Kecerdasan Taktis](#-fungsi-game-pelatihan-kecerdasan-taktis)
3.  [Fitur Universal & Mekanika](#-fitur-universal--mekanika)
4.  [Input Kontrol](#-input-kontrol)
5.  [Daftar Ikon dan Pergerakan](#-daftar-ikon-dan-pergerakan-6-varian)
6.  [Persiapan dan Kompilasi](#-persiapan-dan-kompilasi)
7.  [Kredit](#-kredit)



---



## 📖 Latar Belakang Cerita Dibalik Game (Misi Konservasi)



Hutan Quackmur menyimpan rahasia: spesies bebek langka yang hanya muncul beberapa kali dalam setahun. Populasinya menurun drastis, dan para peneliti berjuang menyelamatkan mereka melalui program pembiakan.



Untuk menjalankan program itu, mereka membutuhkan sampel bulu dan telur, tetapi mengambilnya tidak mudah. Bebek-bebek ini bergerak cepat, sulit terlihat, dan hanya keluar dari hutan lindung dalam waktu singkat.



Sebagai pemain, kamu adalah anggota baru dalam tim penelitian. Tugasmu menangkap bebek tanpa menyakiti mereka, mengumpulkan sampel sebanyak mungkin, dan memastikan keberlangsungan hidup spesies yang terancam punah ini.



---



## 🧠 Fungsi Game: Pelatihan Kecerdasan Taktis



Permainan ini berfungsi sebagai alat uji kognitif dengan dua tingkat kesulitan, dirancang untuk melatih **memori kerja spasial**, **perhitungan prediktif**, dan **fungsi eksekutif** Anda:



### 🌟 Mode Easy (Akses Penuh)

* **Fokus:** Bebek selalu terlihat, fokus pada **kecepatan reaksi** dan **strategi perburuan** di tengah pergerakan Bebek.



### 🔥 Mode Hard (Uji Klinis Memori)

* **Tujuan:** Menguji daya ingat dan kalkulasi Anda. Setelah **Fase Memori (8 Menit)**, grid disembunyikan. Anda harus **menghitung posisi target** setelah setiap pergerakan.

* **Tantangan Ekstra:** Membutuhkan penghafalan **12 Bebek** dengan **6 pola pergerakan** yang berbeda.

* **Kalkulasi Multivariabel *Real-time*:** Setelah tembakan, Bebek bergerak, dan pemain harus segera melakukan perhitungan prediktif pada **12 entitas berbeda** dengan 6 vektor pergerakan unik. Ini mengasah **fungsi eksekutif** dan kemampuan otak untuk mengelola banyak variabel secara simultan.

* **Jendela Keputusan Kritis:** Setiap tembakan memicu *highlight* tembakan selama **3 detik** dan *reveal* singkat setelah hit selama **2 detik**. Jendela waktu yang sangat sempit ini menuntut **kecerdasan adaptif** di bawah tekanan, mengubah Anda menjadi **kalkulator taktis** yang harus selangkah lebih maju dari Bebek.

* **Fitur Penyelamat (1x):** Tombol **R** untuk *Reveal Initial Position* selama **15 detik** menjadi aset yang sangat kritis, memaksa pemain untuk memutuskan kapan waktu paling strategis untuk menggunakan sumber daya ingatan terbatas ini.



----



## 🎮 Fitur Universal & Mekanika

| Fitur Mekanika | Detail Universal |
| :---: | :---: |
| **Grid Utama** | $12$ Baris $\times$ $15$ Kolom. |
| **Pemain** | Dua pemain (*Pass-and-Play*), bergantian tembakan. |
| **Pergerakan** | Semua Bebek yang masih hidup bergerak satu langkah **setelah setiap tembakan**. Pergerakan bersifat *Wrapping* (melingkar) di batas grid. |
| **Batasan** | Maksimal 14 tembakan (7 per pemain), atau sampai semua Bebek berhasil ditangkap. |
| **Pencatatan Skor** | Skor dicatat dan disimpan ke file `score.txt`. |

### Perbandingan Mode

| Fitur | Mode Easy | Mode Hard |
| :---: | :---: | :---: |
| **Visibilitas Bebek** | **Selalu Terlihat** (Memudahkan) | **Tersembunyi** setelah Fase Memori (Menantang) |
| **Jumlah Bebek** | 8 Bebek | **12 Bebek** |
| **Varian Pola** | 4 Jenis Pergerakan | **6 Jenis Pergerakan** |
| **Fase Memori** | **8 Menit** di awal permainan | **8 Menit** di awal permainan |
| **Reveal Initial** | Tidak Ada | **1x Kesempatan (Tombol R)** untuk melihat posisi awal Bebek. |

### 🧭 Fitur Antarmuka Utama

* **Modul `loading`**: Menampilkan *loading screen* saat transisi.
* **Modul `story`**: Menampilkan narasi pengantar misi konservasi.
* **Modul `rules`**: Menjelaskan aturan, mekanisme, dan pergerakan Bebek (dapat diakses dari menu).
* **Modul `leaderboard`**: Menampilkan riwayat skor tertinggi yang disimpan.

### 🔑 Input Kontrol

| Aksi | Tombol | Keterangan |
| :---: | :---: | :---: |
| **Navigasi Kursor** | **Tombol Panah** | Menggerakkan kursor di grid (menggunakan *wrapping*). |
| **Tembak** | **ENTER** / **Newline** (`\n`) | Mengakhiri giliran dan memicu pergerakan Bebek. |
| **Skip Fase Memori** | **E** / **e** | Melewati waktu tunggu 8 menit (Hanya Mode Hard). |
| **Reveal Initial (1x)** | **R** / **r** | Menampilkan posisi awal 12 Bebek selama 15 detik (Hanya Mode Hard). |
| **Keluar Game** | **ESC** (`27`) | Keluar dari *game loop*. |

### 🎨 Daftar Ikon dan Pergerakan (6 Varian)

| Ikon | Warna | Pergerakan ($\Delta x$, $\Delta y$) | Deskripsi |
| :---: | :---: | :---: | :---: |
| `o&` | Biru | $(+2, -1)$ | 2 Kanan, 1 Atas |
| `<o` | Merah | $(-1, -3)$ | 1 Kiri, 3 Atas |
| `o>` | Hijau | $(+3, 0)$ | 3 Kanan, 0 Atas (Horizontal) |
| `o\|` | Kuning | $(-2, +2)$ | 2 Kiri, 2 Bawah |
| `+o` | Magenta | $(+3, +2)$ | 3 Kanan, 2 Bawah (Hanya Mode Hard) |
| `0o` | Cyan | $(-2, +4)$ | 2 Kiri, 4 Bawah (Hanya Mode Hard) |

---




## 🛠️ Persiapan dan Kompilasi



Proyek ini menggunakan pustaka **Ncurses** untuk tampilan terminal dan **Windows API** untuk suara.



### Ketergantungan



* Pustaka Ncurses (atau PDCurses di Windows).


* *Header* `<windows.h>` dan `<mmsystem.h>`.


* *Linker* `winmm.lib` untuk fungsi `PlaySound()`.



### Kompilasi



Gunakan perintah kompilasi ini untuk menggabungkan semua file sumber dengan benar:



```bash

g++ main.cpp leaderboard.cpp story.cpp rules.cpp menu.cpp easy.cpp hard.cpp loading.cpp -lncurses -lwinmm -o game

```
Menjalankan Game 

```bash
./game

```
---

## Kredit
Proyek Tugas Akhir mata kuliah Pengenalan Pemrograman ini dikembangkan oleh:
| Nama Anggota | NPM |
| :---: | :---: |
| Muhammad Revan Wirawan | 2517051010 |
| Ratu Salsabila Humaira | 2517051019 |
| Afifah Raidathu Nasya | 2517051043 |

---

## 📸 Cuplikan Game (GIF Demonstrasi)

Di bawah ini adalah beberapa cuplikan visual dari interface terminal yang dibuat menggunakan library Ncurses:

### 1. Menu Utama

![GIF Menu Utama](https://raw.githubusercontent.com/revanwirawan36-oss/endangeredDucksInQuackmoreForest/main/GIF/menu.gif)

### 2. Cuplikan Gameplay

Berikut adalah demonstrasi Bebek dan input tembakan:
![GIF Gameplay](https://raw.githubusercontent.com/revanwirawan36-oss/endangeredDucksInQuackmoreForest/main/GIF/footage.gif)

### 3. Cuplikan Story/Loading

Cuplikan narasi cerita:
![GIF Story/Loading](https://raw.githubusercontent.com/revanwirawan36-oss/endangeredDucksInQuackmoreForest/main/GIF/story.gif)

---
