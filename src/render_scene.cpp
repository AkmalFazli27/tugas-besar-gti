#include "game.h"

// ============================================================
//  GLUT — DISPLAY (scene orchestration)
// ============================================================
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (gameState == STATE_MENU) {
        drawMenu();
        if (showInfo) drawMenuInfo();
        glutSwapBuffers();
        return;
    }

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
        gluLookAt(cam.x, cam.y + headBobOffset, cam.z,
                  lx,    ly + headBobOffset,    lz,
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
        drawSpawnOverlay();
        drawCodeSpots(true);

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LIGHTING);
    }

    if (viewMode == 1) {
        glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH_TEST);

        // Player triangle (like Google Maps marker)
        float py = W_HEIGHT + 0.07f;
        float rad = toRad(cam.angle);
        float size = 0.45f;

        // Triangle vertices: tip (forward), back-left, back-right
        float tipX = cam.x + sinf(rad) * size;
        float tipZ = cam.z + cosf(rad) * size;

        float backX = cam.x - sinf(rad) * size * 0.6f;
        float backZ = cam.z - cosf(rad) * size * 0.6f;

        float perpX = cosf(rad) * size * 0.45f;
        float perpZ = -sinf(rad) * size * 0.45f;

        // Filled triangle (yellow)
        glColor3f(1.0f, 0.85f, 0.0f);
        glBegin(GL_TRIANGLES);
            glVertex3f(tipX, py, tipZ);
            glVertex3f(backX + perpX, py, backZ + perpZ);
            glVertex3f(backX - perpX, py, backZ - perpZ);
        glEnd();

        // Outline (darker)
        glColor3f(0.6f, 0.5f, 0.0f);
        glLineWidth(2.0f);
        glBegin(GL_LINE_LOOP);
            glVertex3f(tipX, py, tipZ);
            glVertex3f(backX + perpX, py, backZ + perpZ);
            glVertex3f(backX - perpX, py, backZ - perpZ);
        glEnd();
        glLineWidth(1.0f);

        // ==============================
        //  LEGEND — bottom right
        // ==============================
        glMatrixMode(GL_PROJECTION);
        glPushMatrix(); glLoadIdentity();
        gluOrtho2D(0, WIN_W, 0, WIN_H);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix(); glLoadIdentity();
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        float legW = 165.0f;
        float legH = 170.0f;
        float legX = WIN_W - legW - 14.0f;
        float legY = 14.0f;
        float ly = legY + legH - 20.0f;

        // Panel background
        glColor4f(0.04f, 0.04f, 0.08f, 0.80f);
        drawFilledRect(legX, legY, legW, legH);

        // Panel border
        glColor3f(0.3f, 0.3f, 0.25f);
        glLineWidth(1.0f);
        glBegin(GL_LINE_LOOP);
            glVertex2f(legX, legY);
            glVertex2f(legX + legW, legY);
            glVertex2f(legX + legW, legY + legH);
            glVertex2f(legX, legY + legH);
        glEnd();

        // Title
        glColor3f(1.0f, 0.85f, 0.3f);
        drawText((int)(legX + 10), (int)ly, "LEGENDA", GLUT_BITMAP_HELVETICA_12);
        ly -= 8.0f;

        // Separator
        glColor3f(0.2f, 0.2f, 0.18f);
        drawFilledRect(legX + 8.0f, ly, legW - 16.0f, 1.0f);
        ly -= 22.0f;

        // Key — red
        glColor3f(1.0f, 0.25f, 0.25f);
        drawFilledCircle(legX + 16, ly + 4, 5, 10);
        glColor3f(0.8f, 0.8f, 0.75f);
        drawText((int)(legX + 28), (int)(ly - 2), "Kunci", GLUT_BITMAP_HELVETICA_12);
        ly -= 22.0f;

        // Key — blue
        glColor3f(0.25f, 0.55f, 1.0f);
        drawFilledCircle(legX + 16, ly + 4, 5, 10);
        glColor3f(0.8f, 0.8f, 0.75f);
        drawText((int)(legX + 28), (int)(ly - 2), "Kunci", GLUT_BITMAP_HELVETICA_12);
        ly -= 22.0f;

        // Key — yellow
        glColor3f(1.0f, 0.9f, 0.25f);
        drawFilledCircle(legX + 16, ly + 4, 5, 10);
        glColor3f(0.8f, 0.8f, 0.75f);
        drawText((int)(legX + 28), (int)(ly - 2), "Kunci", GLUT_BITMAP_HELVETICA_12);
        ly -= 22.0f;

        // Spawn
        glColor3f(0.3f, 0.6f, 1.0f);
        drawFilledRect(legX + 11, ly - 1, 10, 10);
        glColor3f(0.8f, 0.8f, 0.75f);
        drawText((int)(legX + 28), (int)(ly - 2), "Spawn (S)", GLUT_BITMAP_HELVETICA_12);
        ly -= 22.0f;

        // Exit
        glColor3f(0.0f, 1.0f, 0.4f);
        drawFilledRect(legX + 11, ly - 1, 10, 10);
        glColor3f(0.8f, 0.8f, 0.75f);
        drawText((int)(legX + 28), (int)(ly - 2), "Exit (E)", GLUT_BITMAP_HELVETICA_12);

        glDisable(GL_BLEND);
        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LIGHTING);
    }

    drawHUD();

    glutSwapBuffers();
}
