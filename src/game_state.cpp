#include "game.h"

// ============================================================
//  KONFIGURASI
// ============================================================
const int   WIN_W       = 1280;
const int   WIN_H       = 720;
const char* WIN_NAME    = "Memory Maze — Game";
const int   EXIT_ROW    = 13;
const int   EXIT_COL    = 13;
const float FIRST_PERSON_FOV   = 54.0f;
const float FIRST_PERSON_FAR   = 38.0f;
const float MEMORIZE_DURATION  = 7.0f;
const float GAME_DURATION      = 180.0f;
const int   FLASHLIGHT_USES    = 3;
const float FLASHLIGHT_DURATION= 5.0f;
const int   MAP_REVEAL_USES    = 3;
const float MAP_REVEAL_DURATION= 4.0f;

// ============================================================
//  GLOBAL VARIABLES (mutable state)
// ============================================================
GLuint texWall = 0;
GLuint texFloor = 0;

int maze[ROWS][COLS] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,1,0,0,0,1,0,0,0,1,0,1},
    {1,0,1,0,1,0,1,0,1,0,1,0,1,0,1},
    {1,0,1,0,0,0,1,0,0,0,1,0,0,0,1},
    {1,0,1,1,1,0,1,1,1,0,1,1,1,0,1},
    {1,0,0,0,1,0,0,0,1,0,0,0,1,0,1},
    {1,1,1,0,1,1,1,0,1,1,1,0,1,0,1},
    {1,0,0,0,0,0,1,0,0,0,0,0,1,0,1},
    {1,0,1,1,1,0,1,1,1,1,1,0,1,0,1},
    {1,0,1,0,0,0,0,0,1,0,0,0,0,0,1},
    {1,0,1,0,1,1,1,0,1,0,1,1,1,0,1},
    {1,0,0,0,1,0,0,0,1,0,0,0,1,0,1},
    {1,1,1,0,1,0,1,1,1,0,1,0,1,0,1},
    {1,0,0,0,0,0,1,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

int spawnRow = 1, spawnCol = 1;

Camera cam;
bool keys[256] = {};
GameState gameState = STATE_MEMORIZE;

CodeSpot codeSpots[CODE_COUNT];
int codeDigits[CODE_COUNT] = {0};
bool codeCollected[CODE_COUNT] = {false};

const char* COLOR_NAMES[CODE_COUNT] = {"MERAH", "BIRU", "KUNING"};
const float COLOR_RGB[CODE_COUNT][3] = {
    {1.0f, 0.25f, 0.25f},
    {0.25f, 0.55f, 1.0f},
    {1.0f, 0.9f, 0.25f}
};

const CodeColor INPUT_ORDER[CODE_COUNT] = {CODE_RED, CODE_BLUE, CODE_YELLOW};

float memorizeTimeLeft = MEMORIZE_DURATION;
float gameTimeLeft = GAME_DURATION;
int   flashlightUses = FLASHLIGHT_USES;
bool  flashlightActive = false;
float flashlightTimeLeft = 0.0f;
int   mapRevealUses = MAP_REVEAL_USES;
bool  mapRevealActive = false;
float mapRevealTimeLeft = 0.0f;
bool  reachableFromExit[ROWS][COLS] = {};
char  statusMsg[128] = "";
float statusMsgTimer = 0.0f;
int   enteredCodes[CODE_COUNT] = {-1, -1, -1};
int   inputIndex = 0;
int   viewMode = 0;

// ============================================================
//  UTILITY
// ============================================================
void buildChecker(unsigned char* data, int size, int checkSize,
                  const unsigned char c1[3], const unsigned char c2[3]) {
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            int idx = (y * size + x) * 3;
            int checker = ((x / checkSize) + (y / checkSize)) % 2;
            const unsigned char* c = checker ? c1 : c2;
            data[idx + 0] = c[0];
            data[idx + 1] = c[1];
            data[idx + 2] = c[2];
        }
    }
}

void initTextures() {
    const int size = 64;
    static unsigned char wallData[size * size * 3];
    static unsigned char floorData[size * size * 3];

    const unsigned char wallA[3]  = {90, 70, 55};
    const unsigned char wallB[3]  = {60, 45, 35};
    const unsigned char floorA[3] = {55, 45, 38};
    const unsigned char floorB[3] = {30, 25, 20};

    buildChecker(wallData, size, 8, wallA, wallB);
    buildChecker(floorData, size, 6, floorA, floorB);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glGenTextures(1, &texWall);
    glBindTexture(GL_TEXTURE_2D, texWall);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size, size, 0, GL_RGB, GL_UNSIGNED_BYTE, wallData);

    glGenTextures(1, &texFloor);
    glBindTexture(GL_TEXTURE_2D, texFloor);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size, size, 0, GL_RGB, GL_UNSIGNED_BYTE, floorData);
}

float toRad(float deg) { return deg * 3.14159265f / 180.0f; }

int randRange(int minVal, int maxVal) {
    return minVal + (rand() % (maxVal - minVal + 1));
}

int playerRow() { return (int)(cam.z / CELL); }
int playerCol() { return (int)(cam.x / CELL); }

bool isWalkable(float wx, float wz) {
    int r = (int)(wz / CELL);
    int c = (int)(wx / CELL);
    if (r < 0 || r >= ROWS || c < 0 || c >= COLS) return false;
    return maze[r][c] == 0;
}

bool isCellWalkable(int r, int c) {
    if (r < 0 || r >= ROWS || c < 0 || c >= COLS) return false;
    return maze[r][c] == 0;
}

bool isPlayerAt(int r, int c) {
    return playerRow() == r && playerCol() == c;
}

bool allCodesCollected() {
    for (int i = 0; i < CODE_COUNT; i++)
        if (!codeCollected[i]) return false;
    return true;
}

void setStatus(const char* msg, float duration) {
    std::strncpy(statusMsg, msg, sizeof(statusMsg) - 1);
    statusMsg[sizeof(statusMsg) - 1] = '\0';
    statusMsgTimer = duration;
}

void resetInputCodes() {
    for (int i = 0; i < CODE_COUNT; i++) enteredCodes[i] = -1;
    inputIndex = 0;
}

int codeIndexAtPlayer() {
    int pr = playerRow();
    int pc = playerCol();
    for (int i = 0; i < CODE_COUNT; i++) {
        if (!codeSpots[i].collected && codeSpots[i].row == pr && codeSpots[i].col == pc)
            return i;
    }
    return -1;
}
