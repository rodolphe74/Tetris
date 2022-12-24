#pragma once

#include "constants.h"
#include "globals.h"
#include "shapes.h"
#include <stack>

class Ia
{
  private:
    static int freezeShape(int argrid[GRID_H][GRID_W],
                           int shape,
                           int shapeCol,
                           int shapeRow,
                           int bottomShiftShape,
                           int shapeRotation,
                           int updatedargrid[GRID_H][GRID_W]);
    static int getScore(int argrid[GRID_H][GRID_W]);
    static void copyGrid(int argrid[GRID_H][GRID_W], int to[GRID_H][GRID_W]);
    static bool isShapeLeftCollide(int argrid[GRID_H][GRID_W],
                                   int shape,
                                   int rotation,
                                   int row,
                                   int col);
    static bool isShapeRightCollide(int argrid[GRID_H][GRID_W],
                                    int shape,
                                    int rotation,
                                    int row,
                                    int col);
    static int findBottomShiftShape(int shape, int rotation);
    static int findLeftShiftShape(int shape, int rotation);
    static int findRightShiftShape(int shape, int rotation);
    static inline int checkSomethingInHorizontalLine(int argrid[GRID_H][GRID_W],
                                                     int x,
                                                     int y,
                                                     int direction);
    static inline int checkSomethingInVerticalLine(int argrid[GRID_H][GRID_W],
                                                   int x,
                                                   int y,
                                                   int direction);

  public:
    static Pos m_arrpositions[16];
    static std::stack<Pos> m_stackcurrent;
    static std::stack<Pos> m_stacksaved;
    static int searchCount;
    static Pos findBestPosition(int argrid[GRID_H][GRID_W],
                                int shapesQueue[SHAPES_QUEUE_SIZE],
                                int currentShape,
                                int currentRotation,
                                int currentBottomShiftShape,
                                int currentLeftShiftShape,
                                int currentRightShiftShape,
                                int row,
                                int col,
                                int currentDepth,
                                int fullDepth);

    static std::stack<Pos> reverseStack();
    static void debugGrid(int argrid[GRID_H][GRID_W]);
    static void debugStack();
    static void debugStack(std::stack<Pos>& s);
    static void clearStack();
};
