#include "game.h"

// ============================================================
//  GLUT — KEYBOARD
// ============================================================
void keyDown(unsigned char k, int x, int y) {
    keys[k] = true;

    // ── MENU STATE ──
    if (gameState == STATE_MENU) {
        if (k == 27) {  // ESC
            if (showInfo) {
                showInfo = false;
            } else {
                exit(0);
            }
        }
        if (k == 13) {  // ENTER
            if (showInfo) {
                showInfo = false;
            } else if (menuSelection == 0) {
                // START
                resetGame();
                glutSetCursor(GLUT_CURSOR_NONE);
                mouseWarping = true;
                glutWarpPointer(currentWinW / 2, currentWinH / 2);
            } else if (menuSelection == 1) {
                // INFO
                showInfo = true;
            } else if (menuSelection == 2) {
                // EXIT
                exit(0);
            }
        }
        return;
    }

    // ── WIN / LOSE — back to menu ──
    if (gameState == STATE_WIN || gameState == STATE_LOSE) {
        if (k == 13) {  // ENTER
            gameState = STATE_MENU;
            showInfo = false;
            glutSetCursor(GLUT_CURSOR_INHERIT);
            return;
        }
        if (k == 27) exit(0);
        return;
    }

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
//  GLUT — SPECIAL KEYS (Arrow keys for menu)
// ============================================================
void specialKey(int k, int x, int y) {
    if (gameState == STATE_MENU && !showInfo) {
        if (k == GLUT_KEY_UP) {
            menuSelection = (menuSelection - 1 + 3) % 3;
        } else if (k == GLUT_KEY_DOWN) {
            menuSelection = (menuSelection + 1) % 3;
        }
    }
}

// ============================================================
//  GLUT — MOUSE LOOK
// ============================================================
void mouseMove(int x, int y) {
    if (gameState != STATE_PLAY) return;
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
