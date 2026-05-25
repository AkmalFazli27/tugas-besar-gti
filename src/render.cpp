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
//  RENDER — HELPER FUNCTIONS
// ============================================================

void drawFilledRect(float x, float y, float w, float h) {
    glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x + w, y);
        glVertex2f(x + w, y + h);
        glVertex2f(x, y + h);
    glEnd();
}

void drawOutlinedRect(float x, float y, float w, float h, float thickness) {
    // Top
    glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x + w, y);
        glVertex2f(x + w, y + thickness);
        glVertex2f(x, y + thickness);
    glEnd();
    // Bottom
    glBegin(GL_QUADS);
        glVertex2f(x, y + h - thickness);
        glVertex2f(x + w, y + h - thickness);
        glVertex2f(x + w, y + h);
        glVertex2f(x, y + h);
    glEnd();
    // Left
    glBegin(GL_QUADS);
        glVertex2f(x, y + thickness);
        glVertex2f(x + thickness, y + thickness);
        glVertex2f(x + thickness, y + h - thickness);
        glVertex2f(x, y + h - thickness);
    glEnd();
    // Right
    glBegin(GL_QUADS);
        glVertex2f(x + w - thickness, y + thickness);
        glVertex2f(x + w, y + thickness);
        glVertex2f(x + w, y + h - thickness);
        glVertex2f(x + w - thickness, y + h - thickness);
    glEnd();
}

void drawFilledCircle(float cx, float cy, float r, int segments) {
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(cx, cy);
        for (int i = 0; i <= segments; i++) {
            float angle = 2.0f * 3.14159265f * i / segments;
            glVertex2f(cx + cosf(angle) * r, cy + sinf(angle) * r);
        }
    glEnd();
}

void drawOutlinedCircle(float cx, float cy, float r, int segments, float thickness) {
    // Outer circle
    glBegin(GL_TRIANGLE_STRIP);
        for (int i = 0; i <= segments; i++) {
            float angle = 2.0f * 3.14159265f * i / segments;
            glVertex2f(cx + cosf(angle) * (r + thickness/2), cy + sinf(angle) * (r + thickness/2));
            glVertex2f(cx + cosf(angle) * (r - thickness/2), cy + sinf(angle) * (r - thickness/2));
        }
    glEnd();
}

void drawProgressBar(float x, float y, float w, float h, float progress,
                     float r, float g, float b, float bgR, float bgG, float bgB) {
    // Background
    glColor3f(bgR, bgG, bgB);
    drawFilledRect(x, y, w, h);
    
    // Foreground
    glColor3f(r, g, b);
    drawFilledRect(x, y, w * progress, h);
}

void drawPanel(float x, float y, float w, float h, float alpha) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.0f, 0.0f, 0.0f, alpha);
    drawFilledRect(x, y, w, h);
    glDisable(GL_BLEND);
}

static int textWidth(void* font, const char* s) {
    int width = 0;
    const unsigned char* p = (const unsigned char*)s;
    while (*p) {
        width += glutBitmapWidth(font, *p);
        ++p;
    }
    return width;
}

static int textWidth18(const char* s) {
    return textWidth(GLUT_BITMAP_HELVETICA_18, s);
}

static int textWidth12(const char* s) {
    return textWidth(GLUT_BITMAP_HELVETICA_12, s);
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
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    char buf[256];
    float topStripY = WIN_H - 56.0f;
    float topStripH = 40.0f;

    // ============================================================
    //  STATE BANNER — Top center
    // ============================================================
    const char* stateStr = "";
    float bannerColor[3] = {0.85f, 0.75f, 0.4f};
    if (gameState == STATE_MEMORIZE) {
        stateStr = "HAPALKAN PETA";
        bannerColor[0] = 0.4f; bannerColor[1] = 0.8f; bannerColor[2] = 1.0f;
    } else if (gameState == STATE_PLAY) {
        stateStr = "BERTAHAN HIDUP";
        bannerColor[0] = 1.0f; bannerColor[1] = 0.6f; bannerColor[2] = 0.2f;
    } else if (gameState == STATE_ENTER_CODE) {
        stateStr = "MASUKKAN KODE";
        bannerColor[0] = 0.8f; bannerColor[1] = 0.8f; bannerColor[2] = 0.3f;
    } else if (gameState == STATE_WIN) {
        stateStr = "BERHASIL";
        bannerColor[0] = 0.2f; bannerColor[1] = 1.0f; bannerColor[2] = 0.4f;
    } else if (gameState == STATE_LOSE) {
        stateStr = "GAGAL";
        bannerColor[0] = 1.0f; bannerColor[1] = 0.2f; bannerColor[2] = 0.2f;
    }

    {
        int textW = textWidth18(stateStr);
        float sw = (float)(textW + 32);
        float sx = WIN_W / 2.0f - sw / 2.0f;
        float sy = topStripY;
        float sh = topStripH;

        glColor4f(0.08f, 0.08f, 0.1f, 0.75f);
        drawFilledRect(sx, sy, sw, sh);

        glColor3f(bannerColor[0], bannerColor[1], bannerColor[2]);
        glLineWidth(1.5f);
        glBegin(GL_LINE_LOOP);
            glVertex2f(sx, sy);
            glVertex2f(sx + sw, sy);
            glVertex2f(sx + sw, sy + sh);
            glVertex2f(sx, sy + sh);
        glEnd();
        glLineWidth(1.0f);

        int textY = (int)(sy + (sh - 18.0f) / 2.0f + 2.0f);
        drawText((int)(sx + (sw - textW) / 2.0f), textY, stateStr, GLUT_BITMAP_HELVETICA_18);
    }

    // ============================================================
    //  TIMER BAR — Top left
    // ============================================================
    if (gameState == STATE_MEMORIZE || gameState == STATE_PLAY || gameState == STATE_ENTER_CODE) {
        float maxTime = (gameState == STATE_MEMORIZE) ? MEMORIZE_DURATION : GAME_DURATION;
        float curTime = (gameState == STATE_MEMORIZE) ? memorizeTimeLeft : gameTimeLeft;
        float progress = curTime / maxTime;

        float barX = 20.0f;
        float barW = 220.0f;
        float barH = 30.0f;
        float barY = topStripY + (topStripH - barH) / 2.0f;

        // Background panel
        glColor4f(0.06f, 0.06f, 0.08f, 0.75f);
        drawFilledRect(barX, barY, barW, barH);

        // Progress bar
        float pad = 3.0f;
        float innerX = barX + pad;
        float innerY = barY + pad;
        float innerW = barW - pad * 2;
        float innerH = barH - pad * 2;

        glColor3f(0.12f, 0.12f, 0.14f);
        drawFilledRect(innerX, innerY, innerW, innerH);

        float barR = 1.0f, barG = 0.3f, barB = 0.2f;
        if (progress > 0.5f) {
            float t = (progress - 0.5f) * 2.0f;
            barR = 0.3f + t * 0.7f;
            barG = 0.85f;
            barB = 0.2f;
        } else if (progress > 0.25f) {
            float t = (progress - 0.25f) * 4.0f;
            barR = 0.85f;
            barG = 0.85f - t * 0.55f;
            barB = 0.2f;
        }
        glColor3f(barR, barG, barB);
        if (progress > 0.01f)
            drawFilledRect(innerX, innerY, innerW * progress, innerH);

        // Time text
        float timeVal = (gameState == STATE_MEMORIZE) ? memorizeTimeLeft : gameTimeLeft;
        sprintf(buf, gameState == STATE_MEMORIZE ? "%.0f" : "%.0f", timeVal);
        int textW = textWidth18(buf);
        int textY = (int)(barY + (barH - 18.0f) / 2.0f + 2.0f);
        glColor3f(0.95f, 0.95f, 0.95f);
        drawText((int)(barX + (barW - textW) / 2.0f), textY, buf, GLUT_BITMAP_HELVETICA_18);
    }

    // ============================================================
    //  LEFT PANEL — Items & Keys
    // ============================================================
    float panelX = 14.0f;
    float panelY = WIN_H - 250.0f;
    float panelW = 195.0f;
    float panelH = 195.0f;

    glColor4f(0.06f, 0.06f, 0.08f, 0.7f);
    drawFilledRect(panelX, panelY, panelW, panelH);
    glColor3f(0.25f, 0.25f, 0.3f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(panelX, panelY);
        glVertex2f(panelX + panelW, panelY);
        glVertex2f(panelX + panelW, panelY + panelH);
        glVertex2f(panelX, panelY + panelH);
    glEnd();

    float lx = panelX + 12.0f;
    float ly = panelY + panelH - 24.0f;

    // Flashlight
    {
        glColor3f(0.95f, 0.85f, 0.4f);
        drawFilledCircle(lx + 7, ly, 6, 12);

        sprintf(buf, "Senter:");
        glColor3f(0.65f, 0.65f, 0.6f);
        drawText((int)(lx + 18), (int)(ly - 5), buf, GLUT_BITMAP_HELVETICA_12);

        sprintf(buf, "%d / %d", flashlightUses, FLASHLIGHT_USES);
        glColor3f(0.95f, 0.85f, 0.4f);
        drawText((int)(lx + 80), (int)(ly - 5), buf, GLUT_BITMAP_HELVETICA_12);

        if (flashlightActive) {
            sprintf(buf, "AKTIF %.1f", flashlightTimeLeft);
            glColor3f(0.95f, 0.75f, 0.2f);
            drawText((int)(lx + 18), (int)(ly - 22), buf, GLUT_BITMAP_HELVETICA_12);
        }
    }

    // Separator
    glColor3f(0.15f, 0.15f, 0.18f);
    glBegin(GL_LINES);
        glVertex2f(panelX + 8, ly - 28);
        glVertex2f(panelX + panelW - 8, ly - 28);
    glEnd();

    // Map Reveal
    ly -= 38;
    {
        glColor3f(0.5f, 0.7f, 1.0f);
        drawFilledRect(lx + 2, ly - 6, 12, 12);
        glColor3f(0.15f, 0.15f, 0.2f);
        drawFilledRect(lx + 5, ly - 3, 6, 6);

        sprintf(buf, "Peta:");
        glColor3f(0.65f, 0.65f, 0.6f);
        drawText((int)(lx + 20), (int)(ly - 5), buf, GLUT_BITMAP_HELVETICA_12);

        sprintf(buf, "%d / %d", mapRevealUses, MAP_REVEAL_USES);
        glColor3f(0.5f, 0.7f, 1.0f);
        drawText((int)(lx + 80), (int)(ly - 5), buf, GLUT_BITMAP_HELVETICA_12);

        if (mapRevealActive) {
            sprintf(buf, "AKTIF %.1f", mapRevealTimeLeft);
            glColor3f(0.5f, 0.7f, 1.0f);
            drawText((int)(lx + 18), (int)(ly - 22), buf, GLUT_BITMAP_HELVETICA_12);
        }
    }

    // Separator
    glColor3f(0.15f, 0.15f, 0.18f);
    glBegin(GL_LINES);
        glVertex2f(panelX + 8, ly - 28);
        glVertex2f(panelX + panelW - 8, ly - 28);
    glEnd();

    // Key status
    ly -= 38;
    glColor3f(0.75f, 0.75f, 0.65f);
    drawText((int)(lx), (int)(ly - 4), "KUNCI", GLUT_BITMAP_HELVETICA_12);
    ly -= 20;
    for (int i = 0; i < CODE_COUNT; i++) {
        float cr = COLOR_RGB[i][0];
        float cg = COLOR_RGB[i][1];
        float cb = COLOR_RGB[i][2];

        if (codeCollected[i]) {
            glColor3f(cr * 0.6f, cg * 0.6f, cb * 0.6f);
            drawFilledCircle(lx + 6, ly + 3, 7, 16);
            glColor3f(cr, cg, cb);
            drawFilledCircle(lx + 6, ly + 3, 5, 16);
        } else {
            glColor3f(0.2f, 0.2f, 0.25f);
            drawFilledCircle(lx + 6, ly + 3, 7, 16);
            glColor3f(cr * 0.3f, cg * 0.3f, cb * 0.3f);
            drawOutlinedCircle(lx + 6, ly + 3, 5, 16, 2.0f);
        }

        sprintf(buf, "%-7s: %s", COLOR_NAMES[i], codeCollected[i] ? "OK" : "--");
        glColor3f(cr, cg, cb);
        drawText((int)(lx + 18), (int)(ly - 4), buf, GLUT_BITMAP_HELVETICA_12);
        ly -= 18;
    }

    // ============================================================
    //  CROSSHAIR — Center
    // ============================================================
    if (viewMode == 0 && gameState == STATE_PLAY) {
        float cx = WIN_W / 2.0f;
        float cy = WIN_H / 2.0f;
        float gap = 6.0f;
        float len = 14.0f;

        glColor4f(1.0f, 1.0f, 1.0f, 0.6f);
        glLineWidth(1.5f);

        glBegin(GL_LINES);
            glVertex2f(cx, cy + gap); glVertex2f(cx, cy + gap + len);
            glVertex2f(cx, cy - gap); glVertex2f(cx, cy - gap - len);
            glVertex2f(cx + gap, cy); glVertex2f(cx + gap + len, cy);
            glVertex2f(cx - gap, cy); glVertex2f(cx - gap - len, cy);
        glEnd();

        // Center dot
        glColor4f(1.0f, 1.0f, 1.0f, 0.4f);
        drawFilledCircle(cx, cy, 2.0f, 12);

        glLineWidth(1.0f);
    }

    // ============================================================
    //  STATUS MESSAGES — Center bottom
    // ============================================================
    if (statusMsgTimer > 0.0f) {
        float alpha = (statusMsgTimer < 0.5f) ? (statusMsgTimer / 0.5f) : 0.85f;
        int textW = textWidth18(statusMsg);
        int msgW = textW + 32;

        float msgX = WIN_W / 2.0f - msgW / 2.0f;
        float msgY = 40.0f;
        float msgH = 30.0f;

        glColor4f(0.0f, 0.0f, 0.0f, alpha * 0.7f);
        drawFilledRect(msgX, msgY, (float)msgW, msgH);

        glColor4f(0.4f, 0.35f, 0.2f, alpha * 0.8f);
        glBegin(GL_LINE_LOOP);
            glVertex2f(msgX, msgY);
            glVertex2f(msgX + msgW, msgY);
            glVertex2f(msgX + msgW, msgY + msgH);
            glVertex2f(msgX, msgY + msgH);
        glEnd();

        glColor4f(1.0f, 0.85f, 0.4f, alpha);
        int textY = (int)(msgY + (msgH - 18.0f) / 2.0f + 2.0f);
        drawText((int)(msgX + (msgW - textW) / 2.0f), textY, statusMsg, GLUT_BITMAP_HELVETICA_18);
    }

    // ============================================================
    //  CONTEXT PROMPTS — STATE_PLAY
    // ============================================================
    if (gameState == STATE_PLAY) {
        int codeIdx = codeIndexAtPlayer();
        if (codeIdx != -1) {
            sprintf(buf, "Tekan [E] untuk ambil kunci %s", COLOR_NAMES[codeIdx]);
            int textW = textWidth18(buf);
            float pw = (float)(textW + 32);
            float ph = 30.0f;
            float px = WIN_W / 2.0f - pw / 2.0f;
            float py = 80.0f;

            glColor4f(0.0f, 0.0f, 0.0f, 0.65f);
            drawFilledRect(px, py, pw, ph);
            glColor3f(COLOR_RGB[codeIdx][0], COLOR_RGB[codeIdx][1], COLOR_RGB[codeIdx][2]);
            glBegin(GL_LINE_LOOP);
                glVertex2f(px, py);
                glVertex2f(px + pw, py);
                glVertex2f(px + pw, py + ph);
                glVertex2f(px, py + ph);
            glEnd();

            glColor3f(COLOR_RGB[codeIdx][0], COLOR_RGB[codeIdx][1], COLOR_RGB[codeIdx][2]);
            int textY = (int)(py + (ph - 18.0f) / 2.0f + 2.0f);
            drawText((int)(px + (pw - textW) / 2), textY, buf, GLUT_BITMAP_HELVETICA_18);
        }
        if (isPlayerAt(EXIT_ROW, EXIT_COL)) {
            if (allCodesCollected()) {
                const char* prompt = "Tekan [ENTER] input kode exit";
                int textW = textWidth18(prompt);
                float pw = (float)(textW + 32);
                float ph = 30.0f;
                float px = WIN_W / 2.0f - pw / 2.0f;
                float py = 112.0f;
                glColor4f(0.0f, 0.0f, 0.0f, 0.65f);
                drawFilledRect(px, py, pw, ph);
                glColor3f(0.3f, 1.0f, 0.5f);
                glBegin(GL_LINE_LOOP);
                    glVertex2f(px, py);
                    glVertex2f(px + pw, py);
                    glVertex2f(px + pw, py + ph);
                    glVertex2f(px, py + ph);
                glEnd();
                int textY = (int)(py + (ph - 18.0f) / 2.0f + 2.0f);
                drawText((int)(px + (pw - textW) / 2), textY, prompt, GLUT_BITMAP_HELVETICA_18);
            } else {
                int collectedCount = 0;
                for (int ci = 0; ci < CODE_COUNT; ci++) {
                    if (codeCollected[ci]) collectedCount++;
                }
                sprintf(buf, "Exit terkunci: cari %d kunci tersisa", CODE_COUNT - collectedCount);
                int textW = textWidth18(buf);
                float pw = (float)(textW + 32);
                float ph = 30.0f;
                float px = WIN_W / 2.0f - pw / 2.0f;
                float py = 112.0f;
                glColor4f(0.0f, 0.0f, 0.0f, 0.65f);
                drawFilledRect(px, py, pw, ph);
                glColor3f(1.0f, 0.5f, 0.3f);
                glBegin(GL_LINE_LOOP);
                    glVertex2f(px, py);
                    glVertex2f(px + pw, py);
                    glVertex2f(px + pw, py + ph);
                    glVertex2f(px, py + ph);
                glEnd();
                int textY = (int)(py + (ph - 18.0f) / 2.0f + 2.0f);
                drawText((int)(px + (pw - textW) / 2), textY, buf, GLUT_BITMAP_HELVETICA_18);
            }
        }
    }

    // ============================================================
    //  CODE INPUT SCREEN
    // ============================================================
    if (gameState == STATE_ENTER_CODE) {
        float overlayW = 360.0f;
        float overlayH = 200.0f;
        float overlayX = WIN_W / 2.0f - overlayW / 2.0f;
        float overlayY = WIN_H / 2.0f - overlayH / 2.0f;

        glColor4f(0.05f, 0.05f, 0.08f, 0.85f);
        drawFilledRect(overlayX, overlayY, overlayW, overlayH);
        glColor3f(0.35f, 0.35f, 0.4f);
        glLineWidth(1.5f);
        glBegin(GL_LINE_LOOP);
            glVertex2f(overlayX, overlayY);
            glVertex2f(overlayX + overlayW, overlayY);
            glVertex2f(overlayX + overlayW, overlayY + overlayH);
            glVertex2f(overlayX, overlayY + overlayH);
        glEnd();
        glLineWidth(1.0f);

        glColor3f(1.0f, 1.0f, 0.85f);
        drawText((int)(overlayX + overlayW / 2 - 80), (int)(overlayY + overlayH - 40), "INPUT KODE EXIT", GLUT_BITMAP_HELVETICA_18);

        glColor3f(0.7f, 0.7f, 0.7f);
        drawText((int)(overlayX + 30), (int)(overlayY + overlayH - 65), "0-9  BACKSPACE  ENTER", GLUT_BITMAP_HELVETICA_12);

        float boxY = overlayY + 30.0f;
        float boxXStart = overlayX + 20.0f;
        float boxW = 96.0f;
        float boxH = 70.0f;
        float gap = 10.0f;

        for (int i = 0; i < CODE_COUNT; i++) {
            CodeColor col = INPUT_ORDER[i];
            float bx = boxXStart + (boxW + gap) * i;
            float cr = COLOR_RGB[col][0];
            float cg = COLOR_RGB[col][1];
            float cb = COLOR_RGB[col][2];

            // Box background
            glColor4f(cr * 0.12f, cg * 0.12f, cb * 0.12f, 0.6f);
            drawFilledRect(bx, boxY, boxW, boxH);

            // Border
            if (i == inputIndex) {
                glColor3f(cr, cg, cb);
                glLineWidth(2.0f);
            } else {
                glColor3f(cr * 0.5f, cg * 0.5f, cb * 0.5f);
                glLineWidth(1.0f);
            }
            glBegin(GL_LINE_LOOP);
                glVertex2f(bx, boxY);
                glVertex2f(bx + boxW, boxY);
                glVertex2f(bx + boxW, boxY + boxH);
                glVertex2f(bx, boxY + boxH);
            glEnd();
            glLineWidth(1.0f);

            // Color label
            glColor3f(cr * 0.7f, cg * 0.7f, cb * 0.7f);
            drawText((int)(bx + 8), (int)(boxY + boxH - 18), COLOR_NAMES[col], GLUT_BITMAP_HELVETICA_12);

            // Digit
            char digitStr[4];
            if (enteredCodes[i] >= 0) {
                sprintf(digitStr, "%d", enteredCodes[i]);
                glColor3f(cr, cg, cb);
                drawText((int)(bx + boxW / 2 - 8), (int)(boxY + 12), digitStr, GLUT_BITMAP_HELVETICA_18);
            } else {
                if (i == inputIndex) {
                    // Blinking cursor
                    float blink = sinf(glutGet(GLUT_ELAPSED_TIME) / 100.0f) * 0.5f + 0.5f;
                    glColor4f(cr, cg, cb, blink);
                    drawText((int)(bx + boxW / 2 - 4), (int)(boxY + 12), "_", GLUT_BITMAP_HELVETICA_18);
                }
            }
        }
    }

    // ============================================================
    //  WIN / LOSE SCREEN
    // ============================================================
    if (gameState == STATE_WIN || gameState == STATE_LOSE) {
        float overlayW = 400.0f;
        float overlayH = 120.0f;
        float overlayX = WIN_W / 2.0f - overlayW / 2.0f;
        float overlayY = WIN_H / 2.0f - overlayH / 2.0f;

        glColor4f(0.05f, 0.05f, 0.08f, 0.85f);
        drawFilledRect(overlayX, overlayY, overlayW, overlayH);

        if (gameState == STATE_WIN) {
            glColor3f(0.2f, 1.0f, 0.4f);
            glLineWidth(2.0f);
            glBegin(GL_LINE_LOOP);
                glVertex2f(overlayX, overlayY);
                glVertex2f(overlayX + overlayW, overlayY);
                glVertex2f(overlayX + overlayW, overlayY + overlayH);
                glVertex2f(overlayX, overlayY + overlayH);
            glEnd();
            glLineWidth(1.0f);

            glColor3f(0.2f, 1.0f, 0.4f);
            drawText((int)(overlayX + overlayW / 2 - 90), (int)(overlayY + 70), "KAMU BERHASIL KELUAR!", GLUT_BITMAP_HELVETICA_18);
        } else {
            glColor3f(1.0f, 0.2f, 0.2f);
            glLineWidth(2.0f);
            glBegin(GL_LINE_LOOP);
                glVertex2f(overlayX, overlayY);
                glVertex2f(overlayX + overlayW, overlayY);
                glVertex2f(overlayX + overlayW, overlayY + overlayH);
                glVertex2f(overlayX, overlayY + overlayH);
            glEnd();
            glLineWidth(1.0f);

            glColor3f(1.0f, 0.2f, 0.2f);
            drawText((int)(overlayX + overlayW / 2 - 70), (int)(overlayY + 70), "WAKTU HABIS!", GLUT_BITMAP_HELVETICA_18);
        }

        glColor3f(0.7f, 0.7f, 0.7f);
        drawText((int)(overlayX + overlayW / 2 - 120), (int)(overlayY + 30), "Tekan ESC untuk keluar", GLUT_BITMAP_HELVETICA_12);
    }

    glDisable(GL_BLEND);
    glEnable(GL_FOG);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}
