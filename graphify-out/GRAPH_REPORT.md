# Graph Report - tugas-besar-gti  (2026-05-30)

## Corpus Check
- 16 files · ~65,116 words
- Verdict: corpus is large enough that graph structure adds value.

## Summary
- 348 nodes · 915 edges · 17 communities (15 shown, 2 thin omitted)
- Extraction: 95% EXTRACTED · 5% INFERRED · 0% AMBIGUOUS · INFERRED: 49 edges (avg confidence: 0.8)
- Token cost: 0 input · 0 output

## Graph Freshness
- Built from commit: `2cae6d83`
- Run `git rev-parse HEAD` and compare to check if the graph is stale.
- Run `graphify update .` after code changes (no API cost).

## Community Hubs (Navigation)
- [[_COMMUNITY_Community 0|Community 0]]
- [[_COMMUNITY_Community 1|Community 1]]
- [[_COMMUNITY_Community 2|Community 2]]
- [[_COMMUNITY_Community 3|Community 3]]
- [[_COMMUNITY_Community 4|Community 4]]
- [[_COMMUNITY_Community 5|Community 5]]
- [[_COMMUNITY_Community 6|Community 6]]
- [[_COMMUNITY_Community 7|Community 7]]
- [[_COMMUNITY_Community 8|Community 8]]
- [[_COMMUNITY_Community 9|Community 9]]
- [[_COMMUNITY_Community 10|Community 10]]
- [[_COMMUNITY_Community 11|Community 11]]
- [[_COMMUNITY_Community 12|Community 12]]
- [[_COMMUNITY_Community 13|Community 13]]
- [[_COMMUNITY_Community 14|Community 14]]
- [[_COMMUNITY_Community 16|Community 16]]

## God Nodes (most connected - your core abstractions)
1. `stbi__get8()` - 39 edges
2. `stbi__err()` - 31 edges
3. `stbi__rewind()` - 25 edges
4. `stbi__get16be()` - 24 edges
5. `stbi__load_main()` - 22 edges
6. `stbi__malloc()` - 20 edges
7. `stbi__parse_png_file()` - 19 edges
8. `stbi__malloc_mad2()` - 17 edges
9. `stbi__bmp_load()` - 16 edges
10. `stbi__mad2sizes_valid()` - 14 edges

## Surprising Connections (you probably didn't know these)
- `drawHUD()` --calls--> `drawOutlinedCircle()`  [INFERRED]
  src/render_hud.cpp → src/render_helpers.cpp
- `display()` --calls--> `drawSpawnOverlay()`  [INFERRED]
  src/render_scene.cpp → src/render_maze.cpp
- `computeReachableFromExit()` --calls--> `isCellWalkable()`  [INFERRED]
  src/game_logic.cpp → src/game_state.cpp
- `placeRandomSpawn()` --calls--> `randRange()`  [INFERRED]
  src/game_logic.cpp → src/game_state.cpp
- `isCellUsedForCode()` --calls--> `isCellWalkable()`  [INFERRED]
  src/game_logic.cpp → src/game_state.cpp

## Communities (17 total, 2 thin omitted)

### Community 0 - "Community 0"
Cohesion: 0.08
Nodes (60): stbi__at_eof(), stbi__bitcount(), stbi__bmp_info(), stbi__bmp_load(), stbi__bmp_parse_header(), stbi__bmp_set_mask_defaults(), stbi__bmp_test(), stbi__bmp_test_raw() (+52 more)

### Community 1 - "Community 1"
Cohesion: 0.07
Nodes (8): stbi__clamp(), stbi__cpuid3(), stbi__idct_block(), stbi__jpeg_dequantize(), stbi__jpeg_finish(), stbi__jpeg_test(), stbi__setup_jpeg(), stbi__sse2_available()

### Community 2 - "Community 2"
Cohesion: 0.07
Nodes (46): computeReachableFromExit(), generateMaze(), isCellUsedForCode(), placeCodes(), placeRandomSpawn(), resetGame(), allCodesCollected(), buildChecker() (+38 more)

### Community 3 - "Community 3"
Cohesion: 0.10
Nodes (20): code:powershell (.\setup.ps1), code:powershell ($env:Path = "C:\msys64\mingw64\bin;$env:Path"), code:bash (pacman -S mingw-w64-x86_64-freeglut mingw-w64-x86_64-opengl ), code:powershell (C:\msys64\mingw64\bin\g++.exe -std=c++17 -g `), code:powershell ($env:Path = "C:\msys64\mingw64\bin;$env:Path"), code:block6 (tugas-besar-gti/), Compile, Compile & Run (Manual) (+12 more)

### Community 4 - "Community 4"
Cohesion: 0.14
Nodes (33): load_jpeg_image(), stbi__addsizes_valid(), stbi__blinn_8x8(), stbi__cleanup_jpeg(), stbi__compute_y(), stbi__compute_y_16(), stbi__convert_format(), stbi__convert_format16() (+25 more)

### Community 5 - "Community 5"
Cohesion: 0.12
Nodes (16): Approval Policy, Architecture, Build & Run, code:powershell (# Compile (all 10 source files)), code:block2 (STATE_MENU → STATE_MEMORIZE → STATE_PLAY → STATE_ENTER_CODE ), Command Restrictions, Forbidden Access, graphify (+8 more)

### Community 6 - "Community 6"
Cohesion: 0.14
Nodes (3): initGL, display, update

### Community 7 - "Community 7"
Cohesion: 0.18
Nodes (18): stbi__convert_16_to_8(), stbi__convert_8_to_16(), stbi__float_postprocess(), stbi_info_from_memory(), stbi_is_hdr_from_memory(), stbi_load_16_from_callbacks(), stbi_load_16_from_memory(), stbi__load_and_postprocess_16bit() (+10 more)

### Community 8 - "Community 8"
Cohesion: 0.17
Nodes (25): stbi__bit_reverse(), stbi__bitreverse16(), stbi__build_fast_ac(), stbi__build_huffman(), stbi__compute_huffman_codes(), stbi__decode_jpeg_header(), stbi__decode_jpeg_image(), stbi__err() (+17 more)

### Community 9 - "Community 9"
Cohesion: 0.21
Nodes (13): stbi__fopen(), stbi_info(), stbi_info_from_file(), stbi_is_16_bit(), stbi_is_16_bit_from_file(), stbi_is_hdr(), stbi_is_hdr_from_file(), stbi_load() (+5 more)

### Community 10 - "Community 10"
Cohesion: 0.39
Nodes (12): stbi__addints_valid(), stbi__extend_receive(), stbi__grow_buffer_unsafe(), stbi__jpeg_decode_block(), stbi__jpeg_decode_block_prog_ac(), stbi__jpeg_decode_block_prog_dc(), stbi__jpeg_get_bit(), stbi__jpeg_get_bits() (+4 more)

### Community 11 - "Community 11"
Cohesion: 0.22
Nodes (8): 1) Kamera — sensitivitas mouse lebih kecil, 2) Gerak pemain lebih lambat, 3) Tekstur bebatuan untuk dinding & lantai, 4) Bentuk key menjadi kunci, Catatan Teknis, Plan — Update Berikutnya (Memory Maze), Rekomendasi Update Tambahan (Agar lebih menarik & menantang), Tujuan Utama

### Community 12 - "Community 12"
Cohesion: 0.25
Nodes (8): stbi_info_from_callbacks(), stbi_is_16_bit_from_callbacks(), stbi_is_16_bit_from_memory(), stbi__is_16_main(), stbi_loadf_from_callbacks(), stbi__pnm_is16(), stbi__refill_buffer(), stbi__start_callbacks()

### Community 13 - "Community 13"
Cohesion: 0.13
Nodes (17): stbi__check_png_header(), stbi__compute_transparency(), stbi__compute_transparency16(), stbi__de_iphone(), stbi__do_zlib(), stbi__expand_png_palette(), stbi__get32be(), stbi__get_chunk_header() (+9 more)

## Knowledge Gaps
- **34 isolated node(s):** `code:powershell (# Compile (all 10 source files))`, `Architecture`, `code:block2 (STATE_MENU → STATE_MEMORIZE → STATE_PLAY → STATE_ENTER_CODE )`, `Key Conventions`, `Plan Mode` (+29 more)
  These have ≤1 connection - possible missing edges or undocumented components.
- **2 thin communities (<3 nodes) omitted from report** — run `graphify query` to explore isolated nodes.

## Suggested Questions
_Questions this graph is uniquely positioned to answer:_

- **What connects `code:powershell (# Compile (all 10 source files))`, `Architecture`, `code:block2 (STATE_MENU → STATE_MEMORIZE → STATE_PLAY → STATE_ENTER_CODE )` to the rest of the system?**
  _34 weakly-connected nodes found - possible documentation gaps or missing edges._
- **Should `Community 0` be split into smaller, more focused modules?**
  _Cohesion score 0.0847457627118644 - nodes in this community are weakly interconnected._
- **Should `Community 1` be split into smaller, more focused modules?**
  _Cohesion score 0.0677361853832442 - nodes in this community are weakly interconnected._
- **Should `Community 2` be split into smaller, more focused modules?**
  _Cohesion score 0.06885245901639345 - nodes in this community are weakly interconnected._
- **Should `Community 3` be split into smaller, more focused modules?**
  _Cohesion score 0.09523809523809523 - nodes in this community are weakly interconnected._
- **Should `Community 4` be split into smaller, more focused modules?**
  _Cohesion score 0.13825757575757575 - nodes in this community are weakly interconnected._
- **Should `Community 5` be split into smaller, more focused modules?**
  _Cohesion score 0.11764705882352941 - nodes in this community are weakly interconnected._