// ============================================================
//  MEMORY MAZE — Game Logic
//  Tahap 2: memorisasi peta, gelap, flashlight, kode warna, timer
//
//  # Compile
//  C:\msys64\mingw64\bin\g++.exe memory_maze_game.cpp -o memory_maze_game.exe -lfreeglut -lopengl32 -lglu32
//
//  # Run (tambahkan PATH runtime freeglut)
//  $env:Path = "C:\msys64\mingw64\bin;$env:Path"
//  .\memory_maze_game.exe
// ============================================================

#include <GL/glut.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <queue>

// ============================================================
//  KONFIGURASI WINDOW
// ============================================================
const int   WIN_W    = 1280;
const int   WIN_H    = 720;
const char* WIN_NAME = "Memory Maze — Game";

// ============================================================
//  KONFIGURASI MAZE
// ============================================================
const int   ROWS      = 20;
const int   COLS      = 20;
const float CELL      = 2.0f;     // ukuran 1 sel (unit OpenGL)
const float W_HEIGHT  = 3.0f;     // tinggi dinding

//  Legenda:
//  1 = dinding
//  0 = lorong
//  S = spawn player (disimpan terpisah, di grid tetap 0)
//  E = exit (disimpan terpisah, di grid tetap 0)

int maze[ROWS][COLS] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,1},
    {1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,1},
    {1,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,1},
    {1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1},
    {1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,1},
    {1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,0,1},
    {1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,1},
    {1,0,1,1,1,0,1,1,1,1,1,0,1,1,1,0,1,1,0,1},
    {1,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,1},
    {1,0,1,0,1,1,1,0,1,0,1,1,1,0,1,0,1,1,1,1},
    {1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,1},
    {1,1,1,0,1,0,1,1,1,0,1,0,1,1,1,0,1,0,1,1},
    {1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,1},
    {1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1},
    {1,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,1},
    {1,0,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,0,1},
    {1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,1,0,1},
    {1,1,1,0,1,1,0,0,0,1,1,0,0,0,1,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

// Posisi spawn & exit di grid
int spawnRow = 1, spawnCol = 1;
const int EXIT_ROW  = 18, EXIT_COL  = 18;

// ============================================================
//  KONFIGURASI GAME
// ============================================================
const float MEMORIZE_DURATION  = 7.0f;
const float GAME_DURATION      = 180.0f;
const int   FLASHLIGHT_USES    = 3;
const float FLASHLIGHT_DURATION= 5.0f;
const int   MAP_REVEAL_USES    = 3;
const float MAP_REVEAL_DURATION= 4.0f;

// ============================================================
//  KAMERA FIRST-PERSON
// ============================================================
struct Camera {
    float x     = 0.0f;  // posisi world X
    float z     = 0.0f;  // posisi world Z
    float y     = 1.3f;  // tinggi mata
    float angle = 0.0f;  // arah hadap (derajat)
    float speed = 3.5f;
    float tSpeed= 80.0f;
} cam;

bool keys[256] = {};

// ============================================================
//  STATE GAME
// ============================================================
enum GameState {
    STATE_MEMORIZE,
    STATE_PLAY,
    STATE_ENTER_CODE,
    STATE_WIN,
    STATE_LOSE
};

GameState gameState = STATE_MEMORIZE;

// ============================================================
//  KODE WARNA
// ============================================================
enum CodeColor {
    CODE_RED = 0,
    CODE_BLUE = 1,
    CODE_YELLOW = 2,
    CODE_COUNT = 3
};

struct CodeSpot {
    int row = 0;
    int col = 0;
    int digit = 0;
    CodeColor color = CODE_RED;
    bool collected = false;
};

CodeSpot codeSpots[CODE_COUNT];
int codeDigits[CODE_COUNT] = {0};
bool codeCollected[CODE_COUNT] = {false};

const char* COLOR_NAMES[CODE_COUNT] = {"MERAH", "BIRU", "KUNING"};
const float COLOR_RGB[CODE_COUNT][3] = {
    {1.0f, 0.25f, 0.25f},
    {0.25f, 0.55f, 1.0f},
    {1.0f, 0.9f, 0.25f}
};

const CodeColor INPUT_ORDER[CODE_COUNT] = {CODE_RED, CODE_BLUE, CODE_YELLOW};

// ============================================================
//  TIMER & STATUS
// ============================================================
float memorizeTimeLeft = MEMORIZE_DURATION;
float gameTimeLeft = GAME_DURATION;

int   flashlightUses = FLASHLIGHT_USES;
bool  flashlightActive = false;
float flashlightTimeLeft = 0.0f;

int   mapRevealUses = MAP_REVEAL_USES;
bool  mapRevealActive = false;
float mapRevealTimeLeft = 0.0f;

bool reachableFromExit[ROWS][COLS] = {};

char statusMsg[128] = "";
float statusMsgTimer = 0.0f;

int enteredCodes[CODE_COUNT] = {-1, -1, -1};
int inputIndex = 0;

// ============================================================
//  MODE TAMPILAN
//  0 = First-Person (perspective)
//  1 = Top-Down (orthographic) — hanya untuk tahap hafalan
// ============================================================
int viewMode = 0;

// ============================================================
//  UTILITY
// ============================================================
float toRad(float deg) { return deg * 3.14159265f / 180.0f; }

int randRange(int minVal, int maxVal) {
    return minVal + (rand() % (maxVal - minVal + 1));
}

int playerRow() { return (int)(cam.z / CELL); }
int playerCol() { return (int)(cam.x / CELL); }

bool isWalkable(float wx, float wz) {
    int r = (int)(wz / CELL);
    int c = (int)(wx / CELL);
    if (r < 0 || r >= ROWS || c < 0 || c >= COLS) return false;
    return maze[r][c] == 0;
}

bool isCellWalkable(int r, int c) {
    if (r < 0 || r >= ROWS || c < 0 || c >= COLS) return false;
    return maze[r][c] == 0;
}

bool isPlayerAt(int r, int c) {
    return playerRow() == r && playerCol() == c;
}

bool allCodesCollected() {
    for (int i = 0; i < CODE_COUNT; i++)
        if (!codeCollected[i]) return false;
    return true;
}

void setStatus(const char* msg, float duration) {
    std::strncpy(statusMsg, msg, sizeof(statusMsg) - 1);
    statusMsg[sizeof(statusMsg) - 1] = '\0';
    statusMsgTimer = duration;
}

void resetInputCodes() {
    for (int i = 0; i < CODE_COUNT; i++) enteredCodes[i] = -1;
    inputIndex = 0;
}

void computeReachableFromExit() {
    for (int r = 0; r < ROWS; r++)
        for (int c = 0; c < COLS; c++)
            reachableFromExit[r][c] = false;

    if (!isCellWalkable(EXIT_ROW, EXIT_COL)) return;

    std::queue<std::pair<int, int>> q;
    q.push({EXIT_ROW, EXIT_COL});
    reachableFromExit[EXIT_ROW][EXIT_COL] = true;

    const int dr[4] = {-1, 1, 0, 0};
    const int dc[4] = {0, 0, -1, 1};

    while (!q.empty()) {
        auto cur = q.front();
        q.pop();
        for (int i = 0; i < 4; i++) {
            int nr = cur.first + dr[i];
            int nc = cur.second + dc[i];
            if (!isCellWalkable(nr, nc)) continue;
            if (reachableFromExit[nr][nc]) continue;
            reachableFromExit[nr][nc] = true;
            q.push({nr, nc});
        }
    }
}

void placeRandomSpawn() {
    int candidates[ROWS * COLS][2];
    int count = 0;
    for (int r = 1; r < ROWS - 1; r++)
        for (int c = 1; c < COLS - 1; c++)
            if (reachableFromExit[r][c] && !(r == EXIT_ROW && c == EXIT_COL)) {
                candidates[count][0] = r;
                candidates[count][1] = c;
                count++;
            }

    int r = 1, c = 1;
    if (count > 0) {
        int pick = randRange(0, count - 1);
        r = candidates[pick][0];
        c = candidates[pick][1];
    }
    spawnRow = r;
    spawnCol = c;
    cam.x = (spawnCol + 0.5f) * CELL;
    cam.z = (spawnRow + 0.5f) * CELL;
    cam.angle = 0.0f;
}

bool isCellUsedForCode(int r, int c, bool used[ROWS][COLS]) {
    if (!isCellWalkable(r, c)) return true;
    if (r == EXIT_ROW && c == EXIT_COL) return true;
    if (r == spawnRow && c == spawnCol) return true;
    if (used[r][c]) return true;
    return false;
}

void placeCodes() {
    bool used[ROWS][COLS] = {};
    used[spawnRow][spawnCol] = true;
    used[EXIT_ROW][EXIT_COL] = true;

    for (int i = 0; i < CODE_COUNT; i++) {
        codeDigits[i] = randRange(0, 9);
        codeCollected[i] = false;
    }

    for (int i = 0; i < CODE_COUNT; i++) {
        int candidates[ROWS * COLS][2];
        int count = 0;
        for (int r = 1; r < ROWS - 1; r++)
            for (int c = 1; c < COLS - 1; c++)
                if (reachableFromExit[r][c] && !isCellUsedForCode(r, c, used)) {
                    candidates[count][0] = r;
                    candidates[count][1] = c;
                    count++;
                }

        int r = 1, c = 1;
        if (count > 0) {
            int pick = randRange(0, count - 1);
            r = candidates[pick][0];
            c = candidates[pick][1];
        }

        used[r][c] = true;
        codeSpots[i].row = r;
        codeSpots[i].col = c;
        codeSpots[i].digit = codeDigits[i];
        codeSpots[i].color = (CodeColor)i;
        codeSpots[i].collected = false;
    }
}

void resetGame() {
    if (maze[EXIT_ROW][EXIT_COL] == 1)
        maze[EXIT_ROW][EXIT_COL] = 0;

    computeReachableFromExit();
    placeRandomSpawn();
    placeCodes();
    memorizeTimeLeft = MEMORIZE_DURATION;
    gameTimeLeft = GAME_DURATION;
    flashlightUses = FLASHLIGHT_USES;
    flashlightActive = false;
    flashlightTimeLeft = 0.0f;
    mapRevealUses = MAP_REVEAL_USES;
    mapRevealActive = false;
    mapRevealTimeLeft = 0.0f;
    resetInputCodes();
    viewMode = 1;
    gameState = STATE_MEMORIZE;
    setStatus("Hafalkan peta dan lokasi kode", 2.5f);
}

int codeIndexAtPlayer() {
    int pr = playerRow();
    int pc = playerCol();
    for (int i = 0; i < CODE_COUNT; i++) {
        if (!codeSpots[i].collected && codeSpots[i].row == pr && codeSpots[i].col == pc)
            return i;
    }
    return -1;
}

// ============================================================
//  RENDER — SATU DINDING
// ============================================================
void drawWall(int col, int row) {
    float x0 = col * CELL,  x1 = x0 + CELL;
    float z0 = row * CELL,  z1 = z0 + CELL;
    float y0 = 0.0f,         y1 = W_HEIGHT;

    glBegin(GL_QUADS);
        glNormal3f(0, 0, 1);
        glVertex3f(x0, y0, z1);
        glVertex3f(x1, y0, z1);
        glVertex3f(x1, y1, z1);
        glVertex3f(x0, y1, z1);

        glNormal3f(0, 0, -1);
        glVertex3f(x1, y0, z0);
        glVertex3f(x0, y0, z0);
        glVertex3f(x0, y1, z0);
        glVertex3f(x1, y1, z0);

        glNormal3f(-1, 0, 0);
        glVertex3f(x0, y0, z0);
        glVertex3f(x0, y0, z1);
        glVertex3f(x0, y1, z1);
        glVertex3f(x0, y1, z0);

        glNormal3f(1, 0, 0);
        glVertex3f(x1, y0, z1);
        glVertex3f(x1, y0, z0);
        glVertex3f(x1, y1, z0);
        glVertex3f(x1, y1, z1);

        glNormal3f(0, 1, 0);
        glVertex3f(x0, y1, z0);
        glVertex3f(x0, y1, z1);
        glVertex3f(x1, y1, z1);
        glVertex3f(x1, y1, z0);
    glEnd();
}

// ============================================================
//  RENDER — LANTAI
// ============================================================
void drawFloor() {
    float tw = COLS * CELL;
    float td = ROWS * CELL;

    glColor3f(0.08f, 0.07f, 0.06f);

    glBegin(GL_QUADS);
        glNormal3f(0, 1, 0);
        glVertex3f(0,  0, 0);
        glVertex3f(tw, 0, 0);
        glVertex3f(tw, 0, td);
        glVertex3f(0,  0, td);
    glEnd();
}

// ============================================================
//  RENDER — LANGIT-LANGIT
// ============================================================
void drawCeiling() {
    float tw = COLS * CELL;
    float td = ROWS * CELL;

    glColor3f(0.03f, 0.03f, 0.03f);

    glBegin(GL_QUADS);
        glNormal3f(0, -1, 0);
        glVertex3f(0,  W_HEIGHT, 0);
        glVertex3f(0,  W_HEIGHT, td);
        glVertex3f(tw, W_HEIGHT, td);
        glVertex3f(tw, W_HEIGHT, 0);
    glEnd();
}

// ============================================================
//  RENDER — MARKER EXIT (kotak hijau di lantai)
// ============================================================
void drawExit() {
    float x0 = EXIT_COL * CELL + 0.2f;
    float x1 = x0 + CELL - 0.4f;
    float z0 = EXIT_ROW * CELL + 0.2f;
    float z1 = z0 + CELL - 0.4f;
    float y  = 0.02f;

    GLfloat emis[] = {0.0f, 0.8f, 0.3f, 1.0f};
    glMaterialfv(GL_FRONT, GL_EMISSION, emis);
    glColor3f(0.0f, 1.0f, 0.4f);

    glBegin(GL_QUADS);
        glNormal3f(0, 1, 0);
        glVertex3f(x0, y, z0);
        glVertex3f(x1, y, z0);
        glVertex3f(x1, y, z1);
        glVertex3f(x0, y, z1);
    glEnd();

    GLfloat zero[] = {0,0,0,1};
    glMaterialfv(GL_FRONT, GL_EMISSION, zero);
}

// ============================================================
//  RENDER — MARKER SPAWN (kotak biru di lantai)
// ============================================================
void drawSpawn() {
    float x0 = spawnCol * CELL + 0.2f;
    float x1 = x0 + CELL - 0.4f;
    float z0 = spawnRow * CELL + 0.2f;
    float z1 = z0 + CELL - 0.4f;
    float y  = 0.02f;

    GLfloat emis[] = {0.2f, 0.4f, 1.0f, 1.0f};
    glMaterialfv(GL_FRONT, GL_EMISSION, emis);
    glColor3f(0.3f, 0.6f, 1.0f);

    glBegin(GL_QUADS);
        glNormal3f(0, 1, 0);
        glVertex3f(x0, y, z0);
        glVertex3f(x1, y, z0);
        glVertex3f(x1, y, z1);
        glVertex3f(x0, y, z1);
    glEnd();

    GLfloat zero[] = {0,0,0,1};
    glMaterialfv(GL_FRONT, GL_EMISSION, zero);
}

// ============================================================
//  RENDER — MARKER KODE
// ============================================================
void drawCodeSpots() {
    for (int i = 0; i < CODE_COUNT; i++) {
        if (codeSpots[i].collected) continue;
        float x0 = codeSpots[i].col * CELL + 0.25f;
        float x1 = x0 + CELL - 0.5f;
        float z0 = codeSpots[i].row * CELL + 0.25f;
        float z1 = z0 + CELL - 0.5f;
        float y  = 0.03f;

        float r = COLOR_RGB[i][0];
        float g = COLOR_RGB[i][1];
        float b = COLOR_RGB[i][2];

        GLfloat emis[] = {r * 0.6f, g * 0.6f, b * 0.6f, 1.0f};
        glMaterialfv(GL_FRONT, GL_EMISSION, emis);
        glColor3f(r, g, b);

        glBegin(GL_QUADS);
            glNormal3f(0, 1, 0);
            glVertex3f(x0, y, z0);
            glVertex3f(x1, y, z0);
            glVertex3f(x1, y, z1);
            glVertex3f(x0, y, z1);
        glEnd();

        GLfloat zero[] = {0,0,0,1};
        glMaterialfv(GL_FRONT, GL_EMISSION, zero);
    }
}

// ============================================================
//  RENDER — SELURUH MAZE
// ============================================================
void drawMaze() {
    glColor3f(0.14f, 0.12f, 0.10f);

    for (int r = 0; r < ROWS; r++)
        for (int c = 0; c < COLS; c++)
            if (maze[r][c] == 1)
                drawWall(c, r);

    drawFloor();
    drawCeiling();
    drawExit();
    drawSpawn();
    if (viewMode == 1)
        drawCodeSpots();
}

// ============================================================
//  SETUP PENCAHAYAAN
// ============================================================
void setupLighting() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glShadeModel(GL_SMOOTH);

    GLfloat ambGlobal[] = {0.008f, 0.007f, 0.006f, 1.0f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambGlobal);

    GLfloat lightPos[]  = {cam.x, cam.y, cam.z, 1.0f};
    GLfloat lightDiff[] = {flashlightActive ? 0.95f : 0.45f,
                           flashlightActive ? 0.75f : 0.38f,
                           flashlightActive ? 0.50f : 0.25f,
                           1.0f};
    GLfloat lightSpec[] = {0.45f, 0.4f, 0.3f, 1.0f};

    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  lightDiff);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpec);

    float range = flashlightActive ? 3.8f : 1.8f;
    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION,  1.0f);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION,    1.8f / range);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.6f / (range * range));

    glEnable(GL_FOG);
    glFogi(GL_FOG_MODE, GL_EXP2);
    glFogf(GL_FOG_DENSITY, flashlightActive ? 0.12f : 0.22f);
    GLfloat fogCol[] = {0.02f, 0.02f, 0.02f, 1.0f};
    glFogfv(GL_FOG_COLOR, fogCol);
}

// ============================================================
//  RENDER — HUD
// ============================================================
void drawText(int x, int y, const char* text, void* font) {
    glRasterPos2i(x, y);
    for (const char* c = text; *c; c++)
        glutBitmapCharacter(font, *c);
}

void drawHUD() {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix(); glLoadIdentity();
    gluOrtho2D(0, WIN_W, 0, WIN_H);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix(); glLoadIdentity();
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_FOG);

    // Status game
    glColor3f(1.0f, 0.85f, 0.4f);
    const char* stateStr = "";
    if (gameState == STATE_MEMORIZE) stateStr = "[ MEMORIZE MAP ]";
    else if (gameState == STATE_PLAY) stateStr = "[ PLAY ]";
    else if (gameState == STATE_ENTER_CODE) stateStr = "[ ENTER CODE ]";
    else if (gameState == STATE_WIN) stateStr = "[ YOU WIN ]";
    else if (gameState == STATE_LOSE) stateStr = "[ TIME OUT ]";
    drawText(WIN_W / 2 - 80, WIN_H - 30, stateStr, GLUT_BITMAP_HELVETICA_18);

    // Timer
    char buf[128];
    if (gameState == STATE_MEMORIZE) {
        sprintf(buf, "Waktu hafalan: %.0f", memorizeTimeLeft);
    } else {
        sprintf(buf, "Sisa waktu: %.0f", gameTimeLeft);
    }
    glColor3f(0.85f, 0.85f, 0.85f);
    drawText(20, WIN_H - 30, buf, GLUT_BITMAP_HELVETICA_12);

    // Flashlight
    sprintf(buf, "Flashlight: %d", flashlightUses);
    glColor3f(0.8f, 0.8f, 0.6f);
    drawText(20, WIN_H - 46, buf, GLUT_BITMAP_HELVETICA_12);
    if (flashlightActive) {
        sprintf(buf, "Aktif: %.1fs", flashlightTimeLeft);
        drawText(20, WIN_H - 62, buf, GLUT_BITMAP_HELVETICA_12);
    }

    // Map reveal
    sprintf(buf, "Map reveal: %d", mapRevealUses);
    glColor3f(0.7f, 0.8f, 1.0f);
    drawText(20, WIN_H - 78, buf, GLUT_BITMAP_HELVETICA_12);
    if (mapRevealActive) {
        sprintf(buf, "Map aktif: %.1fs", mapRevealTimeLeft);
        drawText(20, WIN_H - 94, buf, GLUT_BITMAP_HELVETICA_12);
    }

    // Status kode
    int y = WIN_H - 118;
    for (int i = 0; i < CODE_COUNT; i++) {
        glColor3f(COLOR_RGB[i][0], COLOR_RGB[i][1], COLOR_RGB[i][2]);
        sprintf(buf, "%s: %s", COLOR_NAMES[i], codeCollected[i] ? "OK" : "--");
        drawText(20, y, buf, GLUT_BITMAP_HELVETICA_12);
        y -= 16;
    }

    // Pesan status
    if (statusMsgTimer > 0.0f) {
        glColor3f(1.0f, 0.8f, 0.3f);
        drawText(WIN_W / 2 - 170, 40, statusMsg, GLUT_BITMAP_HELVETICA_12);
    }

    // Prompt interaksi
    if (gameState == STATE_PLAY) {
        int codeIdx = codeIndexAtPlayer();
        if (codeIdx != -1) {
            glColor3f(1.0f, 1.0f, 1.0f);
            sprintf(buf, "Tekan E untuk ambil kode %s", COLOR_NAMES[codeIdx]);
            drawText(WIN_W / 2 - 160, 64, buf, GLUT_BITMAP_HELVETICA_12);
        }
        if (isPlayerAt(EXIT_ROW, EXIT_COL)) {
            glColor3f(0.7f, 1.0f, 0.7f);
            if (allCodesCollected()) {
                drawText(WIN_W / 2 - 150, 80, "Tekan ENTER untuk input kode", GLUT_BITMAP_HELVETICA_12);
            } else {
                drawText(WIN_W / 2 - 150, 80, "Exit terkunci: cari 3 kode", GLUT_BITMAP_HELVETICA_12);
            }
        }
    }

    // Input kode
    if (gameState == STATE_ENTER_CODE) {
        glColor3f(1.0f, 1.0f, 1.0f);
        drawText(WIN_W / 2 - 80, WIN_H / 2 + 60, "INPUT KODE EXIT", GLUT_BITMAP_HELVETICA_18);
        drawText(WIN_W / 2 - 200, WIN_H / 2 + 36, "Ketik angka 0-9, Backspace hapus, Enter kirim", GLUT_BITMAP_HELVETICA_12);

        int startY = WIN_H / 2;
        for (int i = 0; i < CODE_COUNT; i++) {
            CodeColor col = INPUT_ORDER[i];
            glColor3f(COLOR_RGB[col][0], COLOR_RGB[col][1], COLOR_RGB[col][2]);
            char line[64];
            if (enteredCodes[i] >= 0)
                sprintf(line, "%s: %d", COLOR_NAMES[col], enteredCodes[i]);
            else
                sprintf(line, "%s: _", COLOR_NAMES[col]);

            if (i == inputIndex)
                strcat(line, "  <");
            drawText(WIN_W / 2 - 60, startY, line, GLUT_BITMAP_HELVETICA_18);
            startY -= 28;
        }
    }

    // Crosshair (hanya di first-person & play)
    if (viewMode == 0 && gameState == STATE_PLAY) {
        glColor3f(1.0f, 1.0f, 1.0f);
        glLineWidth(1.5f);
        glBegin(GL_LINES);
            glVertex2i(WIN_W/2 - 9, WIN_H/2);
            glVertex2i(WIN_W/2 + 9, WIN_H/2);
            glVertex2i(WIN_W/2, WIN_H/2 - 9);
            glVertex2i(WIN_W/2, WIN_H/2 + 9);
        glEnd();
        glLineWidth(1.0f);
    }

    glEnable(GL_FOG);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

// ============================================================
//  GLUT — DISPLAY
// ============================================================
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Atur view berdasarkan state
    viewMode = (gameState == STATE_MEMORIZE || mapRevealActive) ? 1 : 0;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if (viewMode == 0) {
        gluPerspective(75.0, (double)WIN_W / WIN_H, 0.1, 80.0);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        float rad = toRad(cam.angle);
        float lx  = cam.x + sinf(rad);
        float lz  = cam.z + cosf(rad);
        gluLookAt(cam.x, cam.y, cam.z,
                  lx,    cam.y, lz,
                  0, 1, 0);

        setupLighting();
    } else {
        float mazeW = COLS * CELL;
        float mazeD = ROWS * CELL;
        float pad   = CELL;
        float halfW = mazeW * 0.5f + pad;
        float halfD = mazeD * 0.5f + pad;
        glOrtho(-halfW, halfW, -halfD, halfD, 0.1, 50.0);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        gluLookAt(mazeW/2, 5.0f, mazeD/2,
                  mazeW/2, 0.0f, mazeD/2,
                  0, 0, -1);

        glDisable(GL_FOG);
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        GLfloat topPos[]  = {mazeW/2, 8.0f, mazeD/2, 1.0f};
        GLfloat topDiff[] = {0.9f, 0.85f, 0.75f, 1.0f};
        GLfloat topAmb[]  = {0.4f, 0.38f, 0.33f, 1.0f};
        glLightfv(GL_LIGHT0, GL_POSITION, topPos);
        glLightfv(GL_LIGHT0, GL_DIFFUSE,  topDiff);
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, topAmb);
        glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION,  1.0f);
        glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION,    0.0f);
        glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.0f);
    }

    drawMaze();

    // Render posisi player di top-down
    if (viewMode == 1) {
        glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH_TEST);

        float py = W_HEIGHT + 0.06f;
        glColor3f(1.0f, 1.0f, 0.0f);
        glBegin(GL_TRIANGLE_FAN);
            glVertex3f(cam.x, py, cam.z);
            for (int i = 0; i <= 24; i++) {
                float a = toRad(i * (360.0f / 24.0f));
                glVertex3f(cam.x + cosf(a) * 0.22f, py, cam.z + sinf(a) * 0.22f);
            }
        glEnd();

        float rad = toRad(cam.angle);
        glColor3f(1.0f, 0.5f, 0.0f);
        glLineWidth(2.5f);
        glBegin(GL_LINES);
            glVertex3f(cam.x, py, cam.z);
            glVertex3f(cam.x + sinf(rad)*0.9f, py, cam.z + cosf(rad)*0.9f);
        glEnd();
        glLineWidth(1.0f);

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LIGHTING);
    }

    drawHUD();

    glutSwapBuffers();
}

// ============================================================
//  GLUT — UPDATE (Timer ~60fps)
// ============================================================
void update(int v) {
    static float lastT = 0;
    float t  = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
    float dt = t - lastT;
    lastT    = t;
    if (dt > 0.05f) dt = 0.05f;

    if (gameState == STATE_MEMORIZE) {
        memorizeTimeLeft -= dt;
        if (memorizeTimeLeft <= 0.0f) {
            memorizeTimeLeft = 0.0f;
            gameState = STATE_PLAY;
            viewMode = 0;
            setStatus("Peta ditutup. Mulai permainan.", 2.0f);
        }
    }

    if (mapRevealActive) {
        mapRevealTimeLeft -= dt;
        if (mapRevealTimeLeft <= 0.0f) {
            mapRevealTimeLeft = 0.0f;
            mapRevealActive = false;
        }
    }

    if (gameState == STATE_PLAY || gameState == STATE_ENTER_CODE) {
        gameTimeLeft -= dt;
        if (gameTimeLeft <= 0.0f) {
            gameTimeLeft = 0.0f;
            gameState = STATE_LOSE;
            setStatus("Waktu habis.", 3.0f);
        }
    }

    if (flashlightActive) {
        flashlightTimeLeft -= dt;
        if (flashlightTimeLeft <= 0.0f) {
            flashlightTimeLeft = 0.0f;
            flashlightActive = false;
        }
    }

    if (statusMsgTimer > 0.0f) {
        statusMsgTimer -= dt;
        if (statusMsgTimer < 0.0f) statusMsgTimer = 0.0f;
    }

    if (gameState == STATE_PLAY) {
        float rad = toRad(cam.angle);
        float dx  = 0, dz = 0;
        float spd = cam.speed * dt;
        float trn = cam.tSpeed * dt;

        if (keys['w'] || keys['W']) { dx += sinf(rad)*spd; dz += cosf(rad)*spd; }
        if (keys['s'] || keys['S']) { dx -= sinf(rad)*spd; dz -= cosf(rad)*spd; }
        if (keys['a'] || keys['A']) cam.angle += trn;
        if (keys['d'] || keys['D']) cam.angle -= trn;

        float m  = 0.4f;
        float nx = cam.x + dx;
        float nz = cam.z + dz;

        if (isWalkable(nx + m, cam.z) && isWalkable(nx - m, cam.z))
            cam.x = nx;
        if (isWalkable(cam.x, nz + m) && isWalkable(cam.x, nz - m))
            cam.z = nz;
    }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

// ============================================================
//  GLUT — KEYBOARD
// ============================================================
void keyDown(unsigned char k, int x, int y) {
    keys[k] = true;

    if (k == 27) exit(0);  // ESC

    if (gameState == STATE_PLAY) {
        if (k == 'f' || k == 'F') {
            if (!flashlightActive && flashlightUses > 0) {
                flashlightUses--;
                flashlightActive = true;
                flashlightTimeLeft = FLASHLIGHT_DURATION;
                setStatus("Flashlight aktif", 1.5f);
            }
        }

        if (k == 'm' || k == 'M') {
            if (!mapRevealActive && mapRevealUses > 0) {
                mapRevealUses--;
                mapRevealActive = true;
                mapRevealTimeLeft = MAP_REVEAL_DURATION;
                setStatus("Peta terbuka", 1.5f);
            }
        }

        if (k == 'e' || k == 'E') {
            int idx = codeIndexAtPlayer();
            if (idx != -1) {
                codeSpots[idx].collected = true;
                codeCollected[codeSpots[idx].color] = true;
                char msg[64];
                sprintf(msg, "Kode %s ditemukan: %d", COLOR_NAMES[codeSpots[idx].color], codeSpots[idx].digit);
                setStatus(msg, 3.0f);
            }
        }

        if (k == 13) { // ENTER
            if (isPlayerAt(EXIT_ROW, EXIT_COL) && allCodesCollected()) {
                gameState = STATE_ENTER_CODE;
                resetInputCodes();
                setStatus("Masukkan kode", 2.0f);
            }
        }
    } else if (gameState == STATE_ENTER_CODE) {
        if (k >= '0' && k <= '9') {
            if (inputIndex < CODE_COUNT) {
                enteredCodes[inputIndex] = k - '0';
                inputIndex++;
            }
        } else if (k == 8) { // Backspace
            if (inputIndex > 0) {
                inputIndex--;
                enteredCodes[inputIndex] = -1;
            }
        } else if (k == 13) { // Enter
            if (inputIndex >= CODE_COUNT) {
                bool ok = true;
                for (int i = 0; i < CODE_COUNT; i++) {
                    CodeColor col = INPUT_ORDER[i];
                    if (enteredCodes[i] != codeDigits[col]) {
                        ok = false;
                        break;
                    }
                }
                if (ok) {
                    gameState = STATE_WIN;
                    setStatus("Kode benar. Kamu berhasil keluar.", 4.0f);
                } else {
                    resetInputCodes();
                    setStatus("Kode salah. Coba lagi.", 2.0f);
                }
            }
        }
    }
}

void keyUp(unsigned char k, int x, int y) { keys[k] = false; }

// ============================================================
//  GLUT — RESHAPE
// ============================================================
void reshape(int w, int h) {
    glViewport(0, 0, w, h);
}

// ============================================================
//  INISIALISASI OPENGL
// ============================================================
void initGL() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glClearColor(0.02f, 0.02f, 0.03f, 1.0f);

    GLfloat matSpec[] = {0.2f, 0.18f, 0.12f, 1.0f};
    glMaterialfv(GL_FRONT, GL_SPECULAR,  matSpec);
    glMaterialf(GL_FRONT,  GL_SHININESS, 16.0f);

    printf("===========================================\n");
    printf("  MEMORY MAZE — Game Logic                \n");
    printf("===========================================\n");
    printf("  WASD  : Gerak & Putar                     \n");
    printf("  F     : Flashlight (3x, 5 detik)          \n");
    printf("  M     : Buka peta (3x, 4 detik)           \n");
    printf("  E     : Ambil kode                        \n");
    printf("  ENTER : Input kode di exit               \n");
    printf("  ESC   : Keluar                           \n");
    printf("===========================================\n");
}

// ============================================================
//  MAIN
// ============================================================
int main(int argc, char** argv) {
    srand((unsigned)time(nullptr));

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(WIN_W, WIN_H);
    glutInitWindowPosition(100, 50);
    glutCreateWindow(WIN_NAME);

    initGL();
    resetGame();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyDown);
    glutKeyboardUpFunc(keyUp);
    glutTimerFunc(16, update, 0);

    glutMainLoop();
    return 0;
}
