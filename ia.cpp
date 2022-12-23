#include "ia.h"
#include "shapes.h"

int Ia::searchCount = 0;
Pos Ia::m_arrpositions[16] = {};

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

    if (currentDepth == 0) {
        searchCount++;
        int score = getScore(argrid);
        Pos pos = { row, col, currentRotation, currentShape, score };
        return pos;
    }

    // find best score at row
    int bestScore = 0;
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

            if (fullDepth - currentDepth != 0) {
                // printf("Shape Queue:%d\n", fullDepth - currentDepth - 1);
                currentShape = shapesQueue[fullDepth - currentDepth - 1];
            }

            // printf("CurrentShape:%d\n", currentShape);

            Pos currentPos = findBestPosition(updatedGrid,
                                              shapesQueue,
                                              currentShape,
                                              currentRotation,
                                              currentBottomShiftShape,
                                              currentLeftShiftShape,
                                              currentRightShiftShape,
                                              row,
                                              col,
                                              currentDepth - 1,
                                              fullDepth);
            int currentScore = currentPos.score;

            if (currentScore > bestScore) {
                bestScore = currentScore;
                bestCol = x;
                bestRow = r;
                bestRot = rot;
            }

            // Ia::debugGrid(updatedGrid);
            // printf("Score:%d\n\n", currentScore);
        }
    }

    // printf(
    //   "bestScore:%d / bestCol:%d / bestRow:%d\n", bestScore, bestCol,
    //   bestRow);

    
    Pos pos = { bestRow, bestCol, bestRot, currentShape, bestScore };
    
    // TODO : for every depth, compare with existing and replace in vec if
    // necessary
    //m_arrpositions[currentDepth - 1] = pos;

    return pos;
}

int
Ia::getScore(int argrid[GRID_H][GRID_W])
{
    int score = 0;
    int lineScore = 0;
    int lineSquare = 0;
    int lineCount = 0;
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
             //score += 1000;
            //if (lineCount > 2)
            //    printf("   lineCount:%d\n", lineCount);
            score += 1000 * (int)powf(10, (float)lineCount);
            lineCount++;
        }
    }

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

void
Ia::debugGrid(int argrid[GRID_H][GRID_W])
{
    for (int y = 0; y < GRID_H; y++) {
        for (int x = 0; x < GRID_W; x++) {
            if (argrid[y][x] == NOTHING_IN_SQUARE) {
                printf(".");
            } else if (argrid[y][x] == SOMETHING_IN_SQUARE) {
                printf("*");
            }
        }
        printf("\n");
    }
}
