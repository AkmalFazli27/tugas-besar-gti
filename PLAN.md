# Plan — Update Berikutnya (Memory Maze)

## Tujuan Utama
1) Perkecil sensitivitas mouse look
2) Perlambat gerak pemain (WASD)
3) Ganti tekstur dinding & lantai ke tema bebatuan
4) Ubah bentuk key dari bola menjadi bentuk kunci

---

## 1) Kamera — sensitivitas mouse lebih kecil
- Lokasi: `src/game_state.cpp` (nilai `mouseSensitivity`)
- Rencana: turunkan dari `0.25f` -> `0.18f` (bisa di-adjust)
- Verifikasi: mouse kanan/kiri tetap responsif tapi lebih halus

---

## 2) Gerak pemain lebih lambat
- Lokasi: `src/game.h` (Camera.speed) atau `src/game_state.cpp`
- Rencana: turunkan `cam.speed` dari `3.5f` -> `2.6f`
- Verifikasi: W/A/S/D terasa lebih berat, tapi masih playable

---

## 3) Tekstur bebatuan untuk dinding & lantai
- Lokasi: `src/game_state.cpp` -> `initTextures()`
- Rencana:
  - Ganti `buildChecker()` dengan tekstur gambar batu (PNG/BMP)
  - Tambah loader image (mis. `stb_image.h`) untuk load texture
  - Tetap pakai `texWall`, `texFloor`
- Verifikasi: dinding dan lantai menampilkan tekstur batu (tiled)

---

## 4) Bentuk key menjadi kunci
- Lokasi: `src/render.cpp` -> `drawKeyOrb`
- Rencana:
  - Ubah dari `glutSolidSphere` menjadi bentuk kunci 3D sederhana
  - Kombinasi: cylinder + box + ring kecil
  - Bisa pakai beberapa primitive OpenGL/GLU
- Verifikasi: key tampak seperti kunci beneran, bukan bola

---

## Rekomendasi Update Tambahan (Agar lebih menarik & menantang)
1) Stamina system
   - Sprint terbatas, recharge perlahan
2) Audio cue / footsteps
   - Bantu orientasi + suasana horror
3) Dynamic lighting
   - Flicker di lorong tertentu
4) Randomized maze variant
   - Replayability lebih tinggi

---

## Catatan Teknis
- Semua perubahan dilakukan bertahap:
  1) Sensitivitas & speed
  2) Tekstur batu
  3) Model key
  4) Update tambahan (opsional)
