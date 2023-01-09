#pragma once

struct Pos
{
    int row, col;
    int rotation;
    int shape;
    int score;
};

struct ParticlePos
{
    float x;
    float y;
    int count;
    int colorScheme;
};

typedef Pos Pos;
typedef ParticlePos ParticlePos;

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
    preGameOver,
    gameOver,
    pause,
    undefined
};