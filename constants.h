#pragma once

#include <stdint.h>

#define SHAPE_SIZE 4
#define GRID_W 10
#define GRID_H 20
#define PIXEL_SQUARE_SIZE 24
#define PIXEL_ASTER_SIZE 11
#define PIXEL_FOG_SIZE 100
#define FOG_SHIFT_UP 80
#define SHAPE_IN_SQUARE -1
#define SOMETHING_IN_SQUARE -2
#define NOTHING_IN_SQUARE 0
#define FLOODED_IN_SQUARE -5
#define GHOST_IN_SQUARE -3
#define GAME_OVER_SQUARE -4
#define WINDOW_W 1066
#define WINDOW_H 600
#define FRAME_RATE 30
#define REPEAT_KEYBOARD_LATENCY_MS 100
#define REPEAT_KEYBOARD_FIRST_TIME_LATENCY_MS 400
#define WAIT_BEFORE_SCROLLING_DOWN_MS 250
#define LINE_TIME_MS 100
#define STICKY_TIME_MS 1000
#define SHAPES_QUEUE_SIZE 6
#define FIRE_TIME 40
#define SEND_LINE_PARTICLES_SPEED 10
#define BLINK_GAME_OVER_TIME_EFFECT_MS 1000
#define BLINK_GAME_OVER_SPEED 4

constexpr float BORDER_W_MULT = 1.70f;
constexpr float BORDER_H_MULT = 1.03f;
constexpr float BORDER_SHIFT_LEFT_RELATIVE_TO_GRID = -88.0f;
constexpr float BORDER_SHIFT_HEIGHT_RELATIVE_TO_GRID = -7.0f;

constexpr int NEXT_SHAPES_SHIFT_LEFT_RELATIVE_TO_GRID = 266;
constexpr int NEXT_SHAPES_SHIFT_HEIGHT_RELATIVE_TO_GRID = 42;
constexpr float SMALL_SHAPE_REDUCTION_FACTOR = 2.6f;
constexpr float MEDIUM_SHAPE_REDUCTION_FACTOR = 2.0f;

constexpr int LEVEL_SHIFT_LEFT_RELATIVE_TO_GRID = -32;
constexpr int LEVEL_SHIFT_HEIGHT_RELATIVE_TO_GRID = 468;
constexpr int LINES_PER_LEVEL = 10;
constexpr int LEVEL_HEIGHT = 347;
constexpr int LEVEL_WIDTH = 10;

constexpr int SCORE_SHIFT_LEFT_RELATIVE_TO_GRID = 308;
constexpr int SCORE_SHIFT_HEIGHT_RELATIVE_TO_GRID = 325;
constexpr int SCORE_FONT_SIZE = 15;

constexpr int LINE_SHIFT_LEFT_RELATIVE_TO_GRID = 308;
constexpr int LINE_SHIFT_HEIGHT_RELATIVE_TO_GRID = 375;
constexpr int LINE_FONT_SIZE = 20;

constexpr int LEVELF_SHIFT_LEFT_RELATIVE_TO_GRID = 308;
constexpr int LEVELF_SHIFT_HEIGHT_RELATIVE_TO_GRID = 430;
constexpr int LEVELF_FONT_SIZE = 24;

constexpr float QUICK_TIME_MULTIPLIER = 0.08f;
constexpr float NORMAL_TIME_MULTIPLIER = 1.0f;

constexpr int SLEEP_TIME_BETWEEN_EVERY_MOVE_MS = 100;
constexpr int AUTOPLAY_DEPTH = 5;
constexpr int DOWNGRADED_AUTOPLAY_DEPTH = 3;
constexpr int AUTOPLAY_DEPTH_REGARDING_ALT[] = { 5, 5, 5, 5, 5, 4, 4, 4, 4, 4,
                                                 4, 4, 4, 4, 4, 3, 3, 3, 3, 3 };
constexpr int SEARCHING_STOP_THRESHOLD_GAP = 4;

constexpr int64_t SAFETY_ALLOWANCE_DEALLOCATION_TIME_MS = 1000;