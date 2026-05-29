# Graph Report - .  (2026-05-29)

## Corpus Check
- cluster-only mode — file stats not available

## Summary
- 264 nodes · 714 edges · 17 communities (15 shown, 2 thin omitted)
- Extraction: 93% EXTRACTED · 7% INFERRED · 0% AMBIGUOUS · INFERRED: 47 edges (avg confidence: 0.8)
- Token cost: 0 input · 0 output

## Graph Freshness
- Built from commit: `a5983a3a`
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
1. `stbi__get8()` - 36 edges
2. `stbi__err()` - 26 edges
3. `stbi__get16be()` - 23 edges
4. `stbi__load_main()` - 22 edges
5. `stbi__rewind()` - 21 edges
6. `stbi__malloc_mad2()` - 16 edges
7. `stbi__parse_png_file()` - 16 edges
8. `stbi__malloc()` - 15 edges
9. `stbi__info_main()` - 14 edges
10. `display()` - 13 edges

## Surprising Connections (you probably didn't know these)
- `display()` --calls--> `toRad()`  [INFERRED]
  src/render_scene.cpp → src/game_state.cpp
- `drawHUD()` --calls--> `isPlayerAt()`  [INFERRED]
  src/render_hud.cpp → src/game_state.cpp
- `drawHUD()` --calls--> `allCodesCollected()`  [INFERRED]
  src/render_hud.cpp → src/game_state.cpp
- `drawHUD()` --calls--> `codeIndexAtPlayer()`  [INFERRED]
  src/render_hud.cpp → src/game_state.cpp
- `drawHUD()` --calls--> `drawOutlinedCircle()`  [INFERRED]
  src/render_hud.cpp → src/render_helpers.cpp

## Communities (17 total, 2 thin omitted)

### Community 0 - "Community 0"
Cohesion: 0.14
Nodes (39): stbi__at_eof(), stbi__check_png_header(), stbi__compute_transparency(), stbi__de_iphone(), stbi__decode_jpeg_header(), stbi__decode_jpeg_image(), stbi__err(), stbi__get16be() (+31 more)

### Community 1 - "Community 1"
Cohesion: 0.07
Nodes (14): stbi__build_fast_ac(), stbi__clamp(), stbi__cleanup_jpeg(), stbi__copyval(), stbi__cpuid3(), stbi__free_jpeg_components(), stbi__idct_block(), stbi__jpeg_dequantize() (+6 more)

### Community 2 - "Community 2"
Cohesion: 0.11
Nodes (23): computeReachableFromExit(), isCellUsedForCode(), placeCodes(), placeRandomSpawn(), resetGame(), allCodesCollected(), buildChecker(), codeIndexAtPlayer() (+15 more)

### Community 3 - "Community 3"
Cohesion: 0.16
Nodes (22): drawFilledCircle(), drawFilledRect(), drawOutlinedCircle(), drawText(), textWidth(), textWidth12(), textWidth18(), drawHUD() (+14 more)

### Community 4 - "Community 4"
Cohesion: 0.12
Nodes (26): load_jpeg_image(), stbi__addsizes_valid(), stbi__blinn_8x8(), stbi__compute_y(), stbi__convert_format(), stbi__create_png_alpha_expand8(), stbi__create_png_image(), stbi__expand_png_palette() (+18 more)

### Community 5 - "Community 5"
Cohesion: 0.20
Nodes (16): stbi__bmp_test(), stbi__do_png(), stbi__gif_test(), stbi__gif_test_raw(), stbi__hdr_info(), stbi__hdr_test(), stbi__jpeg_test(), stbi__load_main() (+8 more)

### Community 6 - "Community 6"
Cohesion: 0.14
Nodes (3): initGL, display, update

### Community 7 - "Community 7"
Cohesion: 0.21
Nodes (14): stbi__convert_16_to_8(), stbi__convert_8_to_16(), stbi__float_postprocess(), stbi_info_from_memory(), stbi_is_hdr_from_memory(), stbi__ldr_to_hdr(), stbi_load_16_from_memory(), stbi__load_and_postprocess_16bit() (+6 more)

### Community 8 - "Community 8"
Cohesion: 0.31
Nodes (13): stbi__bit_reverse(), stbi__build_huffman(), stbi__compute_huffman_codes(), stbi__fill_bits(), stbi__parse_huffman_block(), stbi__parse_uncompressed_block(), stbi__parse_zlib(), stbi__parse_zlib_header() (+5 more)

### Community 9 - "Community 9"
Cohesion: 0.25
Nodes (11): stbi__fopen(), stbi_info(), stbi_info_from_file(), stbi_is_16_bit(), stbi_is_16_bit_from_file(), stbi_is_hdr(), stbi_is_hdr_from_file(), stbi_load() (+3 more)

### Community 10 - "Community 10"
Cohesion: 0.33
Nodes (9): stbi__addints_valid(), stbi__extend_receive(), stbi__grow_buffer_unsafe(), stbi__jpeg_decode_block(), stbi__jpeg_get_bit(), stbi__jpeg_huff_decode(), stbi__jpeg_reset(), stbi__mul2shorts_valid() (+1 more)

### Community 11 - "Community 11"
Cohesion: 0.25
Nodes (9): stbi__bitcount(), stbi__bmp_info(), stbi__bmp_load(), stbi__bmp_parse_header(), stbi__bmp_set_mask_defaults(), stbi__bmp_test_raw(), stbi__get32le(), stbi__high_bit() (+1 more)

### Community 12 - "Community 12"
Cohesion: 0.25
Nodes (8): stbi_info_from_callbacks(), stbi_is_16_bit_from_callbacks(), stbi_is_16_bit_from_memory(), stbi__is_16_main(), stbi_is_hdr_from_callbacks(), stbi__pnm_is16(), stbi__refill_buffer(), stbi__start_callbacks()

### Community 13 - "Community 13"
Cohesion: 0.33
Nodes (6): stbi__do_zlib(), stbi_zlib_decode_buffer(), stbi_zlib_decode_malloc(), stbi_zlib_decode_malloc_guesssize(), stbi_zlib_decode_noheader_buffer(), stbi_zlib_decode_noheader_malloc()

## Knowledge Gaps
- **3 isolated node(s):** `resetGame`, `update`, `initGL`
  These have ≤1 connection - possible missing edges or undocumented components.
- **2 thin communities (<3 nodes) omitted from report** — run `graphify query` to explore isolated nodes.

## Suggested Questions
_Questions this graph is uniquely positioned to answer:_

- **Why does `toRad()` connect `Community 2` to `Community 3`?**
  _High betweenness centrality (0.031) - this node is a cross-community bridge._
- **Why does `display()` connect `Community 3` to `Community 2`?**
  _High betweenness centrality (0.029) - this node is a cross-community bridge._
- **Why does `drawHUD()` connect `Community 3` to `Community 2`?**
  _High betweenness centrality (0.023) - this node is a cross-community bridge._
- **What connects `resetGame`, `update`, `initGL` to the rest of the system?**
  _3 weakly-connected nodes found - possible documentation gaps or missing edges._
- **Should `Community 0` be split into smaller, more focused modules?**
  _Cohesion score 0.1417004048582996 - nodes in this community are weakly interconnected._
- **Should `Community 1` be split into smaller, more focused modules?**
  _Cohesion score 0.07007575757575757 - nodes in this community are weakly interconnected._
- **Should `Community 2` be split into smaller, more focused modules?**
  _Cohesion score 0.10887096774193548 - nodes in this community are weakly interconnected._