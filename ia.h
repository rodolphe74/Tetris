#pragma once

#include "constants.h"
#include "globals.h"
#include "shapes.h"

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

  public:
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

    static void debugGrid(int argrid[GRID_H][GRID_W]);
};
