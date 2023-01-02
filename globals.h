#pragma once

struct Pos
{
    int row, col;
    int rotation;
    int shape;
    int score;
};

typedef Pos Pos;

enum GameState
{
    none,
    scrollDown,
    rotate,
    scrollRight,
    scrollLeft,
    scrollLine,
    collapse,
    scrollUp,
    gameOver,
    pause,
    undefined
};