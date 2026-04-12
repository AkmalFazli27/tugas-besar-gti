// ============================================================
//  MAZE PARADOX — OpenGL/GLUT 3D Horror Maze Game
//  Struktur Kode Dasar
// ============================================================
//  Kompilasi (Windows + MinGW):
//    g++ maze_paradox.cpp -o maze_paradox -lfreeglut -lopengl32 -lglu32
//  Kompilasi (Linux):
//    g++ maze_paradox.cpp -o maze_paradox -lglut -lGL -lGLU
// ============================================================

#include <GL/glut.h>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cstdio>
#include <vector>
#include <queue>
#include <algorithm>

#ifdef _WIN32
#include <windows.h>  // untuk PlaySound()
#pragma comment(lib, "winmm.lib")
#endif

// ============================================================
//  KONFIGURASI LAYAR & WINDOW
// ============================================================
const int SCREEN_W = 1280;
const int SCREEN_H = 720;
const char* WINDOW_TITLE = "Maze Paradox";

// ============================================================
//  KONFIGURASI MAZE
// ============================================================
const int MAZE_ROWS = 11;
const int MAZE_COLS = 11;
const float CELL_SIZE  = 2.0f;  // ukuran tiap sel dalam unit OpenGL
const float WALL_HEIGHT = 2.5f;

// 1 = dinding, 0 = lorong
// Bisa diganti maze yang lebih kompleks
int mazeGrid[MAZE_ROWS][MAZE_COLS] = {
    {1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,1,0,0,0,1,0,1},
    {1,0,1,0,1,0,1,0,0,0,1},
    {1,0,1,0,0,0,1,1,1,0,1},
    {1,0,1,1,1,0,0,0,1,0,1},
    {1,0,0,0,0,0,1,0,0,0,1},
    {1,1,1,0,1,1,1,0,1,1,1},
    {1,0,0,0,1,0,0,0,1,0,1},
    {1,0,1,1,1,0,1,0,0,0,1},
    {1,0,0,0,0,0,1,0,1,0,1},
    {1,1,1,1,1,1,1,1,1,1,1}
};

// Salinan maze untuk quantum shift
int originalMaze[MAZE_ROWS][MAZE_COLS];

// ============================================================
//  STATE GAME
// ============================================================
enum GameState {
    STATE_PLAYING,
    STATE_JUMPSCARE,
    STATE_GAMEOVER,
    STATE_WIN
};
GameState gameState = STATE_PLAYING;

// ============================================================
//  STRUKTUR PLAYER (First-Person Camera)
// ============================================================
struct Player {
    float x = 1.5f * CELL_SIZE;   // posisi world X
    float z = 1.5f * CELL_SIZE;   // posisi world Z
    float y = 1.2f;                // tinggi mata
    float angle = 0.0f;            // arah hadap (derajat, sumbu Y)
    float pitch = 0.0f;            // arah pandang vertikal (derajat)
    float speed = 0.05f;
    float turnSpeed = 2.0f;
    int health = 3;
} player;

// ============================================================
//  STRUKTUR HANTU
// ============================================================
struct Ghost {
    float x = 9.5f * CELL_SIZE;
    float z = 9.5f * CELL_SIZE;
    float speed = 0.02f;
    float r = 1.0f, g = 0.8f, b = 1.0f; // warna hantu (ungu pucat)
    bool active = true;

    // Pathfinding: path ke player
    std::vector<std::pair<int,int>> path;

    // Animasi mengambang
    float bobTimer = 0.0f;
} ghost;

// ============================================================
//  CRYSTALS (Objektif)
// ============================================================
struct Crystal {
    float x, z;
    float r, g, b;      // warna crystal
    bool collected = false;
    float rotAngle = 0.0f;
};

std::vector<Crystal> crystals = {
    {3.5f*CELL_SIZE, 1.5f*CELL_SIZE, 0.2f, 1.0f, 1.0f, false, 0},  // cyan
    {1.5f*CELL_SIZE, 5.5f*CELL_SIZE, 1.0f, 0.2f, 1.0f, false, 0},  // magenta
    {5.5f*CELL_SIZE, 5.5f*CELL_SIZE, 1.0f, 1.0f, 0.2f, false, 0},  // kuning
    {7.5f*CELL_SIZE, 3.5f*CELL_SIZE, 0.2f, 1.0f, 0.3f, false, 0},  // hijau
    {9.5f*CELL_SIZE, 1.5f*CELL_SIZE, 1.0f, 0.5f, 0.0f, false, 0},  // oranye
};

int crystalsCollected = 0;

// Portal keluar
float portalX = 9.5f * CELL_SIZE;
float portalZ = 1.5f * CELL_SIZE;
bool portalActive = false;

// ============================================================
//  TIMER & EFEK
// ============================================================
float jumpscareTimer   = 0.0f;
float flashTimer       = 0.0f;
float shakeIntensity   = 0.0f;
float shakeX           = 0.0f;
float shakeZ           = 0.0f;
float quantumTimer     = 0.0f;
float panicTimer       = 0.0f;
bool  panicMode        = false;
float ambientFlicker   = 1.0f;
float gameTimer        = 180.0f;  // 3 menit

// Input keyboard
bool keys[256] = {false};

// Mouse look
float mouseSensitivity = 0.15f;
bool  invertMouseY = false;  // false = normal, true = inverted
bool  ignoreMouseWarp = false;

// ============================================================
//  UTILITY
// ============================================================
float degToRad(float deg) { return deg * M_PI / 180.0f; }

// Konversi posisi world → grid index
int worldToGrid(float w) { return (int)(w / CELL_SIZE); }

// Cek apakah posisi world bisa dilewati (bukan dinding)
bool isWalkable(float wx, float wz) {
    int gx = worldToGrid(wx);
    int gz = worldToGrid(wz);
    if (gx < 0 || gx >= MAZE_COLS || gz < 0 || gz >= MAZE_ROWS) return false;
    return mazeGrid[gz][gx] == 0;
}

float distance(float x1, float z1, float x2, float z2) {
    float dx = x1 - x2, dz = z1 - z2;
    return sqrtf(dx*dx + dz*dz);
}

// ============================================================
//  BFS PATHFINDING (Hantu → Player)
// ============================================================
std::vector<std::pair<int,int>> bfsPath(int sr, int sc, int er, int ec) {
    std::vector<std::pair<int,int>> path;
    if (sr < 0 || sr >= MAZE_ROWS || sc < 0 || sc >= MAZE_COLS) return path;

    bool visited[MAZE_ROWS][MAZE_COLS] = {};
    std::pair<int,int> parent[MAZE_ROWS][MAZE_COLS];
    memset(parent, -1, sizeof(parent));

    std::queue<std::pair<int,int>> q;
    q.push({sr, sc});
    visited[sr][sc] = true;

    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};

    while (!q.empty()) {
        auto [r, c] = q.front(); q.pop();
        if (r == er && c == ec) {
            // Rekonstruksi path
            std::pair<int,int> cur = {er, ec};
            while (!(cur.first == sr && cur.second == sc)) {
                path.push_back(cur);
                cur = parent[cur.first][cur.second];
            }
            std::reverse(path.begin(), path.end());
            return path;
        }
        for (int d = 0; d < 4; d++) {
            int nr = r + dr[d], nc = c + dc[d];
            if (nr >= 0 && nr < MAZE_ROWS && nc >= 0 && nc < MAZE_COLS
                && !visited[nr][nc] && mazeGrid[nr][nc] == 0) {
                visited[nr][nc] = true;
                parent[nr][nc] = {r, c};
                q.push({nr, nc});
            }
        }
    }
    return path;
}

// ============================================================
//  QUANTUM SHIFT — Lorong berubah saat tidak terlihat
//  (Simplified: acak beberapa sel lorong non-penting)
// ============================================================
void quantumShift() {
    // Simpan ulang dari original, lalu acak beberapa lorong
    memcpy(mazeGrid, originalMaze, sizeof(mazeGrid));

    // Acak 2-3 sel interior (bukan batas, bukan posisi penting)
    int shifts = 2 + rand() % 2;
    for (int s = 0; s < shifts; s++) {
        int r = 2 + rand() % (MAZE_ROWS - 4);
        int c = 2 + rand() % (MAZE_COLS - 4);
        // Jangan ubah posisi player atau crystal
        int pr = worldToGrid(player.z), pc = worldToGrid(player.x);
        if (abs(r - pr) <= 1 && abs(c - pc) <= 1) continue;
        // Toggle: dinding jadi lorong atau sebaliknya (probabilitas kecil)
        if (rand() % 3 == 0) mazeGrid[r][c] = 1 - originalMaze[r][c];
    }
}

// ============================================================
//  RENDER FUNGSI — DINDING
// ============================================================
void drawWall(float x, float z) {
    float x0 = x * CELL_SIZE, x1 = x0 + CELL_SIZE;
    float z0 = z * CELL_SIZE, z1 = z0 + CELL_SIZE;
    float y0 = 0.0f, y1 = WALL_HEIGHT;

    // Warna dinding: abu gelap dengan sedikit ungu
    glColor3f(0.25f, 0.20f, 0.30f);

    glBegin(GL_QUADS);
    // Depan
    glNormal3f(0, 0, 1);
    glVertex3f(x0, y0, z1); glVertex3f(x1, y0, z1);
    glVertex3f(x1, y1, z1); glVertex3f(x0, y1, z1);
    // Belakang
    glNormal3f(0, 0, -1);
    glVertex3f(x1, y0, z0); glVertex3f(x0, y0, z0);
    glVertex3f(x0, y1, z0); glVertex3f(x1, y1, z0);
    // Kiri
    glNormal3f(-1, 0, 0);
    glVertex3f(x0, y0, z0); glVertex3f(x0, y0, z1);
    glVertex3f(x0, y1, z1); glVertex3f(x0, y1, z0);
    // Kanan
    glNormal3f(1, 0, 0);
    glVertex3f(x1, y0, z1); glVertex3f(x1, y0, z0);
    glVertex3f(x1, y1, z0); glVertex3f(x1, y1, z1);
    // Atas
    glNormal3f(0, 1, 0);
    glVertex3f(x0, y1, z0); glVertex3f(x0, y1, z1);
    glVertex3f(x1, y1, z1); glVertex3f(x1, y1, z0);
    glEnd();
}

// ============================================================
//  RENDER FUNGSI — LANTAI & LANGIT-LANGIT
// ============================================================
void drawFloorAndCeiling() {
    float totalW = MAZE_COLS * CELL_SIZE;
    float totalD = MAZE_ROWS * CELL_SIZE;

    // Lantai
    glColor3f(0.15f, 0.12f, 0.18f);
    glBegin(GL_QUADS);
    glNormal3f(0, 1, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(totalW, 0, 0);
    glVertex3f(totalW, 0, totalD);
    glVertex3f(0, 0, totalD);
    glEnd();

    // Langit-langit
    glColor3f(0.08f, 0.06f, 0.12f);
    glBegin(GL_QUADS);
    glNormal3f(0, -1, 0);
    glVertex3f(0, WALL_HEIGHT, 0);
    glVertex3f(0, WALL_HEIGHT, totalD);
    glVertex3f(totalW, WALL_HEIGHT, totalD);
    glVertex3f(totalW, WALL_HEIGHT, 0);
    glEnd();
}

// ============================================================
//  RENDER FUNGSI — CRYSTAL
// ============================================================
void drawCrystal(Crystal& c) {
    if (c.collected) return;

    glPushMatrix();
    glTranslatef(c.x, 0.8f + 0.1f * sinf(c.rotAngle * 0.05f), c.z);
    glRotatef(c.rotAngle, 0, 1, 0);

    // Crystal bersinar
    GLfloat emissive[] = {c.r * 0.5f, c.g * 0.5f, c.b * 0.5f, 1.0f};
    GLfloat diffuse[]  = {c.r, c.g, c.b, 1.0f};
    glMaterialfv(GL_FRONT, GL_EMISSION, emissive);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,  diffuse);

    // Bentuk octahedron
    glScalef(0.18f, 0.3f, 0.18f);
    glutSolidOctahedron();

    GLfloat zero[] = {0,0,0,1};
    glMaterialfv(GL_FRONT, GL_EMISSION, zero);
    glPopMatrix();

    // Point light dari crystal
    int lightIdx = GL_LIGHT2;
    GLfloat pos[] = {c.x, 0.8f, c.z, 1.0f};
    GLfloat col[] = {c.r * 0.6f, c.g * 0.6f, c.b * 0.6f, 1.0f};
    glLightfv(lightIdx, GL_POSITION, pos);
    glLightfv(lightIdx, GL_DIFFUSE,  col);
    glEnable(lightIdx);
}

// ============================================================
//  RENDER FUNGSI — PORTAL EXIT
// ============================================================
void drawPortal() {
    if (!portalActive) return;

    glPushMatrix();
    glTranslatef(portalX, 1.2f, portalZ);

    // Warna portal: hijau electrik
    GLfloat emissive[] = {0.0f, 1.0f, 0.5f, 1.0f};
    glMaterialfv(GL_FRONT, GL_EMISSION, emissive);
    glColor3f(0.0f, 1.0f, 0.5f);

    // Torus sebagai portal ring
    glutSolidTorus(0.1f, 0.6f, 12, 24);

    GLfloat zero[] = {0,0,0,1};
    glMaterialfv(GL_FRONT, GL_EMISSION, zero);
    glPopMatrix();
}

// ============================================================
//  RENDER FUNGSI — HANTU
// ============================================================
void drawGhost() {
    if (!ghost.active) return;

    glPushMatrix();
    float bobY = 0.2f * sinf(ghost.bobTimer);
    glTranslatef(ghost.x, 1.2f + bobY, ghost.z);

    // Transparan
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    GLfloat ghostDiff[] = {ghost.r, ghost.g, ghost.b, 0.65f};
    GLfloat ghostEmis[] = {ghost.r*0.3f, ghost.g*0.3f, ghost.b*0.4f, 1.0f};
    glMaterialfv(GL_FRONT, GL_DIFFUSE,  ghostDiff);
    glMaterialfv(GL_FRONT, GL_EMISSION, ghostEmis);

    // Kepala: sphere
    glutSolidSphere(0.28f, 16, 16);

    // Body: cone terbalik (rok hantu)
    glTranslatef(0.0f, -0.45f, 0.0f);
    glRotatef(180.0f, 1, 0, 0);
    glutSolidCone(0.32f, 0.55f, 12, 4);

    // Mata (dua sphere kecil hitam)
    GLfloat eyeColor[] = {0.0f, 0.0f, 0.0f, 1.0f};
    glMaterialfv(GL_FRONT, GL_DIFFUSE, eyeColor);
    glMaterialfv(GL_FRONT, GL_EMISSION, eyeColor);
    glTranslatef(0, 0.45f, 0);   // kembali ke kepala
    glPushMatrix();
        glTranslatef(-0.1f, 0.05f, 0.25f);
        glutSolidSphere(0.05f, 8, 8);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(0.1f, 0.05f, 0.25f);
        glutSolidSphere(0.05f, 8, 8);
    glPopMatrix();

    GLfloat zero[] = {0,0,0,1};
    glMaterialfv(GL_FRONT, GL_EMISSION, zero);
    glDisable(GL_BLEND);
    glPopMatrix();
}

// ============================================================
//  RENDER FUNGSI — HUD (HP, Crystal Count, Timer)
// ============================================================
void drawHUD() {
    // Switch ke mode 2D
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, SCREEN_W, 0, SCREEN_H);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    // Teks Crystal
    char buf[64];
    sprintf(buf, "Crystals: %d / %d", crystalsCollected, (int)crystals.size());
    glColor3f(0.4f, 1.0f, 1.0f);
    glRasterPos2i(20, SCREEN_H - 30);
    for (char* c = buf; *c; c++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);

    // Teks HP
    sprintf(buf, "HP: %d", player.health);
    glColor3f(1.0f, 0.3f, 0.3f);
    glRasterPos2i(20, SCREEN_H - 55);
    for (char* c = buf; *c; c++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);

    // Timer
    int mins = (int)gameTimer / 60, secs = (int)gameTimer % 60;
    sprintf(buf, "Time: %02d:%02d", mins, secs);
    glColor3f(panicMode ? 1.0f : 0.8f, panicMode ? 0.2f : 0.8f, 0.2f);
    glRasterPos2i(SCREEN_W - 130, SCREEN_H - 30);
    for (char* c = buf; *c; c++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);

    // Crosshair
    glColor3f(1,1,1);
    glBegin(GL_LINES);
        glVertex2i(SCREEN_W/2 - 10, SCREEN_H/2);
        glVertex2i(SCREEN_W/2 + 10, SCREEN_H/2);
        glVertex2i(SCREEN_W/2, SCREEN_H/2 - 10);
        glVertex2i(SCREEN_W/2, SCREEN_H/2 + 10);
    glEnd();

    // Pesan portal
    if (portalActive) {
        glColor3f(0.0f, 1.0f, 0.5f);
        glRasterPos2i(SCREEN_W/2 - 100, 40);
        const char* msg = "PORTAL AKTIF! Temukan jalan keluar!";
        for (const char* c = msg; *c; c++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

// ============================================================
//  RENDER FUNGSI — JUMPSCARE OVERLAY
// ============================================================
void drawJumpscare() {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, SCREEN_W, 0, SCREEN_H);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Flash merah / putih berulang
    float t = jumpscareTimer;
    float alpha = fminf(1.0f, t / 0.3f);
    if ((int)(t * 10) % 2 == 0)
        glColor4f(1.0f, 0.0f, 0.0f, alpha);
    else
        glColor4f(1.0f, 1.0f, 1.0f, alpha * 0.5f);

    glBegin(GL_QUADS);
        glVertex2i(0, 0); glVertex2i(SCREEN_W, 0);
        glVertex2i(SCREEN_W, SCREEN_H); glVertex2i(0, SCREEN_H);
    glEnd();

    // Teks JUMPSCARE
    if (t > 0.3f) {
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        glRasterPos2i(SCREEN_W/2 - 80, SCREEN_H/2);
        const char* msg = "!!! AWAS !!!";
        for (const char* c = msg; *c; c++)
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
    }

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

// ============================================================
//  RENDER FUNGSI — GAME OVER / WIN SCREEN
// ============================================================
void drawEndScreen(bool win) {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, SCREEN_W, 0, SCREEN_H);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    glColor4f(0, 0, 0, 0.8f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBegin(GL_QUADS);
        glVertex2i(0,0); glVertex2i(SCREEN_W,0);
        glVertex2i(SCREEN_W,SCREEN_H); glVertex2i(0,SCREEN_H);
    glEnd();
    glDisable(GL_BLEND);

    if (win) {
        glColor3f(0.0f, 1.0f, 0.5f);
        glRasterPos2i(SCREEN_W/2 - 60, SCREEN_H/2 + 20);
        const char* msg = "KAMU KABUR! YOU WIN!";
        for (const char* c = msg; *c; c++)
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
    } else {
        glColor3f(1.0f, 0.2f, 0.2f);
        glRasterPos2i(SCREEN_W/2 - 80, SCREEN_H/2 + 20);
        const char* msg = "GAME OVER — Kamu ditangkap!";
        for (const char* c = msg; *c; c++)
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
    }

    glColor3f(0.8f, 0.8f, 0.8f);
    glRasterPos2i(SCREEN_W/2 - 80, SCREEN_H/2 - 20);
    const char* sub = "Tekan R untuk restart, ESC untuk keluar";
    for (const char* c = sub; *c; c++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

// ============================================================
//  SETUP PENCAHAYAAN
// ============================================================
void setupLighting() {
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glShadeModel(GL_SMOOTH);  // Gouraud/Phong shading

    // Ambient global sangat gelap (suasana horror)
    GLfloat ambientGlobal[] = {0.04f * ambientFlicker,
                                0.03f * ambientFlicker,
                                0.06f * ambientFlicker, 1.0f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientGlobal);

    // LIGHT0: Lantern player (mengikuti kamera)
    glEnable(GL_LIGHT0);
    GLfloat lightPos[]  = {player.x + shakeX, player.y, player.z + shakeZ, 1.0f};
    GLfloat lightDiff[] = {0.9f, 0.75f, 0.5f, 1.0f};   // warna kuning lantern
    GLfloat lightSpec[] = {0.5f, 0.4f, 0.2f, 1.0f};
    GLfloat attenConst  = 1.0f, attenLin = 0.3f, attenQuad = 0.05f;
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  lightDiff);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpec);
    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION,  attenConst);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION,    attenLin);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, attenQuad);

    // LIGHT1: Ambient hantu (ungu redup, dari posisi hantu)
    glEnable(GL_LIGHT1);
    GLfloat ghostLightPos[]  = {ghost.x, 1.5f, ghost.z, 1.0f};
    GLfloat ghostLightDiff[] = {0.3f, 0.0f, 0.5f, 1.0f};
    glLightfv(GL_LIGHT1, GL_POSITION, ghostLightPos);
    glLightfv(GL_LIGHT1, GL_DIFFUSE,  ghostLightDiff);
    glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION,  1.0f);
    glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION,    0.5f);
    glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.1f);
}

// ============================================================
//  UPDATE — PLAYER MOVEMENT
// ============================================================
void updatePlayer(float dt) {
    float rad = degToRad(player.angle);
    float dx = 0, dz = 0;

    if (keys['w'] || keys['W']) { dx += sinf(rad); dz += cosf(rad); }
    if (keys['s'] || keys['S']) { dx -= sinf(rad); dz -= cosf(rad); }
    // Strafing kiri/kanan (tanpa rotasi keyboard)
    if (keys['a'] || keys['A']) { dx += cosf(rad); dz -= sinf(rad); }
    if (keys['d'] || keys['D']) { dx -= cosf(rad); dz += sinf(rad); }

    float nx = player.x + dx * player.speed * 60 * dt;
    float nz = player.z + dz * player.speed * 60 * dt;

    // Collision detection dengan margin
    float margin = 0.35f;
    if (isWalkable(nx + margin, player.z) && isWalkable(nx - margin, player.z))
        player.x = nx;
    if (isWalkable(player.x, nz + margin) && isWalkable(player.x, nz - margin))
        player.z = nz;
}

// ============================================================
//  UPDATE — HANTU AI
// ============================================================
void updateGhost(float dt) {
    if (!ghost.active) return;
    ghost.bobTimer += dt * 3.0f;

    // Recalculate path setiap ~0.5 detik
    static float pathTimer = 0;
    pathTimer += dt;
    if (pathTimer > 0.5f) {
        pathTimer = 0;
        int gr = worldToGrid(ghost.z), gc = worldToGrid(ghost.x);
        int pr = worldToGrid(player.z), pc = worldToGrid(player.x);
        ghost.path = bfsPath(gr, gc, pr, pc);
    }

    // Gerakkan hantu ke node berikutnya
    if (!ghost.path.empty()) {
        auto [tr, tc] = ghost.path[0];
        float tx = (tc + 0.5f) * CELL_SIZE;
        float tz = (tr + 0.5f) * CELL_SIZE;
        float dx = tx - ghost.x, dz = tz - ghost.z;
        float len = sqrtf(dx*dx + dz*dz);
        if (len > 0.05f) {
            ghost.x += (dx / len) * ghost.speed * 60 * dt;
            ghost.z += (dz / len) * ghost.speed * 60 * dt;
        } else {
            ghost.path.erase(ghost.path.begin());
        }
    }

    // Cek tabrakan dengan player → JUMPSCARE
    float dist = distance(ghost.x, ghost.z, player.x, player.z);
    if (dist < 0.6f && gameState == STATE_PLAYING) {
        gameState = STATE_JUMPSCARE;
        jumpscareTimer = 0.0f;
        player.health--;
#ifdef _WIN32
        // PlaySound("jumpscare.wav", NULL, SND_ASYNC | SND_FILENAME);
#endif
    }
}

// ============================================================
//  UPDATE — CRYSTAL COLLECTION
// ============================================================
void updateCrystals() {
    for (auto& c : crystals) {
        c.rotAngle += 1.5f;
        if (!c.collected && distance(player.x, player.z, c.x, c.z) < 0.7f) {
            c.collected = true;
            crystalsCollected++;
            if (crystalsCollected >= (int)crystals.size()) {
                portalActive = true;
                ghost.speed += 0.01f; // hantu makin cepat!
            }
        }
    }
}

// ============================================================
//  UPDATE — CEK WIN
// ============================================================
void checkWin() {
    if (!portalActive) return;
    if (distance(player.x, player.z, portalX, portalZ) < 1.0f)
        gameState = STATE_WIN;
}

// ============================================================
//  RESET GAME
// ============================================================
void resetGame() {
    player.x = 1.5f * CELL_SIZE;
    player.z = 1.5f * CELL_SIZE;
    player.angle = 0;
    player.pitch = 0;
    player.health = 3;

    ghost.x = 9.5f * CELL_SIZE;
    ghost.z = 9.5f * CELL_SIZE;
    ghost.speed = 0.02f;
    ghost.active = true;
    ghost.path.clear();

    crystalsCollected = 0;
    for (auto& c : crystals) { c.collected = false; c.rotAngle = 0; }
    portalActive = false;

    memcpy(mazeGrid, originalMaze, sizeof(mazeGrid));

    gameTimer = 180.0f;
    panicMode = false;
    gameState = STATE_PLAYING;
}

// ============================================================
//  GLUT CALLBACK — DISPLAY
// ============================================================
void display() {
    // Screen shake offset
    float sx = (gameState == STATE_JUMPSCARE) ? shakeX : 0;
    float sz = (gameState == STATE_JUMPSCARE) ? shakeZ : 0;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // --- Setup kamera perspective ---
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(75.0, (double)SCREEN_W / SCREEN_H, 0.1, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // First-person camera
    float yawRad = degToRad(player.angle);
    float pitchRad = degToRad(player.pitch);
    float dirX = cosf(pitchRad) * sinf(yawRad);
    float dirY = sinf(pitchRad);
    float dirZ = cosf(pitchRad) * cosf(yawRad);
    float lookX = player.x + dirX;
    float lookY = player.y + dirY;
    float lookZ = player.z + dirZ;
    gluLookAt(player.x + sx, player.y, player.z + sz,
              lookX  + sx, lookY, lookZ  + sz,
              0, 1, 0);

    setupLighting();

    // --- Render dunia 3D ---
    drawFloorAndCeiling();

    for (int r = 0; r < MAZE_ROWS; r++)
        for (int c = 0; c < MAZE_COLS; c++)
            if (mazeGrid[r][c] == 1)
                drawWall(c, r);

    for (auto& cr : crystals) drawCrystal(cr);
    drawPortal();
    drawGhost();

    // --- HUD ---
    drawHUD();

    // --- Overlay berdasarkan state ---
    if (gameState == STATE_JUMPSCARE)
        drawJumpscare();
    else if (gameState == STATE_GAMEOVER)
        drawEndScreen(false);
    else if (gameState == STATE_WIN)
        drawEndScreen(true);

    glutSwapBuffers();
}

// ============================================================
//  GLUT CALLBACK — UPDATE (Timer)
// ============================================================
void update(int value) {
    static float lastTime = 0;
    float currentTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
    float dt = currentTime - lastTime;
    lastTime = currentTime;
    dt = fminf(dt, 0.05f);

    if (gameState == STATE_PLAYING) {
        updatePlayer(dt);
        updateGhost(dt);
        updateCrystals();
        checkWin();

        // Timer
        gameTimer -= dt;
        if (gameTimer <= 0) gameState = STATE_GAMEOVER;

        // Panic mode: 60 detik terakhir
        panicMode = (gameTimer < 60.0f);
        if (panicMode) {
            ambientFlicker = 0.5f + 0.5f * sinf(currentTime * 8.0f);
            ghost.speed = fminf(0.06f, ghost.speed + 0.0001f);
        }

        // Quantum shift
        quantumTimer += dt;
        float shiftInterval = panicMode ? 3.0f : 6.0f;
        if (quantumTimer > shiftInterval) {
            quantumTimer = 0;
            quantumShift();
        }

    } else if (gameState == STATE_JUMPSCARE) {
        jumpscareTimer += dt;
        // Screen shake
        shakeX = (rand() % 100 - 50) * 0.003f * fmaxf(0, 1.0f - jumpscareTimer);
        shakeZ = (rand() % 100 - 50) * 0.003f * fmaxf(0, 1.0f - jumpscareTimer);

        if (jumpscareTimer > 1.5f) {
            // Respawn hantu jauh dari player
            ghost.x = 9.5f * CELL_SIZE;
            ghost.z = 9.5f * CELL_SIZE;
            ghost.path.clear();

            if (player.health <= 0)
                gameState = STATE_GAMEOVER;
            else
                gameState = STATE_PLAYING;
        }
    }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);  // ~60 FPS
}

// ============================================================
//  GLUT CALLBACK — KEYBOARD
// ============================================================
void keyDown(unsigned char key, int x, int y) {
    keys[key] = true;
    if (key == 27) exit(0); // ESC
    if ((key == 'r' || key == 'R') &&
        (gameState == STATE_GAMEOVER || gameState == STATE_WIN))
        resetGame();
}
void keyUp(unsigned char key, int x, int y) { keys[key] = false; }

void mouseMotion(int x, int y) {
    if (ignoreMouseWarp) {
        ignoreMouseWarp = false;
        return;
    }

    int centerX = glutGet(GLUT_WINDOW_WIDTH) / 2;
    int centerY = glutGet(GLUT_WINDOW_HEIGHT) / 2;
    int dx = x - centerX;
    int dy = y - centerY;

    if (dx == 0 && dy == 0) return;

    player.angle -= dx * mouseSensitivity;
    float yDelta = -dy * mouseSensitivity * (invertMouseY ? -1.0f : 1.0f);
    player.pitch += yDelta;
    player.pitch = std::clamp(player.pitch, -80.0f, 80.0f);

    ignoreMouseWarp = true;
    glutWarpPointer(centerX, centerY);
}

// ============================================================
//  GLUT CALLBACK — RESHAPE
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
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Material default
    GLfloat matSpec[] = {0.3f, 0.3f, 0.3f, 1.0f};
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
    glMaterialf(GL_FRONT, GL_SHININESS, 32.0f);

    // Simpan maze original untuk quantum shift
    memcpy(originalMaze, mazeGrid, sizeof(mazeGrid));
}

// ============================================================
//  MAIN
// ============================================================
int main(int argc, char** argv) {
    srand((unsigned)time(NULL));

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(SCREEN_W, SCREEN_H);
    glutCreateWindow(WINDOW_TITLE);

    initGL();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyDown);
    glutKeyboardUpFunc(keyUp);
    glutPassiveMotionFunc(mouseMotion);
    glutMotionFunc(mouseMotion);
    glutTimerFunc(16, update, 0);

    glutSetCursor(GLUT_CURSOR_NONE);
    ignoreMouseWarp = true;
    glutWarpPointer(SCREEN_W / 2, SCREEN_H / 2);

    printf("=== MAZE PARADOX ===\n");
    printf("WASD  : Gerak\n");
    printf("Mouse : Arah kamera\n");
    printf("Tujuan: Kumpulkan semua crystal lalu masuk portal!\n");
    printf("Hati-hati dengan hantu...\n");

    glutMainLoop();
    return 0;
}
