# Memory Maze

Game labirin 3D berbasis OpenGL dengan mekanik memorisasi peta, eksplorasi dalam kegelapan, dan pengumpulan kode warna.

## Deskripsi Game

Kamu terjebak dalam labirin gelap! Sebelum kegelapan menutupi segalanya, kamu memiliki waktu singkat untuk menghafalkan peta dan lokasi 3 kunci berwarna (Merah, Biru, Kuning). Setelah fase memorisasi berakhir, peta tertutup dan kamu harus menavigasi labirin dalam kegelapan menggunakan senter yang terbatas. Temukan semua kunci, kumpulkan digit kode masing-masing, dan masukkan kode di pintu keluar untuk memenangkan permainan.

### Mekanik Utama

- **Fase Memorisasi** — Peta terbuka dari pandangan atas selama 7 detik. Hafalkan layout labirin dan posisi kunci.
- **Fase Eksplorasi** — Peta tertutup, kamu bermain dalam mode first-person. Gunakan senter dan peta singkat secara strategis.
- **Koleksi Kunci** — 3 kunci berwarna tersebar di labirin. Setiap kunci berisi 1 digit kode.
- **Input Kode** — Setelah semua kunci terkumpul, masukkan 3 digit kode di pintu keluar (urutan: Merah → Biru → Kuning).
- **Timer** — Kamu memiliki 180 detik untuk menyelesaikan semuanya.

## Kontrol

| Input | Aksi |
|-------|------|
| `W A S D` | Gerak maju / kiri / mundur / kanan |
| `Mouse` | Kamera / Lihat sekeliling |
| `F` | Nyalakan senter (3x, 5 detik) |
| `M` | Buka peta singkat (3x, 4 detik) |
| `E` | Ambil kunci (saat berada di atas kunci) |
| `ENTER` | Input kode di pintu exit |
| `ESC` | Keluar dari game |

### Menu

| Input | Aksi |
|-------|------|
| `Arrow UP/DOWN` | Navigasi menu |
| `ENTER` | Pilih opsi |
| `ESC` | Kembali / Keluar |

## Quick Start

Jalankan satu script untuk install dependencies dan compile sekaligus:

```powershell
.\setup.ps1
```

Setelah selesai, jalankan game:

```powershell
$env:Path = "C:\msys64\mingw64\bin;$env:Path"
.\memory_maze_game.exe
```

> Script membutuhkan PowerShell dan MSYS2 yang sudah terinstall di `C:\msys64`. Jika belum punya MSYS2, install dari [msys2.org](https://www.msys2.org/).

## Compile & Run (Manual)

### Prerequisites

- **MSYS2** dengan MinGW64
- Paket yang terinstall:
  - `mingw-w64-x86_64-freeglut`
  - `mingw-w64-x86_64-opengl`
  - `mingw-w64-x86_64-glu`

Install paket yang dibutuhkan:
```bash
pacman -S mingw-w64-x86_64-freeglut mingw-w64-x86_64-opengl mingw-w64-x86_64-glu
```

### Compile

```powershell
C:\msys64\mingw64\bin\g++.exe -std=c++17 -g `
    src\game_state.cpp `
    src\game_logic.cpp `
    src\render_helpers.cpp `
    src\render_maze.cpp `
    src\render_hud.cpp `
    src\render_menu.cpp `
    src\render_scene.cpp `
    src\input.cpp `
    src\update.cpp `
    src\main.cpp `
    -o memory_maze_game.exe `
    -lfreeglut -lopengl32 -lglu32
```

### Run

```powershell
$env:Path = "C:\msys64\mingw64\bin;$env:Path"
.\memory_maze_game.exe
```

> Pastikan `freeglut.dll` ada di PATH saat menjalankan executable.

## Struktur Project

```
tugas-besar-gti/
├── src/
│   ├── game.h              — Header utama: deklarasi, struct, enum, konstanta
│   ├── game_state.cpp      — Global state, utility functions, texture loading
│   ├── game_logic.cpp      — Game logic: spawn, kode penempatan, reset
│   ├── render_helpers.cpp  — Fungsi rendering 2D generik (kotak, lingkaran, teks)
│   ├── render_maze.cpp     — Rendering 3D maze: dinding, lantai, langit-langit, pencahayaan
│   ├── render_hud.cpp      — Gameplay HUD: timer, status, panel kunci, input kode, win/lose
│   ├── render_menu.cpp     — Menu utama dan panel info cara bermain
│   ├── render_scene.cpp    — Orkestrasi scene: display(), kamera, pemanggilan render
│   ├── input.cpp           — Input handling: keyboard (keyDown, keyUp, specialKey), mouse
│   ├── update.cpp          — Game loop: update() (timer, fisika, state), reshape()
│   ├── main.cpp            — Entry point: GLUT init, initGL(), main()
│   └── stb_image.h         — Library header-only untuk loading texture
├── assets/
│   └── textures/           — Texture files (stone_wall.jpg, stone_floor.jpg)
├── AGENTS.md               — Konfigurasi untuk OpenCode AI agent
└── README.md               — Dokumentasi ini
```

### Tanggung Jawab Per File

| File | Tanggung Jawab |
|------|----------------|
| `game.h` | Header sentral yang di-include oleh semua `.cpp`. Berisi `extern` globals, struct, enum, dan prototipe fungsi. |
| `game_state.cpp` | Definisi semua variabel global (state game, timer, kode, kamera), utility (toRad, isWalkable), dan loading texture. |
| `game_logic.cpp` | Logika game murni: BFS dari exit, penempatan spawn acak, penempatan kode, reset game. |
| `render_helpers.cpp` | Fungsi rendering 2D yang dipakai bersama oleh HUD dan menu: `drawFilledRect`, `drawFilledCircle`, `drawText`, `textWidth`. |
| `render_maze.cpp` | Rendering 3D scene: dinding, lantai, langit-langit, marker exit/spawn, orb kunci, setup pencahayaan dan fog. |
| `render_hud.cpp` | Overlay 2D saat gameplay: banner state, bar timer, panel item (senter, peta, kunci), crosshair, pesan status, input kode, win/lose. |
| `render_menu.cpp` | Menu utama (judul, tombol MULAI/INFO/KELUAR) dan overlay info cara bermain. |
| `render_scene.cpp` | Fungsi `display()` yang mengorkestrasi seluruh rendering: setup kamera, panggil drawMaze, drawHUD. |
| `input.cpp` | Semua input handler: `keyDown`, `keyUp`, `specialKey` (arrow keys), `mouseMove` (kamera first-person). |
| `update.cpp` | Game loop: `update()` menghitung delta time, update timer, pergerakan player, deteksi state. `reshape()` untuk resize window. |
| `main.cpp` | Entry point: inisialisasi GLUT, `initGL()`, registrasi callback, `glutMainLoop()`. |

## Konfigurasi Game

Konstanta yang bisa diubah di `game.h` atau `game_state.cpp`:

| Konstanta | Nilai | Deskripsi |
|-----------|-------|-----------|
| `WIN_W` | 1280 | Lebar window |
| `WIN_H` | 720 | Tinggi window |
| `ROWS` / `COLS` | 15 | Ukuran maze (sel) |
| `CELL` | 2.0f | Ukuran 1 sel (unit OpenGL) |
| `W_HEIGHT` | 3.0f | Tinggi dinding |
| `MEMORIZE_DURATION` | 7.0f | Durasi fase memorisasi (detik) |
| `GAME_DURATION` | 180.0f | Durasi total permainan (detik) |
| `FLASHLIGHT_USES` | 3 | Jumlah penggunaan senter |
| `FLASHLIGHT_DURATION` | 5.0f | Durasi senter aktif (detik) |
| `MAP_REVEAL_USES` | 3 | Jumlah penggunaan peta |
| `MAP_REVEAL_DURATION` | 4.0f | Durasi peta terbuka (detik) |
| `COLLISION_MARGIN` | 0.4f | Margin deteksi tabrakan player |

## Dependencies

- [freeglut](http://freeglut.sourceforge.net/) — Implementasi GLUT untuk windowing dan input
- **OpenGL** — Rendering 3D
- **GLU** — OpenGL Utility Library (perspektif, kamera)
- [stb_image](https://github.com/nothings/stb) — Header-only library untuk loading gambar (sudah disertakan di `src/stb_image.h`)

Tidak ada build system (CMake/Makefile), tidak ada package manager, tidak ada test framework.
