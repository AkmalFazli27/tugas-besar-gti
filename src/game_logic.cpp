#include "game.h"

void computeReachableFromExit() {
    for (int r = 0; r < ROWS; r++)
        for (int c = 0; c < COLS; c++)
            reachableFromExit[r][c] = false;

    if (!isCellWalkable(EXIT_ROW, EXIT_COL)) return;

    std::queue<std::pair<int, int>> q;
    q.push({EXIT_ROW, EXIT_COL});
    reachableFromExit[EXIT_ROW][EXIT_COL] = true;

    const int dr[4] = {-1, 1, 0, 0};
    const int dc[4] = {0, 0, -1, 1};

    while (!q.empty()) {
        auto cur = q.front();
        q.pop();
        for (int i = 0; i < 4; i++) {
            int nr = cur.first + dr[i];
            int nc = cur.second + dc[i];
            if (!isCellWalkable(nr, nc)) continue;
            if (reachableFromExit[nr][nc]) continue;
            reachableFromExit[nr][nc] = true;
            q.push({nr, nc});
        }
    }
}

void placeRandomSpawn() {
    int candidates[ROWS * COLS][2];
    int count = 0;
    for (int r = 1; r < ROWS - 1; r++)
        for (int c = 1; c < COLS - 1; c++)
            if (reachableFromExit[r][c] && !(r == EXIT_ROW && c == EXIT_COL)) {
                candidates[count][0] = r;
                candidates[count][1] = c;
                count++;
            }

    int r = 1, c = 1;
    if (count > 0) {
        int pick = randRange(0, count - 1);
        r = candidates[pick][0];
        c = candidates[pick][1];
    }
    spawnRow = r;
    spawnCol = c;
    cam.x = (spawnCol + 0.5f) * CELL;
    cam.z = (spawnRow + 0.5f) * CELL;
    cam.angle = 0.0f;
}

bool isCellUsedForCode(int r, int c, bool used[ROWS][COLS]) {
    if (!isCellWalkable(r, c)) return true;
    if (r == EXIT_ROW && c == EXIT_COL) return true;
    if (r == spawnRow && c == spawnCol) return true;
    if (used[r][c]) return true;
    return false;
}

void placeCodes() {
    bool used[ROWS][COLS] = {};
    used[spawnRow][spawnCol] = true;
    used[EXIT_ROW][EXIT_COL] = true;

    for (int i = 0; i < CODE_COUNT; i++) {
        codeDigits[i] = randRange(0, 9);
        codeCollected[i] = false;
    }

    int placedKeys[CODE_COUNT][2];
    int placedCount = 0;

    for (int i = 0; i < CODE_COUNT; i++) {
        int candidates[ROWS * COLS][2];
        int count = 0;

        for (int r = 1; r < ROWS - 1; r++) {
            for (int c = 1; c < COLS - 1; c++) {
                if (!reachableFromExit[r][c] || isCellUsedForCode(r, c, used))
                    continue;

                int distSpawn = abs(r - spawnRow) + abs(c - spawnCol);
                if (distSpawn < 5) continue;

                int distExit = abs(r - EXIT_ROW) + abs(c - EXIT_COL);
                if (distExit < 5) continue;

                bool tooClose = false;
                for (int j = 0; j < placedCount; j++) {
                    int distKey = abs(r - placedKeys[j][0]) + abs(c - placedKeys[j][1]);
                    if (distKey < 5) {
                        tooClose = true;
                        break;
                    }
                }
                if (tooClose) continue;

                candidates[count][0] = r;
                candidates[count][1] = c;
                count++;
            }
        }

        if (count == 0) {
            for (int r = 1; r < ROWS - 1; r++) {
                for (int c = 1; c < COLS - 1; c++) {
                    if (reachableFromExit[r][c] && !isCellUsedForCode(r, c, used)) {
                        candidates[count][0] = r;
                        candidates[count][1] = c;
                        count++;
                    }
                }
            }
        }

        int r = 1, c = 1;
        if (count > 0) {
            int pick = randRange(0, count - 1);
            r = candidates[pick][0];
            c = candidates[pick][1];
        }

        used[r][c] = true;
        placedKeys[placedCount][0] = r;
        placedKeys[placedCount][1] = c;
        placedCount++;

        codeSpots[i].row = r;
        codeSpots[i].col = c;
        codeSpots[i].digit = codeDigits[i];
        codeSpots[i].color = (CodeColor)i;
        codeSpots[i].collected = false;
    }
}

// ============================================================
//  GENERATE — pick random template with random spawn/exit
// ============================================================
void generateMaze() {
    // 1. Pick random template
    int pick = randRange(0, 10);
    
    // 2. Copy template to maze
    for (int r = 0; r < ROWS; r++)
        for (int c = 0; c < COLS; c++)
            maze[r][c] = TEMPLATES[pick][r][c];
    
    // 3. Collect all walkable cells
    int walkable[ROWS * COLS][2];
    int count = 0;
    for (int r = 1; r < ROWS - 1; r++)
        for (int c = 1; c < COLS - 1; c++)
            if (maze[r][c] == 0) {
                walkable[count][0] = r;
                walkable[count][1] = c;
                count++;
            }
    
    // 4. Pick random spawn
    int si = randRange(0, count - 1);
    spawnRow = walkable[si][0];
    spawnCol = walkable[si][1];
    
    // 5. Pick exit far from spawn (Manhattan distance >= 10)
    int ei;
    int attempts = 0;
    do {
        ei = randRange(0, count - 1);
        int dist = abs(walkable[ei][0] - spawnRow) + 
                   abs(walkable[ei][1] - spawnCol);
        if (dist >= 10) break;
        attempts++;
    } while (attempts < 100);
    
    EXIT_ROW = walkable[ei][0];
    EXIT_COL = walkable[ei][1];
}

void resetGame() {
    generateMaze();

    computeReachableFromExit();
    placeCodes();
    memorizeTimeLeft = MEMORIZE_DURATION;
    gameTimeLeft = GAME_DURATION;
    flashlightUses = FLASHLIGHT_USES;
    flashlightActive = false;
    flashlightTimeLeft = 0.0f;
    mapRevealUses = MAP_REVEAL_USES;
    mapRevealActive = false;
    mapRevealTimeLeft = 0.0f;
    resetInputCodes();
    viewMode = 1;
    gameState = STATE_MEMORIZE;
    setStatus("Hafalkan peta dan lokasi kode", 2.5f);
    
    // Set camera position from spawn
    cam.x = (spawnCol + 0.5f) * CELL;
    cam.z = (spawnRow + 0.5f) * CELL;
    cam.angle = 0.0f;
}
