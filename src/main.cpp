// ============================================================
//  MEMORY MAZE — Entry Point
//  Tahap 2: memorisasi peta, gelap, flashlight, kode warna, timer
//
//  # Compile
//  C:\msys64\mingw64\bin\g++.exe -std=c++17 -g src\game_state.cpp src\game_logic.cpp src\render_helpers.cpp src\render_maze.cpp src\render_hud.cpp src\render_menu.cpp src\render_scene.cpp src\input.cpp src\update.cpp src\main.cpp -o memory_maze_game.exe -lfreeglut -lopengl32 -lglu32
//
//  # Run (tambahkan PATH runtime freeglut)
//  $env:Path = "C:\msys64\mingw64\bin;$env:Path"
//  .\memory_maze_game.exe
// ============================================================

#include "game.h"

// ============================================================
//  INISIALISASI OPENGL
// ============================================================
void initGL() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glClearColor(0.02f, 0.02f, 0.03f, 1.0f);

    initTextures();

    // Cursor visible for menu; hidden during gameplay
    glutSetCursor(GLUT_CURSOR_INHERIT);
    mouseWarping = false;
    currentWinW = WIN_W;
    currentWinH = WIN_H;

    GLfloat matSpec[] = {0.2f, 0.18f, 0.12f, 1.0f};
    glMaterialfv(GL_FRONT, GL_SPECULAR,  matSpec);
    glMaterialf(GL_FRONT,  GL_SHININESS, 16.0f);
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

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyDown);
    glutKeyboardUpFunc(keyUp);
    glutSpecialFunc(specialKey);
    glutPassiveMotionFunc(mouseMove);
    glutTimerFunc(16, update, 0);

    glutMainLoop();
    return 0;
}
