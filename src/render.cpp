#include "game.h"

// ============================================================
//  RENDER — SATU DINDING
// ============================================================
void drawWall(int col, int row) {
    float x0 = col * CELL,  x1 = x0 + CELL;
    float z0 = row * CELL,  z1 = z0 + CELL;
    float y0 = 0.0f,         y1 = W_HEIGHT;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texWall);
    glColor3f(1.0f, 1.0f, 1.0f);

    glBegin(GL_QUADS);
        glNormal3f(0, 0, 1);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(x0, y0, z1);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(x1, y0, z1);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(x1, y1, z1);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(x0, y1, z1);

        glNormal3f(0, 0, -1);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(x1, y0, z0);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(x0, y0, z0);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(x0, y1, z0);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(x1, y1, z0);

        glNormal3f(-1, 0, 0);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(x0, y0, z0);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(x0, y0, z1);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(x0, y1, z1);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(x0, y1, z0);

        glNormal3f(1, 0, 0);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(x1, y0, z1);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(x1, y0, z0);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(x1, y1, z0);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(x1, y1, z1);

        glNormal3f(0, 1, 0);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(x0, y1, z0);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(x0, y1, z1);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(x1, y1, z1);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(x1, y1, z0);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

// ============================================================
//  RENDER — LANTAI
// ============================================================
void drawFloor() {
    float tw = COLS * CELL;
    float td = ROWS * CELL;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texFloor);
    glColor3f(1.0f, 1.0f, 1.0f);

    float tu = COLS / 2.0f;
    float tv = ROWS / 2.0f;

    glBegin(GL_QUADS);
        glNormal3f(0, 1, 0);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(0,  0, 0);
        glTexCoord2f(tu,  0.0f); glVertex3f(tw, 0, 0);
        glTexCoord2f(tu,  tv);  glVertex3f(tw, 0, td);
        glTexCoord2f(0.0f, tv);  glVertex3f(0,  0, td);
    glEnd();

    glDisable(GL_TEXTURE_2D);
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
//  RENDER — MARKER EXIT
// ============================================================
void drawExit(bool overlay) {
    float x0 = EXIT_COL * CELL + 0.2f;
    float x1 = x0 + CELL - 0.4f;
    float z0 = EXIT_ROW * CELL + 0.2f;
    float z1 = z0 + CELL - 0.4f;
    float y  = overlay ? (W_HEIGHT + 0.05f) : 0.02f;

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
//  RENDER — MARKER SPAWN
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
void drawKeyOrb(int colorIndex, float x, float z) {
    float r = COLOR_RGB[colorIndex][0];
    float g = COLOR_RGB[colorIndex][1];
    float b = COLOR_RGB[colorIndex][2];
    float t = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
    float bob = 0.12f * sinf(t * 2.2f + colorIndex * 1.7f);
    float y = 0.55f + bob;

    GLfloat emis[] = {r * 0.7f, g * 0.7f, b * 0.7f, 1.0f};
    glMaterialfv(GL_FRONT, GL_EMISSION, emis);
    glColor3f(r, g, b);

    glPushMatrix();
        glTranslatef(x, y, z);
        glutSolidSphere(0.20f, 18, 18);
    glPopMatrix();

    GLfloat zero[] = {0,0,0,1};
    glMaterialfv(GL_FRONT, GL_EMISSION, zero);
}

void setupKeyLights() {
    glDisable(GL_LIGHT1);
    glDisable(GL_LIGHT2);
    glDisable(GL_LIGHT3);

    if (viewMode != 0) return;

    for (int i = 0; i < CODE_COUNT; i++) {
        if (codeSpots[i].collected) continue;

        int lightId = GL_LIGHT1 + i;
        float cx = (codeSpots[i].col + 0.5f) * CELL;
        float cz = (codeSpots[i].row + 0.5f) * CELL;
        float r = COLOR_RGB[i][0];
        float g = COLOR_RGB[i][1];
        float b = COLOR_RGB[i][2];

        GLfloat pos[]  = {cx, 0.85f, cz, 1.0f};
        GLfloat diff[] = {r * 0.9f, g * 0.9f, b * 0.9f, 1.0f};
        GLfloat spec[] = {r * 0.35f, g * 0.35f, b * 0.35f, 1.0f};

        glEnable(lightId);
        glLightfv(lightId, GL_POSITION, pos);
        glLightfv(lightId, GL_DIFFUSE,  diff);
        glLightfv(lightId, GL_SPECULAR, spec);
        glLightf(lightId, GL_CONSTANT_ATTENUATION,  1.0f);
        glLightf(lightId, GL_LINEAR_ATTENUATION,    0.75f);
        glLightf(lightId, GL_QUADRATIC_ATTENUATION, 0.6f);
    }
}

void drawCodeSpots(bool overlay) {
    for (int i = 0; i < CODE_COUNT; i++) {
        if (codeSpots[i].collected) continue;
        float cx = (codeSpots[i].col + 0.5f) * CELL;
        float cz = (codeSpots[i].row + 0.5f) * CELL;

        float r = COLOR_RGB[i][0];
        float g = COLOR_RGB[i][1];
        float b = COLOR_RGB[i][2];

        if (!overlay) {
            drawKeyOrb(i, cx, cz);
            continue;
        }

        float x0 = codeSpots[i].col * CELL + 0.25f;
        float x1 = x0 + CELL - 0.5f;
        float z0 = codeSpots[i].row * CELL + 0.25f;
        float z1 = z0 + CELL - 0.5f;
        float y  = W_HEIGHT + 0.06f;

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
    if (viewMode == 0)
        drawCodeSpots(false);
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

    GLfloat ambGlobal[] = {0.020f, 0.018f, 0.016f, 1.0f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambGlobal);

    GLfloat lightPos[]  = {cam.x, cam.y, cam.z, 1.0f};
    GLfloat lightDiff[] = {flashlightActive ? 1.00f : 0.74f,
                           flashlightActive ? 0.86f : 0.64f,
                           flashlightActive ? 0.58f : 0.44f,
                           1.0f};
    GLfloat lightSpec[] = {0.58f, 0.52f, 0.38f, 1.0f};

    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  lightDiff);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpec);

    setupKeyLights();

    float range = flashlightActive ? 4.0f : 3.0f;
    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION,  1.0f);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION,    1.05f / range);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.30f / (range * range));

    glEnable(GL_FOG);
    glFogi(GL_FOG_MODE, GL_EXP2);
    glFogf(GL_FOG_DENSITY, flashlightActive ? 0.08f : 0.12f);
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

    glColor3f(1.0f, 0.85f, 0.4f);
    const char* stateStr = "";
    if (gameState == STATE_MEMORIZE) stateStr = "[ MEMORIZE MAP ]";
    else if (gameState == STATE_PLAY) stateStr = "[ PLAY ]";
    else if (gameState == STATE_ENTER_CODE) stateStr = "[ ENTER CODE ]";
    else if (gameState == STATE_WIN) stateStr = "[ YOU WIN ]";
    else if (gameState == STATE_LOSE) stateStr = "[ TIME OUT ]";
    drawText(WIN_W / 2 - 80, WIN_H - 30, stateStr, GLUT_BITMAP_HELVETICA_18);

    char buf[128];
    if (gameState == STATE_MEMORIZE) {
        sprintf(buf, "Waktu hafalan: %.0f", memorizeTimeLeft);
    } else {
        sprintf(buf, "Sisa waktu: %.0f", gameTimeLeft);
    }
    glColor3f(0.85f, 0.85f, 0.85f);
    drawText(20, WIN_H - 30, buf, GLUT_BITMAP_HELVETICA_12);

    sprintf(buf, "Flashlight: %d", flashlightUses);
    glColor3f(0.8f, 0.8f, 0.6f);
    drawText(20, WIN_H - 46, buf, GLUT_BITMAP_HELVETICA_12);
    if (flashlightActive) {
        sprintf(buf, "Aktif: %.1fs", flashlightTimeLeft);
        drawText(20, WIN_H - 62, buf, GLUT_BITMAP_HELVETICA_12);
    }

    sprintf(buf, "Map reveal: %d", mapRevealUses);
    glColor3f(0.7f, 0.8f, 1.0f);
    drawText(20, WIN_H - 78, buf, GLUT_BITMAP_HELVETICA_12);
    if (mapRevealActive) {
        sprintf(buf, "Map aktif: %.1fs", mapRevealTimeLeft);
        drawText(20, WIN_H - 94, buf, GLUT_BITMAP_HELVETICA_12);
    }

    int y = WIN_H - 118;
    for (int i = 0; i < CODE_COUNT; i++) {
        glColor3f(COLOR_RGB[i][0], COLOR_RGB[i][1], COLOR_RGB[i][2]);
        sprintf(buf, "Kunci %s: %s", COLOR_NAMES[i], codeCollected[i] ? "OK" : "--");
        drawText(20, y, buf, GLUT_BITMAP_HELVETICA_12);
        y -= 16;
    }

    if (statusMsgTimer > 0.0f) {
        glColor3f(1.0f, 0.8f, 0.3f);
        drawText(WIN_W / 2 - 170, 40, statusMsg, GLUT_BITMAP_HELVETICA_12);
    }

    if (gameState == STATE_PLAY) {
        int codeIdx = codeIndexAtPlayer();
        if (codeIdx != -1) {
            glColor3f(1.0f, 1.0f, 1.0f);
            sprintf(buf, "Tekan E untuk ambil kunci %s", COLOR_NAMES[codeIdx]);
            drawText(WIN_W / 2 - 160, 64, buf, GLUT_BITMAP_HELVETICA_12);
        }
        if (isPlayerAt(EXIT_ROW, EXIT_COL)) {
            glColor3f(0.7f, 1.0f, 0.7f);
            if (allCodesCollected()) {
                drawText(WIN_W / 2 - 150, 80, "Tekan ENTER untuk input kode", GLUT_BITMAP_HELVETICA_12);
            } else {
                drawText(WIN_W / 2 - 150, 80, "Exit terkunci: cari 3 kunci", GLUT_BITMAP_HELVETICA_12);
            }
        }
    }

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
