#pragma once

#include "board.h"

struct Pos
{
    int row, col;
    int rotation;
    int shape;
    int score;
};

typedef Pos Pos;
