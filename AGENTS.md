# Memory Maze — C++ OpenGL Game

## Build & Run

Uses **MSYS2 MinGW64** (not MSVC). No build system, no CMake.

```powershell
# Compile (all 10 source files)
C:\msys64\mingw64\bin\g++.exe -std=c++17 -g src\game_state.cpp src\game_logic.cpp src\render_helpers.cpp src\render_maze.cpp src\render_hud.cpp src\render_menu.cpp src\render_scene.cpp src\input.cpp src\update.cpp src\main.cpp -o memory_maze_game.exe -lfreeglut -lopengl32 -lglu32

# Run (freeglut.dll must be on PATH)
$env:Path = "C:\msys64\mingw64\bin;$env:Path"; .\memory_maze_game.exe
```

Quick setup: `.\setup.ps1` installs deps + compiles.

## Architecture

Single-header C++ game using legacy OpenGL (fixed-function pipeline, no shaders).

- **`game.h`** — Only header. All globals are `extern`; definitions in `game_state.cpp`. Every `.cpp` includes only `game.h`.
- **`game_state.cpp`** — Global state, utility functions, texture loading (uses `stb_image.h`).
- **`game_logic.cpp`** — Pure logic: BFS reachability, spawn/code placement, `resetGame()`.
- **`render_*.cpp`** — Rendering split by concern:
  - `render_helpers.cpp` — 2D primitives (`drawFilledRect`, `drawText`, `textWidth`)
  - `render_maze.cpp` — 3D scene (walls, floor, ceiling, markers, lighting, fog)
  - `render_hud.cpp` — Gameplay HUD (timer, status, key panel, code input, win/lose)
  - `render_menu.cpp` — Main menu + info overlay
  - `render_scene.cpp` — `display()`: camera setup, orchestrates all draw calls
- **`input.cpp`** — `keyDown`, `keyUp`, `specialKey`, `mouseMove`
- **`update.cpp`** — `update()` (game loop, ~60fps via `glutTimerFunc`), `reshape()`
- **`main.cpp`** — `initGL()`, GLUT callback registration, `glutMainLoop()`

## State Machine

```
STATE_MENU → STATE_MEMORIZE → STATE_PLAY → STATE_ENTER_CODE → STATE_WIN/LOSE
                                          ↑_________________________|
                                          (ENTER on win/lose → back to menu)
```

- `STATE_MEMORIZE`: top-down map view, 7s timer, player cannot move
- `STATE_PLAY`: first-person, movement + camera active
- `mapRevealActive`: player movement + camera locked (press M to briefly show map)
- `STATE_ENTER_CODE`: 3-digit colored code input (Red→Blue→Yellow)

## Key Conventions

- **No `#include` except `game.h`** in source files. `stb_image.h` is only included in `game_state.cpp`.
- **Maze grid**: `int maze[ROWS][COLS]` (15×15). 1 = wall, 0 = walkable. Uses 11 templates (`TEMPLATES[11][15][15]` in `game_state.cpp`), randomly selected by `generateMaze()`.
- **Spawn/exit placement**: Random from walkable cells, Manhattan distance ≥ 10 apart. Set in `generateMaze()`.
- **Key placement**: 3 keys placed by `placeCodes()` with Manhattan distance ≥ 5 from spawn, exit, and each other. Fallback if no candidates meet distance.
- **Coordinate system**: `cam.x/z` = world position; `playerRow()/playerCol()` = grid cell (`cam.z/CELL`, `cam.x/CELL`).
- **Overlay rendering**: `drawExit(true)`, `drawCodeSpots(true)`, `drawSpawnOverlay()` render on top of maze (y = `W_HEIGHT + 0.05f`) for map view.
- **viewMode**: 0 = first-person, 1 = top-down map (set during memorize and map reveal).
- **Textures**: fallback to procedural checkerboard if `assets/textures/*.jpg` missing.

## Plan Mode

When entering plan mode, immediately create a `todowrite` checklist. Mark items `in_progress` one at a time, `completed` as done.

## Not Present

No tests, no lint, no formatter, no CI. No `opencode.json` in repo root. Skills in `.opencode/skills/` are web/TS-focused and irrelevant to this C++ project.

## Forbidden Access

Agent must NEVER:

* read `.env`
* read SSH keys
* read files inside `.git`
* read browser cookies
* access password managers
* access files outside the current workspace
* access cloud credentials
* access API keys or tokens

Forbidden paths:

* ~/.ssh
* ~/.aws
* ~/.config
* ~/.gnupg
* ~/.npmrc
* ~/.gitconfig
* .env
* .env.*
* secrets/
* credentials/

---

## Command Restrictions

Agent must NEVER execute:

* rm -rf
* sudo commands
* curl | bash
* wget | bash
* powershell remote scripts
* chmod on system files
* git push
* git commit
* docker commands
* kubectl commands

Agent must ask for confirmation before:

* installing packages
* modifying dependencies
* deleting files
* changing environment variables
* running shell scripts

---

## Network Restrictions

Agent must NEVER:

* upload source code to external services
* send files to unknown endpoints
* use non-official AI proxies
* access arbitrary URLs

Allowed AI providers:

* api.openai.com
* openrouter.ai
* generativelanguage.googleapis.com

---

## Workspace Restrictions

Agent must only operate inside the current workspace directory.

Agent must not traverse parent directories.

---

## Safe Coding Rules

* Never expose secrets in logs
* Never print tokens
* Never hardcode credentials
* Always mask sensitive values
* Prefer mock/example values

---

## Approval Policy

Require explicit user approval before:

* file deletion
* dependency installation
* network requests
* git operations
* database migrations
* system modifications

## graphify

This project has a knowledge graph at graphify-out/ with god nodes, community structure, and cross-file relationships.

When the user types `/graphify`, invoke the `skill` tool with `skill: "graphify"` before doing anything else.

Rules:
- For codebase questions, first run `graphify query "<question>"` when graphify-out/graph.json exists. Use `graphify path "<A>" "<B>"` for relationships and `graphify explain "<concept>"` for focused concepts. These return a scoped subgraph, usually much smaller than GRAPH_REPORT.md or raw grep output.
- Dirty graphify-out/ files are expected after hooks or incremental updates; dirty graph files are not a reason to skip graphify. Only skip graphify if the task is about stale or incorrect graph output, or the user explicitly says not to use it.
- If graphify-out/wiki/index.md exists, use it for broad navigation instead of raw source browsing.
- Read graphify-out/GRAPH_REPORT.md only for broad architecture review or when query/path/explain do not surface enough context.
- After modifying code, run `graphify update .` to keep the graph current (AST-only, no API cost).
