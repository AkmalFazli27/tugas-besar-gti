# Memory Maze — C++ OpenGL Game

## Build

Uses **MSYS2 MinGW64** (not MSVC). Compile with:

```
C:\msys64\mingw64\bin\g++.exe -std=c++17 -g src\game_state.cpp src\game_logic.cpp src\render_helpers.cpp src\render_maze.cpp src\render_hud.cpp src\render_menu.cpp src\render_scene.cpp src\input.cpp src\update.cpp src\main.cpp -o memory_maze_game.exe -lfreeglut -lopengl32 -lglu32
```

## Run

`freeglut.dll` must be on PATH before running:

```
$env:Path = "C:\msys64\mingw64\bin;$env:Path"; .\memory_maze_game.exe
```

## Source Files

- `src/main.cpp` — Entry point: GLUT init, `initGL()`, `main()`
- `src/game.h` — Shared declarations: `extern` globals, structs, enums, function prototypes
- `src/game_state.cpp` — Global variable definitions, utility functions, texture loading
- `src/game_logic.cpp` — Game logic: spawn placement, code placement, reset
- `src/render_helpers.cpp` — Shared 2D rendering helpers: `drawFilledRect`, `drawText`, etc.
- `src/render_maze.cpp` — 3D maze rendering: walls, floor, ceiling, markers, lighting
- `src/render_hud.cpp` — Gameplay HUD: timer, status, key panel, code input, win/lose overlay
- `src/render_menu.cpp` — Menu screen: main menu, info overlay
- `src/render_scene.cpp` — Scene orchestration: `display()` (camera setup + draw calls)
- `src/input.cpp` — Input handling: keyboard (`keyDown`, `keyUp`, `specialKey`), mouse (`mouseMove`)
- `src/update.cpp` — Game loop: `update()` (timer, physics, state transitions), `reshape()`

All source files are standalone executables built without any build system (no CMake/Makefile).

## Dependencies

- **freeglut** (via MSYS2 `mingw-w64-x86_64-freeglut`)
- **OpenGL** / **GLU** (via MSYS2 `mingw-w64-x86_64-opengl` and `mingw-w64-x86_64-glu`)

No package manager, no test framework.

## OpenCode Config Notes

The `opencode.json` loads many generic agents/skills for web/TypeScript projects that do **not** apply here (TDD, Playwright E2E, frontend patterns, API design, etc.). The `instructions/INSTRUCTIONS.md` is similarly generic boilerplate. Ignore or remove them when working in this repo. The relevant subagents are `cpp-reviewer` and `cpp-build-resolver`.

## Plan Mode Todo Rules

- When operating in **plan mode**, always create or refresh a todo list using the `todowrite` tool.
- Convert the plan into actionable checklist items (avoid vague goals).
- When switching to **build mode**, update the same todo list and mark items `completed` as soon as the work for that item is done.
- Complete and check off todo items **one by one** as each item finishes; do not wait until all work is done to update the checklist.
- Keep the todo list current so the sidebar checklist reflects real progress.
