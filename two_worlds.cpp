// ============================================================
//  TWO WORLDS MAZE — Alpha Prototype
//  Gambaran umum konsep, belum lengkap (alpha)
//
//  Kompilasi Windows (MinGW):
//    g++ two_worlds.cpp -o two_worlds -lfreeglut -lopengl32 -lglu32
//  Kompilasi Linux:
//    g++ two_worlds.cpp -o two_worlds -lglut -lGL -lGLU -lm
// ============================================================

#include <GL/glut.h>
#include <cmath>
#include <cstdio>

// ============================================================
//  KONFIGURASI DASAR
// ============================================================
const int  WIN_W     = 1280;
const int  WIN_H     = 720;
const int  ROWS      = 9;
const int  COLS      = 9;
const float CELL     = 2.0f;
const float W_HEIGHT = 2.5f;

// ============================================================
//  DUA GRID MAZE
//  1 = dinding, 0 = lorong
// ============================================================

// World A — Dunia Nyata (hangat, oranye)
int mazeA[ROWS][COLS] = {
    {1,1,1,1,1,1,1,1,1},
    {1,0,0,0,1,0,0,0,1},
    {1,0,1,0,1,0,1,0,1},
    {1,0,1,0,0,0,1,0,1},
    {1,0,1,1,1,1,1,0,1},
    {1,0,0,0,0,0,0,0,1},
    {1,1,1,0,1,0,1,1,1},
    {1,0,0,0,1,0,0,0,1},
    {1,1,1,1,1,1,1,1,1}
};

// World B — Dunia Arwah (dingin, biru)
// Dinding B = lorong A di beberapa bagian (komplementer)
int mazeB[ROWS][COLS] = {
    {1,1,1,1,1,1,1,1,1},
    {1,0,1,0,0,0,1,0,1},
    {1,0,0,0,1,0,0,0,1},
    {1,1,1,0,1,0,1,1,1},
    {1,0,0,0,0,0,0,0,1},
    {1,0,1,1,1,1,1,0,1},
    {1,0,0,0,1,0,0,0,1},
    {1,0,1,0,0,0,1,0,1},
    {1,1,1,1,1,1,1,1,1}
};

// ============================================================
//  STATE
// ============================================================
int   currentWorld = 0;      // 0 = World A, 1 = World B
bool  switching    = false;  // sedang animasi switch?
float switchAnim   = 0.0f;   // 0.0 → 1.0 (progress animasi)
float switchCool   = 0.0f;   // cooldown switch (detik)

// Flash merah saat gagal switch
float failFlash    = 0.0f;

// ============================================================
//  PLAYER (First-Person Camera Sederhana)
// ============================================================
struct {
    float x     = 1.5f * CELL;
    float z     = 1.5f * CELL;
    float angle = 0.0f;       // derajat, rotasi Y
    float pitch = 0.0f;       // derajat, rotasi X (lihat atas/bawah)
    float y     = 1.2f;       // tinggi mata
} player;

bool keys[256] = {};

// Mouse look
float mouseSensitivity = 0.15f;
bool  ignoreMouseWarp  = false;

// ============================================================
//  UTILITY
// ============================================================
float toRad(float d) { return d * 3.14159f / 180.0f; }

bool walkable(float wx, float wz, int world) {
    int r = (int)(wz / CELL);
    int c = (int)(wx / CELL);
    if (r < 0 || r >= ROWS || c < 0 || c >= COLS) return false;
    int (*maze)[COLS] = (world == 0) ? mazeA : mazeB;
    return maze[r][c] == 0;
}

// ============================================================
//  RENDER — SATU DINDING (BOX)
// ============================================================
void drawWall(int col, int row) {
    float x0 = col * CELL, x1 = x0 + CELL;
    float z0 = row * CELL, z1 = z0 + CELL;
    float y0 = 0, y1 = W_HEIGHT;

    glBegin(GL_QUADS);
        // Depan
        glNormal3f(0,0,1);
        glVertex3f(x0,y0,z1); glVertex3f(x1,y0,z1);
        glVertex3f(x1,y1,z1); glVertex3f(x0,y1,z1);
        // Belakang
        glNormal3f(0,0,-1);
        glVertex3f(x1,y0,z0); glVertex3f(x0,y0,z0);
        glVertex3f(x0,y1,z0); glVertex3f(x1,y1,z0);
        // Kiri
        glNormal3f(-1,0,0);
        glVertex3f(x0,y0,z0); glVertex3f(x0,y0,z1);
        glVertex3f(x0,y1,z1); glVertex3f(x0,y1,z0);
        // Kanan
        glNormal3f(1,0,0);
        glVertex3f(x1,y0,z1); glVertex3f(x1,y0,z0);
        glVertex3f(x1,y1,z0); glVertex3f(x1,y1,z1);
        // Atas
        glNormal3f(0,1,0);
        glVertex3f(x0,y1,z0); glVertex3f(x0,y1,z1);
        glVertex3f(x1,y1,z1); glVertex3f(x1,y1,z0);
    glEnd();
}

// ============================================================
//  RENDER — LANTAI
// ============================================================
void drawFloor() {
    float tw = COLS * CELL, td = ROWS * CELL;
    glBegin(GL_QUADS);
        glNormal3f(0,1,0);
        glVertex3f(0,0,0);   glVertex3f(tw,0,0);
        glVertex3f(tw,0,td); glVertex3f(0,0,td);
    glEnd();
}

// ============================================================
//  RENDER — SELURUH MAZE (sesuai world aktif)
// ============================================================
void drawMaze() {
    int (*maze)[COLS] = (currentWorld == 0) ? mazeA : mazeB;

    if (currentWorld == 0) {
        // World A — dinding batu hangat
        glColor3f(0.35f, 0.25f, 0.18f);
    } else {
        // World B — dinding ethereal biru
        glColor4f(0.2f, 0.4f, 0.8f, 0.75f);
    }

    for (int r = 0; r < ROWS; r++)
        for (int c = 0; c < COLS; c++)
            if (maze[r][c] == 1)
                drawWall(c, r);

    // Lantai
    if (currentWorld == 0)
        glColor3f(0.18f, 0.14f, 0.10f);
    else
        glColor4f(0.05f, 0.08f, 0.20f, 1.0f);
    drawFloor();
}

// ============================================================
//  SETUP LIGHTING — berbeda tiap world
// ============================================================
void setupLighting() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glShadeModel(GL_SMOOTH);

    // Posisi cahaya ikut player (lantern)
    GLfloat pos[] = {player.x, player.y, player.z, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, pos);

    // Attenuation — jarak pendek agar gelap di kejauhan
    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION,  1.0f);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION,    0.4f);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.08f);

    if (currentWorld == 0) {
        // World A: cahaya obor hangat
        GLfloat amb[] = {0.05f, 0.03f, 0.01f, 1.0f};
        GLfloat dif[] = {1.0f,  0.75f, 0.4f,  1.0f};
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);

        GLfloat fogCol[] = {0.12f, 0.07f, 0.03f, 1.0f};
        glFogfv(GL_FOG_COLOR, fogCol);
        glFogf(GL_FOG_DENSITY, 0.12f);
    } else {
        // World B: cahaya bulan dingin
        GLfloat amb[] = {0.02f, 0.03f, 0.08f, 1.0f};
        GLfloat dif[] = {0.4f,  0.6f,  1.0f,  1.0f};
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);

        GLfloat fogCol[] = {0.03f, 0.05f, 0.18f, 1.0f};
        glFogfv(GL_FOG_COLOR, fogCol);
        glFogf(GL_FOG_DENSITY, 0.18f);
    }
}

// ============================================================
//  RENDER — OVERLAY TRANSISI SWITCH
//  Layar fade saat berpindah dunia
// ============================================================
void drawSwitchOverlay() {
    if (!switching && switchAnim <= 0) return;

    // Masuk ke mode 2D
    glMatrixMode(GL_PROJECTION);
    glPushMatrix(); glLoadIdentity();
    gluOrtho2D(0, WIN_W, 0, WIN_H);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix(); glLoadIdentity();

    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Fade in → gelap total → fade out
    // switchAnim: 0→0.5 fade in, 0.5→1.0 fade out
    float alpha;
    if (switchAnim < 0.5f) alpha = switchAnim * 2.0f;
    else                   alpha = (1.0f - switchAnim) * 2.0f;

    // Warna overlay sesuai world tujuan
    if (currentWorld == 0) // baru saja pindah ke A
        glColor4f(1.0f, 0.6f, 0.1f, alpha);
    else                   // baru saja pindah ke B
        glColor4f(0.1f, 0.3f, 1.0f, alpha);

    glBegin(GL_QUADS);
        glVertex2i(0,0); glVertex2i(WIN_W,0);
        glVertex2i(WIN_W,WIN_H); glVertex2i(0,WIN_H);
    glEnd();

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

// ============================================================
//  RENDER — OVERLAY FLASH MERAH (switch gagal)
// ============================================================
void drawFailFlash() {
    if (failFlash <= 0) return;

    glMatrixMode(GL_PROJECTION);
    glPushMatrix(); glLoadIdentity();
    gluOrtho2D(0, WIN_W, 0, WIN_H);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix(); glLoadIdentity();

    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(1.0f, 0.0f, 0.0f, failFlash * 0.6f);

    glBegin(GL_QUADS);
        glVertex2i(0,0); glVertex2i(WIN_W,0);
        glVertex2i(WIN_W,WIN_H); glVertex2i(0,WIN_H);
    glEnd();

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

// ============================================================
//  RENDER — HUD SEDERHANA
// ============================================================
void drawHUD() {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix(); glLoadIdentity();
    gluOrtho2D(0, WIN_W, 0, WIN_H);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix(); glLoadIdentity();
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    // Label world
    const char* worldLabel = (currentWorld == 0)
        ? "[ WORLD A — Dunia Nyata ]"
        : "[ WORLD B — Dunia Arwah ]";

    if (currentWorld == 0) glColor3f(1.0f, 0.7f, 0.2f);
    else                   glColor3f(0.3f, 0.7f, 1.0f);

    glRasterPos2i(WIN_W/2 - 100, WIN_H - 35);
    for (const char* c = worldLabel; *c; c++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);

    // Kontrol
    glColor3f(0.6f, 0.6f, 0.6f);
    const char* ctrl = "WASD: Gerak  |  Mouse: Kamera  |  E: Switch World  |  ESC: Keluar";
    glRasterPos2i(WIN_W/2 - 170, 18);
    for (const char* c = ctrl; *c; c++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);

    // Cooldown indicator
    if (switchCool > 0) {
        char buf[32];
        sprintf(buf, "Switch cooldown: %.1fs", switchCool);
        glColor3f(1.0f, 0.4f, 0.4f);
        glRasterPos2i(20, WIN_H - 35);
        for (char* c = buf; *c; c++)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
    } else {
        glColor3f(0.4f, 1.0f, 0.4f);
        glRasterPos2i(20, WIN_H - 35);
        const char* ready = "Switch: READY";
        for (const char* c = ready; *c; c++)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
    }

    // Crosshair
    glColor3f(1,1,1);
    glBegin(GL_LINES);
        glVertex2i(WIN_W/2-8, WIN_H/2);
        glVertex2i(WIN_W/2+8, WIN_H/2);
        glVertex2i(WIN_W/2, WIN_H/2-8);
        glVertex2i(WIN_W/2, WIN_H/2+8);
    glEnd();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

// ============================================================
//  SWITCH WORLD — Logika Utama
// ============================================================
void trySwitch() {
    if (switchCool > 0 || switching) return;

    int targetWorld = 1 - currentWorld;
    int (*targetMaze)[COLS] = (targetWorld == 0) ? mazeA : mazeB;

    int gr = (int)(player.z / CELL);
    int gc = (int)(player.x / CELL);

    // Cek: posisi player di dunia tujuan harus bukan dinding
    if (targetMaze[gr][gc] == 1) {
        // GAGAL — posisi adalah dinding di dunia tujuan
        failFlash = 1.0f;
        printf("[!] Gagal switch — posisi ini adalah dinding di dunia lain!\n");
        return;
    }

    // BERHASIL — mulai animasi
    currentWorld = targetWorld;
    switching    = true;
    switchAnim   = 0.0f;
    switchCool   = 3.0f;
    printf("[+] Switch ke %s\n", currentWorld == 0 ? "World A" : "World B");
}

// ============================================================
//  GLUT — DISPLAY
// ============================================================
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Setup kamera perspective (first-person)
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(75.0, (double)WIN_W / WIN_H, 0.1, 50.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    float yawRad   = toRad(player.angle);
    float pitchRad = toRad(player.pitch);
    float dirX = cosf(pitchRad) * sinf(yawRad);
    float dirY = sinf(pitchRad);
    float dirZ = cosf(pitchRad) * cosf(yawRad);
    float lx   = player.x + dirX;
    float ly   = player.y + dirY;
    float lz   = player.z + dirZ;
    gluLookAt(player.x, player.y, player.z,
              lx,       ly,       lz,
              0, 1, 0);

    setupLighting();

    // Dunia B sedikit transparan — aktifkan blend
    if (currentWorld == 1) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    drawMaze();

    if (currentWorld == 1) glDisable(GL_BLEND);

    // Overlay & HUD
    drawSwitchOverlay();
    drawFailFlash();
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
    lastT = t;
    dt = fminf(dt, 0.05f);

    // --- Player movement ---
    float rad = toRad(player.angle);
    float dx  = 0, dz = 0;
    float spd = 3.5f * dt;

    if (keys['w'] || keys['W']) { dx += sinf(rad)*spd; dz += cosf(rad)*spd; }
    if (keys['s'] || keys['S']) { dx -= sinf(rad)*spd; dz -= cosf(rad)*spd; }
    // Strafe kiri/kanan
    if (keys['a'] || keys['A']) { dx += cosf(rad)*spd; dz -= sinf(rad)*spd; }
    if (keys['d'] || keys['D']) { dx -= cosf(rad)*spd; dz += sinf(rad)*spd; }

    float margin = 0.35f;
    float nx = player.x + dx;
    float nz = player.z + dz;
    if (walkable(nx + margin, player.z, currentWorld) &&
        walkable(nx - margin, player.z, currentWorld))
        player.x = nx;
    if (walkable(player.x, nz + margin, currentWorld) &&
        walkable(player.x, nz - margin, currentWorld))
        player.z = nz;

    // --- Animasi switch ---
    if (switching) {
        switchAnim += dt * 1.8f;  // durasi animasi ~0.55 detik
        if (switchAnim >= 1.0f) {
            switchAnim = 0;
            switching  = false;
        }
    }

    // --- Countdown cooldown ---
    if (switchCool > 0) switchCool -= dt;
    if (switchCool < 0) switchCool = 0;

    // --- Fade fail flash ---
    if (failFlash > 0) failFlash -= dt * 3.0f;

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

// ============================================================
//  GLUT — KEYBOARD
// ============================================================
void keyDown(unsigned char k, int x, int y) {
    keys[k] = true;
    if (k == 27) exit(0);               // ESC keluar
    if (k == 'e' || k == 'E') trySwitch();
}
void keyUp(unsigned char k, int x, int y) { keys[k] = false; }

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
    player.pitch -= dy * mouseSensitivity;

    if (player.pitch > 80.0f) player.pitch = 80.0f;
    if (player.pitch < -80.0f) player.pitch = -80.0f;

    ignoreMouseWarp = true;
    glutWarpPointer(centerX, centerY);
}

// ============================================================
//  GLUT — RESHAPE
// ============================================================
void reshape(int w, int h) {
    glViewport(0, 0, w, h);
}

// ============================================================
//  INISIALISASI
// ============================================================
void initGL() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glEnable(GL_FOG);
    glFogi(GL_FOG_MODE, GL_EXP2);
    glClearColor(0, 0, 0, 1);

    printf("=== TWO WORLDS — Alpha Prototype ===\n");
    printf("WASD : Gerak\n");
    printf("Mouse: Kamera\n");
    printf("E    : Switch dunia\n");
    printf("ESC  : Keluar\n");
    printf("=====================================\n");
}

// ============================================================
//  MAIN
// ============================================================
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(WIN_W, WIN_H);
    glutCreateWindow("Two Worlds Maze — Alpha");

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
    glutWarpPointer(WIN_W / 2, WIN_H / 2);

    glutMainLoop();
    return 0;
}