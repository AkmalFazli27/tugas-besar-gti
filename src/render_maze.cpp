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
    glColor3f(0.9f, 0.9f, 0.9f);

    glBegin(GL_QUADS);
        glNormal3f(0, 0, 1);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(x0, y0, z1);
        glTexCoord2f(2.0f, 0.0f); glVertex3f(x1, y0, z1);
        glTexCoord2f(2.0f, 2.0f); glVertex3f(x1, y1, z1);
        glTexCoord2f(0.0f, 2.0f); glVertex3f(x0, y1, z1);

        glNormal3f(0, 0, -1);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(x1, y0, z0);
        glTexCoord2f(2.0f, 0.0f); glVertex3f(x0, y0, z0);
        glTexCoord2f(2.0f, 2.0f); glVertex3f(x0, y1, z0);
        glTexCoord2f(0.0f, 2.0f); glVertex3f(x1, y1, z0);

        glNormal3f(-1, 0, 0);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(x0, y0, z0);
        glTexCoord2f(2.0f, 0.0f); glVertex3f(x0, y0, z1);
        glTexCoord2f(2.0f, 2.0f); glVertex3f(x0, y1, z1);
        glTexCoord2f(0.0f, 2.0f); glVertex3f(x0, y1, z0);

        glNormal3f(1, 0, 0);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(x1, y0, z1);
        glTexCoord2f(2.0f, 0.0f); glVertex3f(x1, y0, z0);
        glTexCoord2f(2.0f, 2.0f); glVertex3f(x1, y1, z0);
        glTexCoord2f(0.0f, 2.0f); glVertex3f(x1, y1, z1);

        glNormal3f(0, 1, 0);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(x0, y1, z0);
        glTexCoord2f(2.0f, 0.0f); glVertex3f(x0, y1, z1);
        glTexCoord2f(2.0f, 2.0f); glVertex3f(x1, y1, z1);
        glTexCoord2f(0.0f, 2.0f); glVertex3f(x1, y1, z0);
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

    float tu = 2.0f;
    float tv = 2.0f;

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

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texFloor);
    glColor3f(1.0f, 1.0f, 1.0f);

    float tu = 2.0f;
    float tv = 2.0f;

    glBegin(GL_QUADS);
        glNormal3f(0, -1, 0);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(0,  W_HEIGHT, 0);
        glTexCoord2f(tu,  0.0f);  glVertex3f(0,  W_HEIGHT, td);
        glTexCoord2f(tu,  tv);   glVertex3f(tw, W_HEIGHT, td);
        glTexCoord2f(0.0f, tv);   glVertex3f(tw, W_HEIGHT, 0);
    glEnd();

    glDisable(GL_TEXTURE_2D);
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
    float rotY = t * 45.0f + colorIndex * 120.0f;

    GLfloat emis[] = {r * 0.7f, g * 0.7f, b * 0.7f, 1.0f};
    glMaterialfv(GL_FRONT, GL_EMISSION, emis);
    glColor3f(r, g, b);

    glPushMatrix();
        glTranslatef(x, y, z);
        glRotatef(rotY, 0, 1, 0);

        // Bow (ring)
        glPushMatrix();
            glRotatef(90, 1, 0, 0);
            glutSolidTorus(0.025f, 0.09f, 8, 16);
        glPopMatrix();

        // Shaft
        glPushMatrix();
            glTranslatef(0.0f, -0.07f, 0.0f);
            glScalef(0.03f, 0.18f, 0.03f);
            glutSolidCube(1.0f);
        glPopMatrix();

        // Teeth
        glPushMatrix();
            glTranslatef(0.01f, -0.20f, 0.0f);
            glScalef(0.04f, 0.035f, 0.025f);
            glutSolidCube(1.0f);
        glPopMatrix();

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

    GLfloat ambGlobal[] = {0.085f, 0.078f, 0.068f, 1.0f};
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

    float range = flashlightActive ? 5.0f : 2.0f;
    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION,  1.0f);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION,    1.05f / range);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.30f / (range * range));

    glEnable(GL_FOG);
    glFogi(GL_FOG_MODE, GL_EXP2);
    glFogf(GL_FOG_DENSITY, flashlightActive ? 0.06f : 0.22f);
    GLfloat fogCol[] = {0.02f, 0.02f, 0.02f, 1.0f};
    glFogfv(GL_FOG_COLOR, fogCol);
}
