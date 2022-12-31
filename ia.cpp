#include "ia.h"
#include "shapes.h"
#include <vector>
#include <stdint.h>

int Ia::m_isearchCount = 0;
bool Ia::m_boolsearching = true;
std::stack<Pos> Ia::m_stackcurrent;
std::stack<Pos> Ia::m_stacksaved;

std::vector<std::pair<int, int>> Ia::queue;
std::pair<int, int> Ia::p;

int
Ia::freezeShape(int argrid[GRID_H][GRID_W],
                int shape,
                int shapeCol,
                int shapeRow,
                int bottomShiftShape,
                int shapeRotation,
                int updatedargrid[GRID_H][GRID_W])
{
    int row = 0;
    int maxRow = -1;

    for (int r = shapeRow; r < GRID_H - bottomShiftShape; r++) {
        bool fit = true;
        for (int y = SHAPE_SIZE - 1; y >= 0; y--) {
            for (int x = 0; x < SHAPE_SIZE; x++) {

                if (tt::gShapesArray[shape][shapeRotation][y * SHAPE_SIZE + x] >
                      0 &&
                    argrid[r + y][shapeCol + x] == SOMETHING_IN_SQUARE) {
                    fit = false;
                }
            }
        }

        if (fit && r > maxRow) {
            maxRow = r;
        }

        if (!fit) {
            break;
        }
    }

    if (maxRow >= 0) {
        const int* s = tt::gShapesArray[shape][shapeRotation];
        int squarecount = 0;
        for (int yy = maxRow; yy < SHAPE_SIZE + maxRow; yy++) {
            for (int xx = shapeCol; xx < SHAPE_SIZE + shapeCol; xx++) {
                if (yy < GRID_H && xx < GRID_W && s[squarecount] == 1) {
                    updatedargrid[yy][xx] = SOMETHING_IN_SQUARE;
                }
                squarecount++;
            }
        }
    }

    return maxRow;
}

inline int
Ia::checkSomethingInVerticalLine(int argrid[GRID_H][GRID_W],
                                 int x,
                                 int y,
                                 int direction)
{
    return false;
}

Pos
Ia::findBestPosition(int argrid[GRID_H][GRID_W],
                     int shapesQueue[SHAPES_QUEUE_SIZE],
                     int currentShape,
                     int currentRotation,
                     int currentBottomShiftShape,
                     int currentLeftShiftShape,
                     int currentRightShiftShape,
                     int row,
                     int col,
                     int currentDepth,
                     int fullDepth)
{


    if (currentDepth == 0 || !m_boolsearching) {
        m_isearchCount++;
        int score = getScore(argrid);
        Pos evaluatedPos = { row, col, currentRotation, currentShape, score };

        // on leaf, compare stack with saved stack - if score is better, replace
        // top stack with the same move adding the evaluated score
        // at the end of the recursive function, m_stacksaved contains the path
        // to the best scored leaf
        if (score > m_stacksaved.top().score) {
            Pos topPos = m_stackcurrent.top();
            topPos.score = score;
            m_stackcurrent.pop();
            m_stackcurrent.push(topPos);
            m_stacksaved = m_stackcurrent;
        }

        return evaluatedPos;
    }

    // find best score at row
    int bestScore = INT_MIN;
    int bestCol = 0;
    int bestRow = 0;
    int bestRot = 0;

    for (int rot = 0; rot < tt::gDepthRotationSearch[currentShape]; rot++) {

        currentBottomShiftShape = findBottomShiftShape(currentShape, rot);
        currentLeftShiftShape = findLeftShiftShape(currentShape, rot);
        currentRightShiftShape = findRightShiftShape(currentShape, rot);

        // find max moves at row,col
        int minLeft = 0, maxRight = 0;
        for (int x = col; x >= -1;
             x--) { // -1 to compensate empty left margin of certain shapes
            minLeft = x;
            if (x <= 0 - currentLeftShiftShape)
                break;
            if (isShapeLeftCollide(
                  argrid, currentShape, currentRotation, row, x))
                break;
        }

        for (int x = col; x < GRID_W; x++) {
            maxRight = x;
            if (x >= GRID_W - currentRightShiftShape - 1)
                break;
            if (isShapeRightCollide(
                  argrid, currentShape, currentRotation, row, x))
                break;
        }

        // printf("minLeft:%d   maxRight:%d   currentLeftShiftShape:%d   "
        //        "currentRightShiftShape:%d\n",
        //        minLeft,
        //        maxRight,
        //        currentLeftShiftShape,
        //        currentRightShiftShape);

        for (int x = minLeft; x <= maxRight; x++) {
            int updatedGrid[GRID_H][GRID_W] = { 0 };
            Ia::copyGrid(argrid, updatedGrid);
            int r = Ia::freezeShape(argrid,
                                    currentShape,
                                    x,
                                    row,
                                    currentBottomShiftShape,
                                    rot,
                                    updatedGrid);

            int nextShape = shapesQueue[fullDepth - currentDepth];

            Pos searchPos = { r, x, rot, currentShape, 0 };
            m_stackcurrent.push(searchPos);
            Pos currentPos = findBestPosition(updatedGrid,
                                              shapesQueue,
                                              nextShape,
                                              currentRotation,
                                              currentBottomShiftShape,
                                              currentLeftShiftShape,
                                              currentRightShiftShape,
                                              row,
                                              col,
                                              currentDepth - 1,
                                              fullDepth);
            m_stackcurrent.pop();

            int currentScore = currentPos.score;

            if (currentScore > bestScore) {
                bestScore = currentScore;
                bestCol = x;
                bestRow = r;
                bestRot = rot;
            }
            // Ia::debugGrid(updatedGrid);
        }
    }

    // not necessary but returns the best pos (same as m_stacksaved top)
    Pos pos = { bestRow, bestCol, bestRot, currentShape, bestScore };
    return pos;
}

inline bool
Ia::isValid(int argrid[GRID_H][GRID_W], int x, int y, int prevC, int newC)
{
    if (x < 0 || x >= GRID_W || y < 0 || y >= GRID_H ||
        argrid[y][x] == SOMETHING_IN_SQUARE || argrid[y][x] == FLOODED_IN_SQUARE)
        return false;
    return true;
}

int
Ia::getScore(int argrid[GRID_H][GRID_W])
{
    int score = 0;
    int lineScore = 0;
    int lineSquare = 0;
    // int lineCount = 0;
    for (int y = 0; y < GRID_H; y++) {
        lineScore = 0;
        lineSquare = 0;
        for (int x = 0; x < GRID_W; x++) {
            if (argrid[y][x] == SOMETHING_IN_SQUARE) {
                lineSquare++;
            }
            if (x < GRID_W - 1 && argrid[y][x] == SOMETHING_IN_SQUARE &&
                argrid[y][x + 1] == SOMETHING_IN_SQUARE) {
                lineScore += 2 * y;
            }
        }
        score += lineScore;
        if (lineSquare == GRID_W) {
            score += 1000 * y; // bonus on bottom lines
            // lineCount++;
        }
    }


    // Too slow and does not play well 
    // int emmental = countHoles(argrid);

    // Fast and play well
    int emmental = countHolesUnderTheRoof(argrid);

    score -= (emmental * 200);  // emmental penalty

    //if (emmental > 0) {
    //    printf("Holes:%d\n", emmental);
    //    debugGrid(argrid);
    //}

    return score;
}

void
Ia::copyGrid(int argrid[GRID_H][GRID_W], int to[GRID_H][GRID_W])
{
    for (int y = 0; y < GRID_H; y++) {
        for (int x = 0; x < GRID_W; x++) {
            to[y][x] = argrid[y][x];
        }
    }
}

bool
Ia::isShapeLeftCollide(int argrid[GRID_H][GRID_W],
                       int shape,
                       int rotation,
                       int row,
                       int col)
{
    for (int y = SHAPE_SIZE - 1; y >= 0; y--) {
        for (int x = 0; x < SHAPE_SIZE; x++) {
            if (tt::gShapesArray[shape][rotation][y * SHAPE_SIZE + x] > 0 &&
                argrid[row + y][col + x - 1] == SOMETHING_IN_SQUARE) {
                return true;
            }
        }
    }
    return false;
}

bool
Ia::isShapeRightCollide(int argrid[GRID_H][GRID_W],
                        int shape,
                        int rotation,
                        int row,
                        int col)
{
    for (int y = SHAPE_SIZE - 1; y >= 0; y--) {
        for (int x = 0; x < SHAPE_SIZE; x++) {
            if (tt::gShapesArray[shape][rotation][y * SHAPE_SIZE + x] > 0 &&
                argrid[row + y][col + x + 1] == SOMETHING_IN_SQUARE) {

                return true;
            }
        }
    }
    return false;
}

int
Ia::findBottomShiftShape(int shape, int rotation)
{
    for (int y = SHAPE_SIZE - 1; y >= 0; y--) {
        int firstLine = 0;
        for (int x = 0; x < SHAPE_SIZE; x++) {
            if (tt::gShapesArray[shape][rotation][y * SHAPE_SIZE + x] > 0) {
                firstLine = 1;
            }
        }
        if (firstLine == 1) {
            return y;
        }
    }
    return 0;
}

int
Ia::findLeftShiftShape(int shape, int rotation)
{
    for (int x = 0; x < SHAPE_SIZE; x++) {
        int firstCol = 0;
        for (int y = 0; y < SHAPE_SIZE; y++) {
            if (tt::gShapesArray[shape][rotation][y * SHAPE_SIZE + x] > 0) {
                firstCol = 1;
            }
        }
        if (firstCol == 1) {
            return x;
        }
    }
    return SHAPE_SIZE - 1;
}

int
Ia::findRightShiftShape(int shape, int rotation)
{
    for (int x = SHAPE_SIZE - 1; x >= 0; x--) {
        int firstCol = 0;
        for (int y = 0; y < SHAPE_SIZE; y++) {
            if (tt::gShapesArray[shape][rotation][y * SHAPE_SIZE + x] > 0) {
                firstCol = 1;
            }
        }
        if (firstCol == 1) {
            return x;
        }
    }
    return 0;
}

inline int
Ia::checkSomethingInHorizontalLine(int argrid[GRID_H][GRID_W],
                                   int x,
                                   int y,
                                   int direction)
{
    int i = x;
    for (i = x; i < GRID_W && i >= 0; i += direction) {
        if (argrid[y][i] == SOMETHING_IN_SQUARE) {
            return i;
        }
    }
    return i;
}

void
Ia::debugGrid(int argrid[GRID_H][GRID_W])
{
    for (int y = 0; y < GRID_H; y++) {
        for (int x = 0; x < GRID_W; x++) {
            if (argrid[y][x] == NOTHING_IN_SQUARE) {
                printf(".");
            } else if (argrid[y][x] == SOMETHING_IN_SQUARE) {
                printf("*");
            } else if (argrid[y][x] == FLOODED_IN_SQUARE) {
                printf("F");
            }
        }
        printf("\n");
    }
}

inline int
Ia::floodFill(int grid[GRID_H][GRID_W], int x, int y, int prevC, int newC)
{
    queue.clear();
    p = { x, y };
    queue.push_back(p);

    // Color the pixel with the new color
    grid[y][x] = newC;

    // While the queue is not empty i.e. the
    // whole component having prevC color
    // is not colored with newC color
    while (queue.size() > 0) {
        // Dequeue the front node
        std::pair<int, int> currPixel = queue[queue.size() - 1];
        queue.pop_back();
        int posX = currPixel.first;
        int posY = currPixel.second;

        if (posY == 0) {
            // Hole when posY at top
            return -1;
        }

        // Check if the adjacent
        // pixels are valid
        if (isValid(grid, posX + 1, posY, prevC, newC)) {
            // Color with newC
            // if valid and enqueue
            // argrid[posX + 1][posY] = newC;
            grid[posY][posX + 1] = newC;
            p.first = posX + 1;
            p.second = posY;
            queue.push_back(p);
        }
        if (isValid(grid, posX - 1, posY, prevC, newC)) {
            // argrid[posX - 1][posY] = newC;
            grid[posY][posX - 1] = newC;
            p.first = posX - 1;
            p.second = posY;
            queue.push_back(p);
        }
        if (isValid(grid, posX, posY + 1, prevC, newC)) {
            // argrid[posX][posY + 1] = newC;
            grid[posY + 1][posX] = newC;
            p.first = posX;
            p.second = posY + 1;
            queue.push_back(p);
        }
        if (isValid(grid, posX, posY - 1, prevC, newC)) {
            // argrid[posX][posY - 1] = newC;
            grid[posY - 1][posX] = newC;
            p.first = posX;
            p.second = posY - 1;
            queue.push_back(p);
        }
    }

    return (int)queue.size(); // Hole size
}

std::stack<Pos>
Ia::reverseStack()
{
    std::stack<Pos> s;
    while (!m_stacksaved.empty()) {
        s.push(m_stacksaved.top());
        m_stacksaved.pop();
    }
    return s;
}

void
Ia::debugStack()
{
    std::stack<Pos> s = m_stacksaved;
    while (!s.empty()) {
        printf("%d-%d(%d,%d=%d) | ",
               s.top().shape,
               s.top().score,
               s.top().col,
               s.top().row,
               s.top().rotation);
        s.pop();
    }
    printf("\n");
}

void
Ia::clearStack(int depth)
{
    m_stacksaved = std::stack<Pos>();
    for (int i = 0; i < /*AUTOPLAY_DEPTH*/depth + 1; i++)
        m_stacksaved.push({0, 0, 0, 0, INT_MIN});
}

void
Ia::debugStack(std::stack<Pos>& s)
{
    while (!s.empty()) {
        printf("%d-%d(%d,%d=%d) | ",
               s.top().shape,
               s.top().score,
               s.top().col,
               s.top().row,
               s.top().rotation);
        s.pop();
    }
    printf("\n");
}

int
Ia::countHoles(int argrid[GRID_H][GRID_W])
{
    int grid[GRID_H][GRID_W];
    int flooded[GRID_H][GRID_W];
    // sf::Clock cl;
    // cl.restart();
    memcpy(grid, argrid, sizeof(int) * GRID_H * GRID_W);
    memcpy(flooded, grid, sizeof(int) * GRID_H * GRID_W);
    for (int x = 0; x < GRID_W; x++) {
        for (int y = 0; y < GRID_H; y++) {
            if (flooded[y][x] == NOTHING_IN_SQUARE) {

                memcpy(grid, flooded, sizeof(int) * GRID_H * GRID_W);
                int c = floodFill(
                  flooded, x, y, NOTHING_IN_SQUARE, FLOODED_IN_SQUARE);
                if (c == -1) {
                    memcpy(flooded, grid, sizeof(int) * GRID_H * GRID_W);
                }
            }
        }
    }
    // int64_t time = cl.getElapsedTime().asMicroseconds();
    // printf("T=%llu\n", time);
    // debugGrid(grid);
    //  TODO : COUNT F

    int holes = 0;
    for (int x = 0; x < GRID_W; x++) {
        for (int y = 0; y < GRID_H; y++) {
            if (flooded[y][x] == FLOODED_IN_SQUARE)
                holes++;
        }
    }

    return holes;
}

int
Ia::countHolesUnderTheRoof(int argrid[GRID_H][GRID_W])
{
    int holes = 0;
    for (int x = 0; x < GRID_W; x++) {
        int startCountingHere = -1;
        for (int y = 0; y < GRID_H; y++) {

            if (startCountingHere == -1 && argrid[y][x] == SOMETHING_IN_SQUARE) {
                startCountingHere = y;
            }

            if (startCountingHere >= 0 && argrid[y][x] == NOTHING_IN_SQUARE) {
                holes++;
            }

        }
    }
    return holes;
}
