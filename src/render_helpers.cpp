#include "game.h"

// ============================================================
//  RENDER — HELPER FUNCTIONS (shared by HUD, menu, overlays)
// ============================================================

void drawFilledRect(float x, float y, float w, float h) {
    glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x + w, y);
        glVertex2f(x + w, y + h);
        glVertex2f(x, y + h);
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
    glBegin(GL_TRIANGLE_STRIP);
        for (int i = 0; i <= segments; i++) {
            float angle = 2.0f * 3.14159265f * i / segments;
            glVertex2f(cx + cosf(angle) * (r + thickness/2), cy + sinf(angle) * (r + thickness/2));
            glVertex2f(cx + cosf(angle) * (r - thickness/2), cy + sinf(angle) * (r - thickness/2));
        }
    glEnd();
}

int textWidth(void* font, const char* s) {
    int width = 0;
    const unsigned char* p = (const unsigned char*)s;
    while (*p) {
        width += glutBitmapWidth(font, *p);
        ++p;
    }
    return width;
}

int textWidth18(const char* s) {
    return textWidth(GLUT_BITMAP_HELVETICA_18, s);
}

int textWidth12(const char* s) {
    return textWidth(GLUT_BITMAP_HELVETICA_12, s);
}

void drawText(int x, int y, const char* text, void* font) {
    glRasterPos2i(x, y);
    for (const char* c = text; *c; c++)
        glutBitmapCharacter(font, *c);
}
