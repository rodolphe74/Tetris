#pragma once

#define SHAPE_SIZE 4
#define GRID_W 10
#define GRID_H 20
#define PIXEL_SQUARE_SIZE 24
#define PIXEL_ASTER_SIZE 8
#define PIXEL_FOG_SIZE 100
#define FOG_SHIFT_UP 80
#define SHAPE_IN_SQUARE -1
#define SOMETHING_IN_SQUARE -2
#define NOTHING_IN_SQUARE 0
#define GHOST_IN_SQUARE -3
#define GAME_OVER_SQUARE -4
#define WINDOW_W 800
#define WINDOW_H 600
#define FRAME_RATE 30
#define REPEAT_KEYBOARD_LATENCY_MS 75
#define WAIT_BEFORE_SCROLLING_DOWN_MS 250
#define LINE_TIME_MS 100
#define STICKY_TIME_MS 1000
constexpr float BORDER_W_MULT = 1.70f;
constexpr float BORDER_H_MULT = 1.03f;
constexpr float BORDER_SHIFT_LEFT = 192.0f;
constexpr float BORDER_SHIFT_HEIGHT = 53.0f;

constexpr int NEXT_SHAPES_SHIFT_LEFT = 546;
constexpr int NEXT_SHAPES_SHIFT_HEIGHT = 99;
constexpr float SMALL_SHAPE_REDUCTION_FACTOR = 2.6f;

constexpr float QUICK_TIME_MULTIPLIER = 0.08f;
constexpr float NORMAL_TIME_MULTIPLIER = 1.0f;