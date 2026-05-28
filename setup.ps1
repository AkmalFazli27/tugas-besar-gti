# setup.ps1 — Memory Maze: Install dependencies & compile
# Jalankan dari root project: .\setup.ps1

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "  Memory Maze — Setup & Build" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

# 1. Cek MSYS2
if (-not (Test-Path "C:\msys64\mingw64\bin\g++.exe")) {
    Write-Host "[ERROR] MSYS2 MinGW64 tidak ditemukan di C:\msys64" -ForegroundColor Red
    Write-Host "Install MSYS2 dari: https://www.msys2.org/" -ForegroundColor Yellow
    Write-Host "Setelah install, jalankan:" -ForegroundColor Yellow
    Write-Host '  pacman -S mingw-w64-x86_64-gcc' -ForegroundColor White
    exit 1
}
Write-Host "[OK] MSYS2 ditemukan" -ForegroundColor Green

# 2. Install dependencies
Write-Host ""
Write-Host "Menginstall dependencies (freeglut, opengl, glu)..." -ForegroundColor Yellow
C:\msys64\mingw64\bin\pacman.exe -S --noconfirm mingw-w64-x86_64-freeglut mingw-w64-x86_64-opengl mingw-w64-x86_64-glu
if ($LASTEXITCODE -ne 0) {
    Write-Host "[WARN] Gagal install beberapa paket. Mungkin sudah terinstall." -ForegroundColor Yellow
} else {
    Write-Host "[OK] Dependencies terinstall" -ForegroundColor Green
}

# 3. Compile
Write-Host ""
Write-Host "Compiling Memory Maze..." -ForegroundColor Yellow

$srcFiles = @(
    "src\game_state.cpp",
    "src\game_logic.cpp",
    "src\render_helpers.cpp",
    "src\render_maze.cpp",
    "src\render_hud.cpp",
    "src\render_menu.cpp",
    "src\render_scene.cpp",
    "src\input.cpp",
    "src\update.cpp",
    "src\main.cpp"
)

# Cek semua file sumber ada
foreach ($f in $srcFiles) {
    if (-not (Test-Path $f)) {
        Write-Host "[ERROR] File tidak ditemukan: $f" -ForegroundColor Red
        exit 1
    }
}

& "C:\msys64\mingw64\bin\g++.exe" -std=c++17 -g $srcFiles -o memory_maze_game.exe -lfreeglut -lopengl32 -lglu32

if ($LASTEXITCODE -ne 0) {
    Write-Host "[ERROR] Compile gagal!" -ForegroundColor Red
    exit 1
}

Write-Host "[OK] Compile berhasil: memory_maze_game.exe" -ForegroundColor Green

# 4. Selesai
Write-Host ""
Write-Host "========================================" -ForegroundColor Cyan
Write-Host "  Setup selesai!" -ForegroundColor Green
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""
Write-Host "Jalankan game dengan:" -ForegroundColor White
Write-Host '  $env:Path = "C:\msys64\mingw64\bin;$env:Path"' -ForegroundColor White
Write-Host '  .\memory_maze_game.exe' -ForegroundColor White
Write-Host ""
