#include "game.h"

// ============================================================
//  GLUT — UPDATE (Timer ~60fps)
// ============================================================
void update(int v) {
    static float lastT = 0;
    float t  = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
    float dt = t - lastT;
    lastT    = t;
    if (dt > 0.05f) dt = 0.05f;

    if (gameState == STATE_MENU) {
        glutPostRedisplay();
        glutTimerFunc(16, update, 0);
        return;
    }

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

    if (gameState == STATE_PLAY && !mapRevealActive) {
        float rad = toRad(cam.angle);
        float dx  = 0, dz = 0;
        float spd = cam.speed * dt;

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

        float nx = cam.x + dx;
        float nz = cam.z + dz;

        auto canMoveTo = [](float x, float z) {
            return isWalkable(x + COLLISION_MARGIN, z + COLLISION_MARGIN) &&
                   isWalkable(x - COLLISION_MARGIN, z + COLLISION_MARGIN) &&
                   isWalkable(x + COLLISION_MARGIN, z - COLLISION_MARGIN) &&
                   isWalkable(x - COLLISION_MARGIN, z - COLLISION_MARGIN);
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
//  GLUT — RESHAPE
// ============================================================
void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    currentWinW = w;
    currentWinH = h;
    mouseWarping = true;
    glutWarpPointer(currentWinW / 2, currentWinH / 2);
}
