#include "game.h"

// ============================================================
//  RENDER — MENU SCREEN
// ============================================================
void drawMenu() {
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

    float t = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;

    // Background
    glColor3f(0.03f, 0.03f, 0.06f);
    drawFilledRect(0, 0, (float)WIN_W, (float)WIN_H);

    // Decorative top/bottom lines
    glColor4f(0.3f, 0.25f, 0.1f, 0.6f);
    drawFilledRect(0, (float)WIN_H - 4.0f, (float)WIN_W, 4.0f);
    drawFilledRect(0, 0.0f, (float)WIN_W, 4.0f);

    // Title — "MEMORY MAZE"
    {
        const char* title = "MEMORY MAZE";
        void* titleFont = GLUT_BITMAP_TIMES_ROMAN_24;
        int titleW = textWidth(titleFont, title);
        float pulse = 0.85f + 0.15f * sinf(t * 2.0f);

        // Title glow
        glColor4f(0.8f, 0.6f, 0.1f, 0.15f * pulse);
        drawFilledRect(WIN_W / 2.0f - titleW / 2.0f - 20.0f, WIN_H - 240.0f,
                       (float)(titleW + 40), 50.0f);

        // Title text
        glColor3f(1.0f, 0.85f, 0.3f);
        drawText((int)(WIN_W / 2 - titleW / 2), WIN_H - 215, title, titleFont);

        // Subtitle
        const char* sub = "Hafalkan Peta - Cari Kunci - Keluar dari Labirin";
        void* subFont = GLUT_BITMAP_HELVETICA_12;
        int subW = textWidth(subFont, sub);
        glColor3f(0.5f, 0.45f, 0.35f);
        drawText((int)(WIN_W / 2 - subW / 2), WIN_H - 255, sub, subFont);
    }

    // Menu items
    const char* items[] = {"MULAI", "INFO", "KELUAR"};
    int itemCount = 3;
    float itemY = WIN_H - 340.0f;
    float itemSpacing = 55.0f;
    void* menuFont = GLUT_BITMAP_HELVETICA_18;

    for (int i = 0; i < itemCount; i++) {
        int tw = textWidth(menuFont, items[i]);
        float boxW = 220.0f;
        float boxH = 40.0f;
        float boxX = WIN_W / 2.0f - boxW / 2.0f;
        float boxY = itemY - i * itemSpacing;
        bool selected = (i == menuSelection);

        if (selected) {
            // Highlight background
            float glow = 0.5f + 0.2f * sinf(t * 3.0f);
            glColor4f(0.8f, 0.65f, 0.1f, 0.25f * glow);
            drawFilledRect(boxX, boxY, boxW, boxH);

            // Border
            glColor3f(1.0f, 0.85f, 0.3f);
            glLineWidth(2.0f);
            glBegin(GL_LINE_LOOP);
                glVertex2f(boxX, boxY);
                glVertex2f(boxX + boxW, boxY);
                glVertex2f(boxX + boxW, boxY + boxH);
                glVertex2f(boxX, boxY + boxH);
            glEnd();
            glLineWidth(1.0f);

            // Arrow indicator
            glColor3f(1.0f, 0.85f, 0.3f);
            drawText((int)(boxX - 20), (int)(boxY + (boxH - 18) / 2 + 2), ">", menuFont);

            // Text
            glColor3f(1.0f, 0.95f, 0.7f);
            drawText((int)(boxX + (boxW - tw) / 2), (int)(boxY + (boxH - 18) / 2 + 2),
                     items[i], menuFont);
        } else {
            // Dim background
            glColor4f(0.1f, 0.1f, 0.12f, 0.5f);
            drawFilledRect(boxX, boxY, boxW, boxH);

            // Dim border
            glColor3f(0.25f, 0.25f, 0.3f);
            glLineWidth(1.0f);
            glBegin(GL_LINE_LOOP);
                glVertex2f(boxX, boxY);
                glVertex2f(boxX + boxW, boxY);
                glVertex2f(boxX + boxW, boxY + boxH);
                glVertex2f(boxX, boxY + boxH);
            glEnd();

            // Dim text
            glColor3f(0.5f, 0.5f, 0.5f);
            drawText((int)(boxX + (boxW - tw) / 2), (int)(boxY + (boxH - 18) / 2 + 2),
                     items[i], menuFont);
        }
    }

    // Controls hint at bottom
    {
        const char* hint = "Arrow UP/DOWN untuk navigasi   |   ENTER untuk pilih";
        void* hintFont = GLUT_BITMAP_HELVETICA_12;
        int hintW = textWidth(hintFont, hint);
        glColor3f(0.35f, 0.35f, 0.3f);
        drawText((int)(WIN_W / 2 - hintW / 2), 30, hint, hintFont);
    }

    // Version
    {
        const char* ver = "v1.0";
        void* verFont = GLUT_BITMAP_HELVETICA_12;
        glColor3f(0.2f, 0.2f, 0.2f);
        drawText(WIN_W - 50, 10, ver, verFont);
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

// ============================================================
//  RENDER — MENU INFO OVERLAY
// ============================================================
void drawMenuInfo() {
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

    // Semi-transparent overlay
    glColor4f(0.0f, 0.0f, 0.0f, 0.8f);
    drawFilledRect(0, 0, (float)WIN_W, (float)WIN_H);

    // Info panel
    float panelW = 580.0f;
    float panelH = 440.0f;
    float panelX = WIN_W / 2.0f - panelW / 2.0f;
    float panelY = WIN_H / 2.0f - panelH / 2.0f;

    glColor4f(0.06f, 0.06f, 0.1f, 0.95f);
    drawFilledRect(panelX, panelY, panelW, panelH);

    glColor3f(0.4f, 0.35f, 0.15f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(panelX, panelY);
        glVertex2f(panelX + panelW, panelY);
        glVertex2f(panelX + panelW, panelY + panelH);
        glVertex2f(panelX, panelY + panelH);
    glEnd();
    glLineWidth(1.0f);

    float lx = panelX + 30.0f;
    float ly = panelY + panelH - 40.0f;
    void* titleFont = GLUT_BITMAP_HELVETICA_18;
    void* bodyFont = GLUT_BITMAP_HELVETICA_12;
    float lineSpacing = 22.0f;

    // Title
    glColor3f(1.0f, 0.85f, 0.3f);
    drawText((int)lx, (int)ly, "CARA BERMAIN", titleFont);
    ly -= lineSpacing + 8.0f;

    // Separator
    glColor3f(0.25f, 0.25f, 0.2f);
    drawFilledRect(lx, ly, panelW - 60.0f, 1.0f);
    ly -= lineSpacing;

    // Description
    glColor3f(0.8f, 0.8f, 0.75f);
    drawText((int)lx, (int)ly, "Kamu terjebak dalam labirin gelap!", bodyFont);
    ly -= lineSpacing;
    drawText((int)lx, (int)ly, "Hafalkan peta selama fase memorisasi,", bodyFont);
    ly -= lineSpacing;
    drawText((int)lx, (int)ly, "lalu cari 3 kunci berwarna untuk mendapatkan kode exit.", bodyFont);
    ly -= lineSpacing;
    drawText((int)lx, (int)ly, "Masukkan kode di pintu keluar untuk menang!", bodyFont);
    ly -= lineSpacing + 10.0f;

    // Controls header
    glColor3f(1.0f, 0.85f, 0.3f);
    drawText((int)lx, (int)ly, "KONTROL", titleFont);
    ly -= lineSpacing + 4.0f;

    glColor3f(0.25f, 0.25f, 0.2f);
    drawFilledRect(lx, ly, panelW - 60.0f, 1.0f);
    ly -= lineSpacing;

    // Controls list
    struct ControlEntry { const char* key; const char* desc; };
    ControlEntry controls[] = {
        {"W A S D",      "Gerak maju / kiri / mundur / kanan"},
        {"MOUSE",        "Kamera / Lihat sekeliling"},
        {"F",            "Nyalakan senter (terbatas)"},
        {"M",            "Buka peta singkat (terbatas)"},
        {"E",            "Ambil kunci"},
        {"ENTER",        "Input kode di pintu exit"},
        {"ESC",          "Keluar dari game"}
    };
    int controlCount = 7;
    void* keyFont = GLUT_BITMAP_8_BY_13;

    for (int i = 0; i < controlCount; i++) {
        glColor3f(1.0f, 0.85f, 0.3f);
        drawText((int)(lx + 10), (int)ly, controls[i].key, keyFont);
        glColor3f(0.7f, 0.7f, 0.65f);
        drawText((int)(lx + 120), (int)ly, controls[i].desc, bodyFont);
        ly -= lineSpacing;
    }

    ly -= 10.0f;

    // Items header
    glColor3f(1.0f, 0.85f, 0.3f);
    drawText((int)lx, (int)ly, "ITEM", titleFont);
    ly -= lineSpacing + 4.0f;

    glColor3f(0.25f, 0.25f, 0.2f);
    drawFilledRect(lx, ly, panelW - 60.0f, 1.0f);
    ly -= lineSpacing;

    glColor3f(0.95f, 0.85f, 0.4f);
    drawText((int)(lx + 10), (int)ly, "Senter", keyFont);
    glColor3f(0.7f, 0.7f, 0.65f);
    drawText((int)(lx + 120), (int)ly, "Menerangi sekitar (3x, 5 detik)", bodyFont);
    ly -= lineSpacing;

    glColor3f(0.5f, 0.7f, 1.0f);
    drawText((int)(lx + 10), (int)ly, "Peta", keyFont);
    glColor3f(0.7f, 0.7f, 0.65f);
    drawText((int)(lx + 120), (int)ly, "Membuka peta singkat (3x, 4 detik)", bodyFont);
    ly -= lineSpacing;

    // Close hint
    ly -= 15.0f;
    const char* closeHint = "Tekan ESC atau ENTER untuk kembali";
    int hintW = textWidth(bodyFont, closeHint);
    glColor3f(0.4f, 0.4f, 0.35f);
    drawText((int)(panelX + panelW / 2 - hintW / 2), (int)ly, closeHint, bodyFont);

    glDisable(GL_BLEND);
    glEnable(GL_FOG);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}
