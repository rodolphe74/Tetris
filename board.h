#pragma once

#include "audioStream.h"
#include "constants.h"
#include "particles.h"
#include "fog.h"
#include "shapes.h"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

enum GameState
{
    none,
    scrollDown,
    rotate,
    scrollRight,
    scrollLeft,
    scrollLine,
    gameOver
};

class Board
{
  private:
    int m_argrid[GRID_H][GRID_W];
    int m_arghost[GRID_H][GRID_W];
    int m_arshape[GRID_H][GRID_W];

    // Graphics
    sf::RenderWindow* m_prenderWindow;
    sf::Texture m_squareTexture;
    sf::Sprite m_squareSprite;
    sf::Sprite m_smallSquareSprite;
    sf::Texture m_borderTexture;
    sf::Sprite m_borderSprite;
    sf::Font m_gameFont;

    // Timers
    sf::Clock m_waitClock;
    sf::Clock m_allowedTimeToMoveAwayClock;

    // Sounds
    sf::SoundBuffer m_soundBufferIntro;
    sf::Sound m_soundIntro;
    sf::SoundBuffer m_soundBufferExplode;
    sf::Sound m_soundExplode;
    sf::SoundBuffer m_soundBufferRotate;
    sf::Sound m_soundRotate;
    sf::SoundBuffer m_soundBufferLine;
    sf::Sound m_soundLine;
    sf::SoundBuffer m_soundBufferMove;
    sf::Sound m_soundMove;
    sf::SoundBuffer m_soundBufferWarp;
    sf::Sound m_soundWarp;

    Particles m_particles;
    Fog m_fog;
    int m_currentShape = -1;
    int m_currentShapeRow = 0;
    int m_currentShapeCol = 0;
    int m_currentShapeRotation = 0;
    int m_currentBottomShiftShape = SHAPE_SIZE - 1;
    int m_currentRightShiftShape = SHAPE_SIZE - 1;
    int m_currentLeftShiftShape = 0;
    float m_currentShiftLeft = 0;
    float m_currentShiftHeight = 0;
    int m_currentFrameRate = 30;
    int m_currentFrameCount = 0;
    int m_arlinesToRemove[GRID_H] = { 0 };
    float m_currentLineExtension = 0.0f;
    float m_currentLineExtensionStep = 1.0f;
    int m_ishapesQueue[6];
    bool m_boolOnceLineSound = false;

  public:
    GameState m_egameState = none;
    bool m_boolOnceMoveSound = true;
    bool m_boolAllowedTimeStarted = false;
    bool m_boolCanAccelerate = true;
    float m_ftimeMultiplier = 1.0f;

    // Music
    static sf::SoundBuffer m_soundBufferMainOne;
    static sf::SoundBuffer m_soundBufferMainTwo;
    static AudioStream m_audioStream;

    Board(sf::RenderWindow& window)
    {
        m_prenderWindow = &window;
        for (int y = 0; y < GRID_H; y++) {
            for (int x = 0; x < GRID_W; x++) {
                m_argrid[y][x] = NOTHING_IN_SQUARE;
                m_arghost[y][x] = NOTHING_IN_SQUARE;
                m_arshape[y][x] = NOTHING_IN_SQUARE;
            }
        }

        // load square sprite
        if (!m_squareTexture.loadFromFile("big_square_clear.png")) {
            perror("can't load texture");
        }
        m_squareTexture.setSmooth(true);
        m_squareSprite.setTexture(m_squareTexture);
        float rx = PIXEL_SQUARE_SIZE / (float)m_squareTexture.getSize().x;
        float ry = PIXEL_SQUARE_SIZE / (float)m_squareTexture.getSize().y;
        m_squareSprite.setScale(rx, ry);

        m_smallSquareSprite.setTexture(m_squareTexture);
        rx = (PIXEL_SQUARE_SIZE / SMALL_SHAPE_REDUCTION_FACTOR) /
             (float)m_squareTexture.getSize().x;
        ry = (PIXEL_SQUARE_SIZE / SMALL_SHAPE_REDUCTION_FACTOR) /
             (float)m_squareTexture.getSize().y;
        m_smallSquareSprite.setScale(rx, ry);

        // load border sprite
        if (!m_borderTexture.loadFromFile("borderb.png")) {
            perror("can't load texture");
        }
        m_borderTexture.setSmooth(true);
        m_borderSprite.setTexture(m_borderTexture);
        rx = (PIXEL_SQUARE_SIZE * GRID_W) / (float)m_borderTexture.getSize().x;
        rx *= BORDER_W_MULT;
        ry = (PIXEL_SQUARE_SIZE * GRID_H) / (float)m_borderTexture.getSize().y;
        ry *= BORDER_H_MULT;
        m_borderSprite.setScale(rx, ry);
        printf("%f\n", m_borderSprite.getGlobalBounds().width);
        printf("%f\n", m_borderSprite.getGlobalBounds().height);

        // load font
        if (!m_gameFont.loadFromFile("Brick3DRegular-nRJR4.ttf")) {
            perror("can't load font");
        }

        // load sounds
        if (!m_soundBufferExplode.loadFromFile("explode.ogg")) {
            perror("can't load sound");
        }
        m_soundExplode.setBuffer(m_soundBufferExplode);

        if (!m_soundBufferRotate.loadFromFile("rotate.ogg")) {
            perror("can't load sound");
        }
        m_soundRotate.setBuffer(m_soundBufferRotate);

        if (!m_soundBufferMove.loadFromFile("move.ogg")) {
            perror("can't load sound");
        }
        m_soundMove.setBuffer(m_soundBufferMove);

        if (!m_soundBufferLine.loadFromFile("line.ogg")) {
            perror("can't load sound");
        }
        m_soundLine.setBuffer(m_soundBufferLine);

        if (!m_soundBufferWarp.loadFromFile("warp.ogg")) {
            perror("can't load sound");
        }
        m_soundWarp.setBuffer(m_soundBufferWarp);

        if (!m_soundBufferIntro.loadFromFile("intro.ogg")) {
            perror("can't load sound");
        }
        m_soundIntro.setBuffer(m_soundBufferIntro);

        srand((unsigned int)time(NULL));
        for (int i = 0; i < sizeof(m_ishapesQueue) / sizeof(int); i++) {
            float s = static_cast<float>(rand()) /
                      (static_cast<float>(RAND_MAX / 6.0f));
            m_ishapesQueue[i] = (int)round(s);
        }
    }

    void renderSquare(const tt::rect& squareRect,
                      const sf::Color& color,
                      bool isGhost);
    void renderSmallSquare(const tt::rect& squareRect, const sf::Color& color);
    bool setCurrentShape(int shapeIndex, int row, int col, int currentRotation);
    void render(float shiftLeft,
                float shiftHeight,
                int& framesCount,
                int frameRate);
    void drawFreezedAndGhostSquares(float shiftLeft, float shiftHeight);
    void clear();
    void clearCurrentShape();
    void findCurrentBottomShiftShape();
    void findCurrentRightShiftShape();
    int findCurrentRightShiftShape(int shape, int rotation);
    void findCurrentLeftShiftShape();
    bool checkIfCurrentBottomShiftShapeCollide();
    bool checkIfCurrentLeftShiftShapeCollide();
    bool checkIfCurrentRightShiftShapeCollide();
    void freezeCurrentShape();
    bool removesFullLines();
    void scrollEverythingDown(int fromLine);
    void right();
    void left();
    void accelerate();
    void normalSpeed();
    void warp();
    void rotate();
    int getNextShape();
    void drawNextShapes();
    void updateGhost();
    void youLoose();
    void playIntro();
    void downSpecifics();
    tt::rect locateCurrentShape();
};
