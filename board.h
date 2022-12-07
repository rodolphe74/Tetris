#pragma once

#include "audioStream.h"
#include "constants.h"
#include "fog.h"
#include "particles.h"
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
    sf::Sprite m_mediumSquareSprite;
    sf::Texture m_borderTexture;
    sf::Sprite m_borderSprite;
    sf::Font m_gameFont;

    // Timers
    sf::Clock m_waitClock;
    sf::Clock m_allowedTimeToMoveAwayClock;
    sf::Clock m_keyLatencyClock;

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
    sf::SoundBuffer m_soundBufferHurry;
    sf::Sound m_soundHurry;
    sf::SoundBuffer m_soundBufferGameOver;
    sf::Sound m_soundGameOver;

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
    bool m_boolonceLineSound = false;
    bool m_boolshouldRotate = 0;
    bool m_boolshouldWarp = 0;
    bool m_boolparticlesLevel = false;
    int m_ilevel = 0;
    int m_icountLines = 0;
    int m_icountLinesPerLevel = 0;

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
        if (!m_squareTexture.loadFromFile("resources/big_square_clear.png")) {
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

        m_mediumSquareSprite.setTexture(m_squareTexture);
        rx = (PIXEL_SQUARE_SIZE / MEDIUM_SHAPE_REDUCTION_FACTOR) /
             (float)m_squareTexture.getSize().x;
        ry = (PIXEL_SQUARE_SIZE / MEDIUM_SHAPE_REDUCTION_FACTOR) /
             (float)m_squareTexture.getSize().y;
        m_mediumSquareSprite.setScale(rx, ry);

        // load border sprite
        if (!m_borderTexture.loadFromFile("resources/borderb.png")) {
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
        if (!m_gameFont.loadFromFile("resources/Brick3DRegular-nRJR4.ttf")) {
            perror("can't load font");
        }

        // load sounds
        if (!m_soundBufferExplode.loadFromFile("resources/explode.ogg")) {
            perror("can't load sound");
        }
        m_soundExplode.setBuffer(m_soundBufferExplode);

        if (!m_soundBufferRotate.loadFromFile("resources/rotate.ogg")) {
            perror("can't load sound");
        }
        m_soundRotate.setBuffer(m_soundBufferRotate);

        if (!m_soundBufferMove.loadFromFile("resources/move.ogg")) {
            perror("can't load sound");
        }
        m_soundMove.setBuffer(m_soundBufferMove);

        if (!m_soundBufferLine.loadFromFile("resources/line.ogg")) {
            perror("can't load sound");
        }
        m_soundLine.setBuffer(m_soundBufferLine);

        if (!m_soundBufferWarp.loadFromFile("resources/warp.ogg")) {
            perror("can't load sound");
        }
        m_soundWarp.setBuffer(m_soundBufferWarp);

        if (!m_soundBufferIntro.loadFromFile("resources/intro.ogg")) {
            perror("can't load sound");
        }
        m_soundIntro.setBuffer(m_soundBufferIntro);

        if (!m_soundBufferWarp.loadFromFile("resources/warp.ogg")) {
            perror("can't load sound");
        }
        m_soundWarp.setBuffer(m_soundBufferWarp);

        if (!m_soundBufferIntro.loadFromFile("resources/intro.ogg")) {
            perror("can't load sound");
        }
        m_soundIntro.setBuffer(m_soundBufferIntro);

        if (!m_soundBufferHurry.loadFromFile("resources/hurry.ogg")) {
            perror("can't load sound");
        }
        m_soundHurry.setBuffer(m_soundBufferHurry);

        if (!m_soundBufferGameOver.loadFromFile("resources/gameover.ogg")) {
            perror("can't load sound");
        }
        m_soundGameOver.setBuffer(m_soundBufferGameOver);

        srand((unsigned int)time(NULL));
        for (int i = 0; i < sizeof(m_ishapesQueue) / sizeof(int); i++) {
            float s = static_cast<float>(rand()) /
                      (static_cast<float>(RAND_MAX / 6.0f));
            m_ishapesQueue[i] = (int)round(s);
        }

        bool m_shouldRotate = 0;
        bool m_shouldWarp = 0;
    }

    void renderSquare(const tt::rect& squareRect,
                      const sf::Color& color,
                      bool isGhost);
    void renderSmallSquare(const tt::rect& squareRect, const sf::Color& color);
    void renderMediumSquare(const tt::rect& squareRect, const sf::Color& color);
    bool setCurrentShape(int shapeIndex, int row, int col, int currentRotation);
    void render(float shiftLeft,
                float shiftHeight,
                int& framesCount,
                int frameRate);
    void checkKeyboard();
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
    void drawLevel();
    void updateGhost();
    void youLoose();
    void playIntro();
    void downSpecifics();
    tt::rect locateCurrentShape();
    float getNextLevelMultiplier();
    float getCurrentLevelMultiplier();
};
