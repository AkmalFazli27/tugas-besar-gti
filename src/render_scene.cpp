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
