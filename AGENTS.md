# Memory Maze — C++ OpenGL Game

## Build

Uses **MSYS2 MinGW64** (not MSVC). Compile with:

```
C:\msys64\mingw64\bin\g++.exe -std=c++17 -g src\game_state.cpp src\game_logic.cpp src\render.cpp src\main.cpp -o memory_maze_game.exe -lfreeglut -lopengl32 -lglu32
```

The older `maze_structure.cpp` (Phase 1) is a single-file build:
```
C:\msys64\mingw64\bin\g++.exe -std=c++17 -g src\maze_structure.cpp -o memory_maze.exe -lfreeglut -lopengl32 -lglu32
```

## Run

`freeglut.dll` must be on PATH before running:

```
$env:Path = "C:\msys64\mingw64\bin;$env:Path"; .\memory_maze_game.exe
```

## Source Files

- `src/maze_structure.cpp` — Phase 1: basic maze structure, first-person camera, key collection (single file)
- `src/main.cpp` — Phase 2 entry point: GLUT callbacks and `main()`. Includes `game.h`
- `src/game.h` — Shared declarations: `extern` globals, structs, enums, function prototypes
- `src/game_state.cpp` — Global variable definitions + utility functions
- `src/game_logic.cpp` — Game logic: spawn placement, code placement, reset
- `src/render.cpp` — All rendering: walls, floor, ceiling, markers, lighting, HUD

Both phases are standalone executables built without any build system (no CMake/Makefile).

## Dependencies

- **freeglut** (via MSYS2 `mingw-w64-x86_64-freeglut`)
- **OpenGL** / **GLU** (via MSYS2 `mingw-w64-x86_64-opengl` and `mingw-w64-x86_64-glu`)

No package manager, no test framework.

## OpenCode Config Notes

The `opencode.json` loads many generic agents/skills for web/TypeScript projects that do **not** apply here (TDD, Playwright E2E, frontend patterns, API design, etc.). The `instructions/INSTRUCTIONS.md` is similarly generic boilerplate. Ignore or remove them when working in this repo. The relevant subagents are `cpp-reviewer` and `cpp-build-resolver`.
