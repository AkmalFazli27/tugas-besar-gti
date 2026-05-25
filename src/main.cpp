// ============================================================
//  MEMORY MAZE — Entry Point
//  Tahap 2: memorisasi peta, gelap, flashlight, kode warna, timer
//
//  # Compile
//  C:\msys64\mingw64\bin\g++.exe -std=c++17 -g src\game_state.cpp src\game_logic.cpp src\render.cpp src\main.cpp -o memory_maze_game.exe -lfreeglut -lopengl32 -lglu32
//
//  # Run (tambahkan PATH runtime freeglut)
//  $env:Path = "C:\msys64\mingw64\bin;$env:Path"
//  .\memory_maze_game.exe
// ============================================================

#include "game.h"

// ============================================================
//  GLUT — DISPLAY
// ============================================================
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    viewMode = (gameState == STATE_MEMORIZE || mapRevealActive) ? 1 : 0;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if (viewMode == 0) {
        gluPerspective(FIRST_PERSON_FOV, (double)WIN_W / WIN_H, 0.1, FIRST_PERSON_FAR);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        float radYaw = toRad(cam.angle);
        float radPitch = toRad(cam.pitch);
        float lx  = cam.x + sinf(radYaw) * cosf(radPitch);
        float ly  = cam.y + sinf(radPitch);
        float lz  = cam.z + cosf(radYaw) * cosf(radPitch);
        gluLookAt(cam.x, cam.y, cam.z,
                  lx,    ly,    lz,
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
        GLfloat topDiff[] = {1.0f, 0.95f, 0.85f, 1.0f};
        GLfloat topAmb[]  = {0.48f, 0.44f, 0.38f, 1.0f};
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

        drawExit(true);
        drawCodeSpots(true);

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LIGHTING);
    }

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
        if (keys['a'] || keys['A']) {
            dx += sinf(rad + 3.14159265f / 2.0f) * spd;
            dz += cosf(rad + 3.14159265f / 2.0f) * spd;
        }
        if (keys['d'] || keys['D']) {
            dx += sinf(rad - 3.14159265f / 2.0f) * spd;
            dz += cosf(rad - 3.14159265f / 2.0f) * spd;
        }

        float m  = 0.4f;
        float nx = cam.x + dx;
        float nz = cam.z + dz;

        auto canMoveTo = [m](float x, float z) {
            return isWalkable(x + m, z + m) &&
                   isWalkable(x - m, z + m) &&
                   isWalkable(x + m, z - m) &&
                   isWalkable(x - m, z - m);
        };

        if (canMoveTo(nx, cam.z))
            cam.x = nx;
        if (canMoveTo(cam.x, nz))
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

    if (k == 27) exit(0);

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
                sprintf(msg, "Kunci %s ditemukan: %d", COLOR_NAMES[codeSpots[idx].color], codeSpots[idx].digit);
                setStatus(msg, 3.0f);
            }
        }

        if (k == 13) {
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
        } else if (k == 8) {
            if (inputIndex > 0) {
                inputIndex--;
                enteredCodes[inputIndex] = -1;
            }
        } else if (k == 13) {
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
//  GLUT — MOUSE LOOK
// ============================================================
void mouseMove(int x, int y) {
    if (mouseWarping) {
        mouseWarping = false;
        return;
    }

    int centerX = currentWinW / 2;
    int centerY = currentWinH / 2;

    int dx = x - centerX;
    int dy = y - centerY;

    if (dx == 0 && dy == 0) return;

    cam.angle -= dx * mouseSensitivity;
    cam.pitch += (invertY ? dy : -dy) * mouseSensitivity;

    if (cam.pitch > 85.0f) cam.pitch = 85.0f;
    if (cam.pitch < -85.0f) cam.pitch = -85.0f;

    mouseWarping = true;
    glutWarpPointer(centerX, centerY);
}

// ============================================================
//  GLUT — RESHAPE
// ============================================================
void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    currentWinW = w;
    currentWinH = h;
    mouseWarping = true;
    glutWarpPointer(currentWinW / 2, currentWinH / 2);
}

// ============================================================
//  INISIALISASI OPENGL
// ============================================================
void initGL() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glClearColor(0.02f, 0.02f, 0.03f, 1.0f);

    initTextures();

    glutSetCursor(GLUT_CURSOR_NONE);
    mouseWarping = false;
    currentWinW = WIN_W;
    currentWinH = WIN_H;
    mouseWarping = true;
    glutWarpPointer(currentWinW / 2, currentWinH / 2);

    GLfloat matSpec[] = {0.2f, 0.18f, 0.12f, 1.0f};
    glMaterialfv(GL_FRONT, GL_SPECULAR,  matSpec);
    glMaterialf(GL_FRONT,  GL_SHININESS, 16.0f);

    printf("===========================================\n");
    printf("  MEMORY MAZE — Game Logic                \n");
    printf("===========================================\n");
    printf("  WASD  : Gerak & Putar                     \n");
    printf("  F     : Flashlight (3x, 5 detik)          \n");
    printf("  M     : Buka peta (3x, 4 detik)           \n");
    printf("  E     : Ambil kunci                       \n");
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
    glutPassiveMotionFunc(mouseMove);
    glutTimerFunc(16, update, 0);

    glutMainLoop();
    return 0;
}
