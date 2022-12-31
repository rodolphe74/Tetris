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
    static inline bool isValid(int argrid[GRID_H][GRID_W],
                               int x,
                               int y,
                               int prevC,
                               int newC);
    static inline int floodFill(int argrid[GRID_H][GRID_W],
                                int x,
                                int y,
                                int prevC,
                                int newC);

    static std::vector<std::pair<int, int>> queue;
    static std::pair<int, int> p;

  public:
    static std::stack<Pos> m_stackcurrent;
    static std::stack<Pos> m_stacksaved;
    static int m_isearchCount;
    static bool m_boolsearching;
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

    static int countHoles(int argrid[GRID_H][GRID_W]);
    static int countHolesUnderTheRoof(int argrid[GRID_H][GRID_W]);

    static std::stack<Pos> reverseStack();
    static void debugGrid(int argrid[GRID_H][GRID_W]);
    static void debugStack();
    static void debugStack(std::stack<Pos>& s);
    static void clearStack(int depth);
};
