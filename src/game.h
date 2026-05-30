#ifndef GAME_H
#define GAME_H

#include <GL/glut.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <queue>

// ============================================================
//  KONFIGURASI WINDOW
// ============================================================
extern const int   WIN_W;
extern const int   WIN_H;
extern const char* WIN_NAME;

// ============================================================
//  KONFIGURASI MAZE (scalar compile-time constants defined here)
// ============================================================
const int   ROWS      = 15;
const int   COLS      = 15;
const float CELL      = 2.0f;
const float W_HEIGHT  = 3.0f;
const float COLLISION_MARGIN = 0.4f;

extern GLuint texWall;
extern GLuint texFloor;
extern int maze[ROWS][COLS];
extern const int TEMPLATES[11][ROWS][COLS];
extern int spawnRow;
extern int spawnCol;
extern int EXIT_ROW;
extern int EXIT_COL;

// ============================================================
//  KONFIGURASI GAME
// ============================================================
extern const float FIRST_PERSON_FOV;
extern const float FIRST_PERSON_FAR;
extern const float MEMORIZE_DURATION;
extern const float GAME_DURATION;
extern const int   FLASHLIGHT_USES;
extern const float FLASHLIGHT_DURATION;
extern const int   MAP_REVEAL_USES;
extern const float MAP_REVEAL_DURATION;

// ============================================================
//  KAMERA FIRST-PERSON
// ============================================================
struct Camera {
    float x     = 0.0f;
    float z     = 0.0f;
    float y     = 1.3f;
    float angle = 0.0f;
    float pitch = 0.0f;
    float speed = 2.2f;
    float tSpeed= 80.0f;
};

extern Camera cam;
extern bool keys[256];

// ============================================================
//  STATE GAME
// ============================================================
enum GameState {
    STATE_MENU,
    STATE_MEMORIZE,
    STATE_PLAY,
    STATE_ENTER_CODE,
    STATE_WIN,
    STATE_LOSE
};

extern GameState gameState;

// ============================================================
//  MOUSE LOOK
// ============================================================
extern float mouseSensitivity;
extern bool  mouseWarping;
extern bool  invertY;
extern int   currentWinW;
extern int   currentWinH;

// ============================================================
//  KODE WARNA
// ============================================================
enum CodeColor {
    CODE_RED = 0,
    CODE_BLUE = 1,
    CODE_YELLOW = 2,
    CODE_COUNT = 3
};

struct CodeSpot {
    int row = 0;
    int col = 0;
    int digit = 0;
    CodeColor color = CODE_RED;
    bool collected = false;
};

extern CodeSpot codeSpots[CODE_COUNT];
extern int codeDigits[CODE_COUNT];
extern bool codeCollected[CODE_COUNT];
extern const char* COLOR_NAMES[CODE_COUNT];
extern const float COLOR_RGB[CODE_COUNT][3];
extern const CodeColor INPUT_ORDER[CODE_COUNT];

// ============================================================
//  TIMER & STATUS
// ============================================================
extern float memorizeTimeLeft;
extern float gameTimeLeft;
extern int   flashlightUses;
extern bool  flashlightActive;
extern float flashlightTimeLeft;
extern int   mapRevealUses;
extern bool  mapRevealActive;
extern float mapRevealTimeLeft;
extern bool  reachableFromExit[15][15];
extern char  statusMsg[128];
extern float statusMsgTimer;
extern int   enteredCodes[CODE_COUNT];
extern int   inputIndex;

extern int viewMode;

// ============================================================
//  MENU STATE
// ============================================================
extern int  menuSelection;
extern bool showInfo;

// ============================================================
//  FUNCTION DECLARATIONS
// ============================================================

// Utility
void buildChecker(unsigned char* data, int size, int checkSize,
                  const unsigned char c1[3], const unsigned char c2[3]);
void initTextures();
float toRad(float deg);
int randRange(int minVal, int maxVal);
int playerRow();
int playerCol();
bool isWalkable(float wx, float wz);
bool isCellWalkable(int r, int c);
bool isPlayerAt(int r, int c);
bool allCodesCollected();
void setStatus(const char* msg, float duration);
void resetInputCodes();
int codeIndexAtPlayer();

// Game logic
void computeReachableFromExit();
void placeRandomSpawn();
bool isCellUsedForCode(int r, int c, bool used[ROWS][COLS]);
void placeCodes();
void generateMaze();
void resetGame();

// Render — maze
void drawWall(int col, int row);
void drawFloor();
void drawCeiling();
void drawExit(bool overlay = false);
void drawSpawn();
void drawSpawnOverlay();
void drawKeyOrb(int colorIndex, float x, float z);
void setupKeyLights();
void drawCodeSpots(bool overlay = false);
void drawMaze();

// Render — lighting & HUD
void setupLighting();
void drawText(int x, int y, const char* text, void* font);
int  textWidth(void* font, const char* s);
int  textWidth18(const char* s);
int  textWidth12(const char* s);
void drawHUD();
void drawMenu();
void drawMenuInfo();

// Render — helper functions
void drawFilledRect(float x, float y, float w, float h);
void drawFilledCircle(float cx, float cy, float r, int segments);
void drawOutlinedCircle(float cx, float cy, float r, int segments, float thickness);

// Entry
void display();
void update(int v);
void keyDown(unsigned char k, int x, int y);
void keyUp(unsigned char k, int x, int y);
void specialKey(int k, int x, int y);
void mouseMove(int x, int y);
void reshape(int w, int h);
void initGL();

#endif
