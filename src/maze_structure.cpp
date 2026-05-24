// ============================================================
//  MEMORY MAZE — Struktur Maze Dasar
//  Tahap 1: Maze layout, dinding, lantai, kamera first-person
//
//  # Compile
//  C:\msys64\mingw64\bin\g++.exe -std=c++17 -g src\maze_structure.cpp -o memory_maze.exe -lfreeglut -lopengl32 -lglu32
//
//  # Run (tambahkan PATH runtime freeglut)
//  $env:Path = "C:\msys64\mingw64\bin;$env:Path"
//  .\memory_maze.exe
// ============================================================

#include <GL/glut.h>
#include <cmath>
#include <cstdio>

// ============================================================
//  KONFIGURASI WINDOW
// ============================================================
const int   WIN_W    = 1280;
const int   WIN_H    = 720;
const char* WIN_NAME = "Memory Maze — Struktur";

// ============================================================
//  KONFIGURASI MAZE
// ============================================================
const int   ROWS      = 13;
const int   COLS      = 13;
const float CELL      = 2.0f;     // ukuran 1 sel (unit OpenGL)
const float W_HEIGHT  = 3.0f;     // tinggi dinding

//  Legenda:
//  1 = dinding
//  0 = lorong
//  S = spawn player (disimpan terpisah, di grid tetap 0)
//  E = exit (disimpan terpisah, di grid tetap 0)

int maze[ROWS][COLS] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,1,0,0,0,0,0,1},
    {1,0,1,1,1,0,1,0,1,1,1,0,1},
    {1,0,1,0,0,0,0,0,0,0,1,0,1},
    {1,0,1,0,1,1,1,1,1,0,1,0,1},
    {1,0,0,0,1,0,0,0,1,0,0,0,1},
    {1,1,1,0,1,0,1,0,1,0,1,1,1},
    {1,0,0,0,1,0,0,0,1,0,0,0,1},
    {1,0,1,0,1,1,1,1,1,0,1,0,1},
    {1,0,1,0,0,0,0,0,0,0,1,0,1},
    {1,0,1,1,1,0,1,0,1,1,1,0,1},
    {1,0,0,0,0,0,1,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1}
};

// Posisi spawn & exit di grid
const int SPAWN_ROW = 1, SPAWN_COL = 1;
const int EXIT_ROW  = 11, EXIT_COL  = 11;

// Kunci untuk membuka exit
struct KeyItem {
    int row;
    int col;
    GLfloat color[4];
    bool collected;
};

const int KEY_COUNT = 3;
KeyItem keyItems[KEY_COUNT] = {
    {1, 11, {1.0f, 0.82f, 0.20f, 1.0f}, false},
    {5, 7,  {0.25f, 0.85f, 1.0f, 1.0f}, false},
    {9, 9,  {1.0f, 0.35f, 0.85f, 1.0f}, false}
};

bool allKeysCollected() {
    for (int i = 0; i < KEY_COUNT; i++) {
        if (!keyItems[i].collected) return false;
    }
    return true;
}

// ============================================================
//  KAMERA FIRST-PERSON
// ============================================================
struct Camera {
    float x     = (SPAWN_COL + 0.5f) * CELL;  // posisi world X
    float z     = (SPAWN_ROW + 0.5f) * CELL;  // posisi world Z
    float y     = 1.3f;                         // tinggi mata
    float angle = 0.0f;                         // arah hadap (derajat)
    float speed = 3.5f;
    float tSpeed= 80.0f;
} cam;

bool keys[256] = {};

// ============================================================
//  MODE TAMPILAN
//  0 = First-Person (perspective)
//  1 = Top-Down (orthographic) — untuk lihat layout maze
// ============================================================
int viewMode = 0;

// ============================================================
//  UTILITY
// ============================================================
float toRad(float deg) { return deg * 3.14159265f / 180.0f; }

bool isWalkable(float wx, float wz) {
    int r = (int)(wz / CELL);
    int c = (int)(wx / CELL);
    if (r < 0 || r >= ROWS || c < 0 || c >= COLS) return false;
    return maze[r][c] == 0;
}

void drawKeyMarker(const KeyItem& key, bool overlay) {
    float wx = (key.col + 0.5f) * CELL;
    float wz = (key.row + 0.5f) * CELL;
    float y  = overlay ? (W_HEIGHT + 0.08f) : 0.18f;
    float size = key.collected ? 0.14f : 0.22f;
    GLfloat color[] = {
        key.collected ? 0.55f : key.color[0],
        key.collected ? 0.55f : key.color[1],
        key.collected ? 0.55f : key.color[2],
        1.0f
    };
    GLfloat emis[] = {
        key.collected ? 0.05f : key.color[0],
        key.collected ? 0.05f : key.color[1],
        key.collected ? 0.05f : key.color[2],
        1.0f
    };

    glMaterialfv(GL_FRONT, GL_EMISSION, emis);
    glColor3fv(color);

    glPushMatrix();
        glTranslatef(wx, y, wz);

        glBegin(GL_TRIANGLE_FAN);
            glVertex3f(0.0f, 0.0f, 0.0f);
            for (int i = 0; i <= 20; i++) {
                float a = toRad(i * (360.0f / 20.0f));
                glVertex3f(cosf(a) * size, 0.0f, sinf(a) * size);
            }
        glEnd();

        glLineWidth(2.0f);
        glBegin(GL_LINES);
            glVertex3f(size * 0.4f, 0.0f, 0.0f);
            glVertex3f(size * 0.95f, 0.0f, 0.0f);
        glEnd();
        glLineWidth(1.0f);
    glPopMatrix();

    GLfloat zero[] = {0,0,0,1};
    glMaterialfv(GL_FRONT, GL_EMISSION, zero);
}

void drawExitMarker(bool overlay) {
    float x0 = EXIT_COL * CELL + 0.16f;
    float x1 = x0 + CELL - 0.32f;
    float z0 = EXIT_ROW * CELL + 0.16f;
    float z1 = z0 + CELL - 0.32f;
    float y  = overlay ? (W_HEIGHT + 0.05f) : 0.02f;
    bool unlocked = allKeysCollected();

    GLfloat emis[] = {
        unlocked ? 0.0f : 0.55f,
        unlocked ? 0.85f : 0.12f,
        unlocked ? 0.35f : 0.05f,
        1.0f
    };
    glMaterialfv(GL_FRONT, GL_EMISSION, emis);
    glColor3f(unlocked ? 0.0f : 0.7f, unlocked ? 1.0f : 0.2f, unlocked ? 0.45f : 0.1f);

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

void collectKeysIfNear() {
    for (int i = 0; i < KEY_COUNT; i++) {
        if (keyItems[i].collected) continue;

        float keyX = (keyItems[i].col + 0.5f) * CELL;
        float keyZ = (keyItems[i].row + 0.5f) * CELL;
        float dx = cam.x - keyX;
        float dz = cam.z - keyZ;
        float dist2 = dx * dx + dz * dz;

        if (dist2 < 0.45f * 0.45f) {
            keyItems[i].collected = true;
            printf("[KEY] Terkumpul %d/%d\n", i + 1, KEY_COUNT);
        }
    }
}

// ============================================================
//  RENDER — SATU DINDING
//  Setiap dinding = box penuh 5 sisi (atas + 4 sisi samping)
// ============================================================
void drawWall(int col, int row) {
    float x0 = col * CELL,  x1 = x0 + CELL;
    float z0 = row * CELL,  z1 = z0 + CELL;
    float y0 = 0.0f,         y1 = W_HEIGHT;

    glBegin(GL_QUADS);

        // === Sisi DEPAN (z+) ===
        glNormal3f(0, 0, 1);
        glVertex3f(x0, y0, z1);
        glVertex3f(x1, y0, z1);
        glVertex3f(x1, y1, z1);
        glVertex3f(x0, y1, z1);

        // === Sisi BELAKANG (z-) ===
        glNormal3f(0, 0, -1);
        glVertex3f(x1, y0, z0);
        glVertex3f(x0, y0, z0);
        glVertex3f(x0, y1, z0);
        glVertex3f(x1, y1, z0);

        // === Sisi KIRI (x-) ===
        glNormal3f(-1, 0, 0);
        glVertex3f(x0, y0, z0);
        glVertex3f(x0, y0, z1);
        glVertex3f(x0, y1, z1);
        glVertex3f(x0, y1, z0);

        // === Sisi KANAN (x+) ===
        glNormal3f(1, 0, 0);
        glVertex3f(x1, y0, z1);
        glVertex3f(x1, y0, z0);
        glVertex3f(x1, y1, z0);
        glVertex3f(x1, y1, z1);

        // === Sisi ATAS ===
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

    // Warna lantai: abu gelap kecoklatan
    glColor3f(0.20f, 0.16f, 0.13f);

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

    // Langit-langit: sangat gelap
    glColor3f(0.08f, 0.06f, 0.05f);

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
    drawExitMarker(false);
}

// ============================================================
//  RENDER — MARKER SPAWN (kotak biru di lantai)
// ============================================================
void drawSpawn() {
    float x0 = SPAWN_COL * CELL + 0.2f;
    float x1 = x0 + CELL - 0.4f;
    float z0 = SPAWN_ROW * CELL + 0.2f;
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
//  RENDER — SELURUH MAZE
// ============================================================
void drawMaze() {
    // Warna dinding batu
    glColor3f(0.38f, 0.30f, 0.22f);

    for (int r = 0; r < ROWS; r++)
        for (int c = 0; c < COLS; c++)
            if (maze[r][c] == 1)
                drawWall(c, r);

    drawFloor();
    drawCeiling();
    drawExit();
    drawSpawn();

    for (int i = 0; i < KEY_COUNT; i++)
        drawKeyMarker(keyItems[i], false);
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

    // Ambient global — redup seperti ruangan gelap
    GLfloat ambGlobal[] = {0.07f, 0.06f, 0.05f, 1.0f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambGlobal);

    // Cahaya utama mengikuti posisi kamera (seperti obor/lilin)
    GLfloat lightPos[]  = {cam.x, cam.y, cam.z, 1.0f};
    GLfloat lightDiff[] = {1.0f, 0.93f, 0.68f, 1.0f};  // kuning hangat
    GLfloat lightSpec[] = {0.55f, 0.50f, 0.30f, 1.0f};

    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  lightDiff);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpec);

    // Attenuation: cahaya meredup di kejauhan
    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION,  1.0f);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION,    0.26f);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.03f);

    // Fog tipis untuk kedalaman
    glEnable(GL_FOG);
    glFogi(GL_FOG_MODE, GL_EXP2);
    glFogf(GL_FOG_DENSITY, 0.045f);
    GLfloat fogCol[] = {0.07f, 0.06f, 0.05f, 1.0f};
    glFogfv(GL_FOG_COLOR, fogCol);
}

// ============================================================
//  RENDER — HUD
// ============================================================
void drawHUD() {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix(); glLoadIdentity();
    gluOrtho2D(0, WIN_W, 0, WIN_H);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix(); glLoadIdentity();
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_FOG);

    // Judul mode
    const char* modeStr = (viewMode == 0)
        ? "[ First-Person View — Perspective ]"
        : "[ Top-Down View — Orthographic    ]";
    glColor3f(1.0f, 0.85f, 0.4f);
    glRasterPos2i(WIN_W/2 - 145, WIN_H - 30);
    for (const char* c = modeStr; *c; c++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);

    // Kontrol
    glColor3f(0.65f, 0.65f, 0.65f);
    const char* ctrl = "WASD: Gerak & Putar  |  V: Ganti View  |  ESC: Keluar";
    glRasterPos2i(WIN_W/2 - 190, 18);
    for (const char* c = ctrl; *c; c++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);

    // Legenda warna
    // Biru = Spawn
    glColor3f(0.3f, 0.6f, 1.0f);
    glRasterPos2i(20, WIN_H - 30);
    const char* s1 = "[ BIRU ] Spawn";
    for (const char* c = s1; *c; c++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);

    // Hijau = Exit
    glColor3f(0.0f, 1.0f, 0.4f);
    glRasterPos2i(20, WIN_H - 46);
    const char* s2 = "[ HIJAU ] Exit";
    for (const char* c = s2; *c; c++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);

    // Crosshair (hanya di first-person)
    if (viewMode == 0) {
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

    // Posisi grid player (debug)
    char buf[64];
    int pr = (int)(cam.z / CELL), pc = (int)(cam.x / CELL);
    sprintf(buf, "Grid: row=%d col=%d", pr, pc);
    glColor3f(0.5f, 0.5f, 0.5f);
    glRasterPos2i(WIN_W - 170, 18);
    for (char* c = buf; *c; c++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);

    // Status kunci
    char keyBuf[64];
    int collectedCount = 0;
    for (int i = 0; i < KEY_COUNT; i++)
        if (keyItems[i].collected) collectedCount++;
    sprintf(keyBuf, "Keys: %d/%d", collectedCount, KEY_COUNT);
    glColor3f(0.85f, 0.85f, 0.85f);
    glRasterPos2i(WIN_W - 170, 34);
    for (char* c = keyBuf; *c; c++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);

    if (viewMode == 1) {
        glColor3f(0.85f, 0.85f, 0.85f);
        glRasterPos2i(20, 34);
        const char* mapHint = "Map terbuka: kunci dan exit ditandai";
        for (const char* c = mapHint; *c; c++)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
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

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if (viewMode == 0) {
        // ---- FIRST-PERSON (Perspective) ----
        gluPerspective(62.0, (double)WIN_W / WIN_H, 0.1, 55.0);

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
        // ---- TOP-DOWN (Orthographic) ----
        float mazeW = COLS * CELL;
        float mazeD = ROWS * CELL;
        float pad   = CELL;
        // Penting: kamera berada di tengah maze, jadi volume ortho harus
        // dipusatkan di (0,0) pada eye-space (bukan dari 0..mazeW/mazeD).
        // Jika tidak, hanya sebagian maze yang terlihat (ter-clipping).
        float halfW = mazeW * 0.5f + pad;
        float halfD = mazeD * 0.5f + pad;
        glOrtho(-halfW, halfW, -halfD, halfD, 0.1, 50.0);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        // Kamera dari atas, lihat ke bawah
        gluLookAt(mazeW/2, 5.0f, mazeD/2,
                  mazeW/2, 0.0f, mazeD/2,
                  0, 0, -1);

        // Pencahayaan sederhana untuk top-down
        glDisable(GL_FOG);
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        GLfloat topPos[]  = {mazeW/2, 8.0f, mazeD/2, 1.0f};
        GLfloat topDiff[] = {1.0f, 0.95f, 0.85f, 1.0f};
        GLfloat topAmb[]  = {0.45f, 0.40f, 0.34f, 1.0f};
        glLightfv(GL_LIGHT0, GL_POSITION, topPos);
        glLightfv(GL_LIGHT0, GL_DIFFUSE,  topDiff);
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, topAmb);
        glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION,  1.0f);
        glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION,    0.0f);
        glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.0f);
    }

    drawMaze();

    if (viewMode == 1) {
        glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH_TEST);

        drawExitMarker(true);
        for (int i = 0; i < KEY_COUNT; i++)
            drawKeyMarker(keyItems[i], true);

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LIGHTING);
    }

    // Render posisi player sebagai titik kecil di top-down
    if (viewMode == 1) {
        glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH_TEST);

        // Marker player (kuning) — digambar di atas maze agar selalu terlihat
        float py = W_HEIGHT + 0.06f;
        glColor3f(1.0f, 1.0f, 0.0f);
        glBegin(GL_TRIANGLE_FAN);
            glVertex3f(cam.x, py, cam.z);
            for (int i = 0; i <= 24; i++) {
                float a = toRad(i * (360.0f / 24.0f));
                glVertex3f(cam.x + cosf(a) * 0.22f, py, cam.z + sinf(a) * 0.22f);
            }
        glEnd();

        // Garis arah hadap player
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

    if (viewMode == 0) {
        float rad = toRad(cam.angle);
        float dx  = 0, dz = 0;
        float spd = cam.speed * dt;
        float trn = cam.tSpeed * dt;

        if (keys['w'] || keys['W']) { dx += sinf(rad)*spd; dz += cosf(rad)*spd; }
        if (keys['s'] || keys['S']) { dx -= sinf(rad)*spd; dz -= cosf(rad)*spd; }
        if (keys['a'] || keys['A']) cam.angle += trn;
        if (keys['d'] || keys['D']) cam.angle -= trn;

        // Collision dengan margin
        float m  = 0.4f;
        float nx = cam.x + dx;
        float nz = cam.z + dz;

        if (isWalkable(nx + m, cam.z) && isWalkable(nx - m, cam.z))
            cam.x = nx;
        if (isWalkable(cam.x, nz + m) && isWalkable(cam.x, nz - m))
            cam.z = nz;
    }

    collectKeysIfNear();

    if (allKeysCollected()) {
        float exitX = (EXIT_COL + 0.5f) * CELL;
        float exitZ = (EXIT_ROW + 0.5f) * CELL;
        float dx = cam.x - exitX;
        float dz = cam.z - exitZ;
        if (dx * dx + dz * dz < 0.55f * 0.55f) {
            printf("[EXIT] Pintu keluar terbuka. Level selesai.\n");
            exit(0);
        }
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

    // Toggle view mode
    if (k == 'v' || k == 'V') {
        viewMode = 1 - viewMode;
        printf("[V] Mode: %s\n", viewMode == 0 ? "First-Person" : "Top-Down");
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

    // Material default
    GLfloat matSpec[] = {0.2f, 0.18f, 0.12f, 1.0f};
    glMaterialfv(GL_FRONT, GL_SPECULAR,  matSpec);
    glMaterialf(GL_FRONT,  GL_SHININESS, 16.0f);

    printf("===========================================\n");
    printf("  MEMORY MAZE — Struktur Maze             \n");
    printf("===========================================\n");
    printf("  WASD  : Gerak & Putar (First-Person)    \n");
    printf("  V     : Toggle First-Person / Top-Down  \n");
    printf("  ESC   : Keluar                          \n");
    printf("  Biru  : Titik Spawn                     \n");
    printf("  Hijau : Titik Exit                      \n");
    printf("===========================================\n");
}

// ============================================================
//  MAIN
// ============================================================
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(WIN_W, WIN_H);
    glutInitWindowPosition(100, 50);
    glutCreateWindow(WIN_NAME);

    initGL();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyDown);
    glutKeyboardUpFunc(keyUp);
    glutTimerFunc(16, update, 0);

    glutMainLoop();
    return 0;
}