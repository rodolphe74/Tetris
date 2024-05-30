#pragma once

#include "audioStream.h"
#include "constants.h"
#include "fog.h"
#include "gameStatesQueue.h"
#include "globals.h"
#include "math.h"
#include "particles.h"
#include "shapes.h"
#include "string.h"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

class Board
{
  private:
    int m_argrid[GRID_H][GRID_W];
    int m_arghost[GRID_H][GRID_W];
    int m_arshape[GRID_H][GRID_W];
    int m_argameOverShape[GRID_H][GRID_W];

    // Graphics
    sf::RenderWindow* m_prenderWindow;
    //sf::Texture m_squareTexture;
    sf::Texture *m_squareTexturePtr;
    sf::Sprite m_squareSprite;
    sf::Sprite m_smallSquareSprite;
    sf::Sprite m_mediumSquareSprite;
    //sf::Texture m_borderTexture;
    sf::Texture *m_borderTexturePtr;
    sf::Sprite m_borderSprite;
    //sf::Font m_gameFont;
    sf::Font *m_gameFontPtr;

    // Timers
    sf::Clock m_waitClock;
    sf::Clock m_allowedTimeToMoveAwayClock;
    sf::Clock m_keyLatencyClock;

    // Thread
    sf::Thread* m_moveComputerThread = NULL;
    bool m_bisComputerMoving = false;
    bool m_moveComputerThreadAllocated = false;

    
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
    /*int m_arlinesToRemove[GRID_H] = { 0 };*/
    int m_arlinesToCollapse[GRID_H] = { 0 };
    float m_currentLineExtension = 0.0f;
    float m_currentLineExtensionStep = 1.0f;
    int m_ishapesQueue[SHAPES_QUEUE_SIZE];
    bool m_boolonceLineSound = false;
    bool m_boolshouldRotate = 0;
    bool m_boolshouldWarp = 0;
    int m_ishouldCollapse = 0;
    bool m_boolshouldAddLinesToOpponent = 0;
    bool m_boolshouldStopSearchingThread = 0;
    bool m_boolparticlesLevel = false;
    int m_icountLines = 0;
    int m_icountLinesPerLevel = 0;
    int m_iscore = 0;
    
    
    std::string m_strscore = "0";
    std::string m_strcountLines = "0";
    std::string m_strlevel = "1";
    sf::Text m_textscore;
    sf::Text m_textline;
    sf::Text m_textlevel;

  public:
    GameStatesQueue m_equeueGameStates;
    GameState m_ecurrentGameState;
    bool m_waitNextTurn = true;
    int m_ilevel = 1;
    bool m_boolOnceMoveSound = true;
    bool m_boolAllowedTimeStarted = false;
    bool m_boolCanAccelerate = true;
    float m_ftimeMultiplier = 1.0f;
    bool m_boolAutoplay = false;
    int m_icountScrolledDown = 0;
    int m_ikeyRepeatCount = 0;
    int m_arlinesToRemove[GRID_H] = { 0 };
    int m_isearchDepth = AUTOPLAY_DEPTH;

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
                m_argameOverShape[y][x] = NOTHING_IN_SQUARE;
            }
        }

        m_squareTexturePtr = resources::getSquare();
        m_squareTexturePtr->setSmooth(true);
        m_squareSprite.setTexture(*m_squareTexturePtr);
        float rx = PIXEL_SQUARE_SIZE / (float)(*m_squareTexturePtr).getSize().x;
        float ry = PIXEL_SQUARE_SIZE / (float)(*m_squareTexturePtr).getSize().y;
        m_squareSprite.setScale(rx, ry);


        //m_smallSquareSprite.setTexture(m_squareTexture);
        m_smallSquareSprite.setTexture(*m_squareTexturePtr);
        rx = (PIXEL_SQUARE_SIZE / SMALL_SHAPE_REDUCTION_FACTOR) /
             (float)(*m_squareTexturePtr).getSize().x;
        ry = (PIXEL_SQUARE_SIZE / SMALL_SHAPE_REDUCTION_FACTOR) /
             (float)(*m_squareTexturePtr).getSize().y;
        m_smallSquareSprite.setScale(rx, ry);

        m_mediumSquareSprite.setTexture(*m_squareTexturePtr);
        rx = (PIXEL_SQUARE_SIZE / MEDIUM_SHAPE_REDUCTION_FACTOR) /
             (float)(*m_squareTexturePtr).getSize().x;
        ry = (PIXEL_SQUARE_SIZE / MEDIUM_SHAPE_REDUCTION_FACTOR) /
             (float)(*m_squareTexturePtr).getSize().y;
        m_mediumSquareSprite.setScale(rx, ry);

        m_borderTexturePtr = resources::getBorder();
        (*m_borderTexturePtr).setSmooth(true);
        m_borderSprite.setTexture(*m_borderTexturePtr);
        rx =
          (PIXEL_SQUARE_SIZE * GRID_W) / (float)(*m_borderTexturePtr).getSize().x;
        rx *= BORDER_W_MULT;
        ry =
          (PIXEL_SQUARE_SIZE * GRID_H) / (float)(*m_borderTexturePtr).getSize().y;
        ry *= BORDER_H_MULT;
        m_borderSprite.setScale(rx, ry);
        printf("%f\n", m_borderSprite.getGlobalBounds().width);
        printf("%f\n", m_borderSprite.getGlobalBounds().height);

        m_gameFontPtr = resources::getFont();

        // init strings and text
        m_textscore.setFont(*m_gameFontPtr);
        m_textscore.setCharacterSize(SCORE_FONT_SIZE);
        m_textline.setFont(*m_gameFontPtr);
        m_textline.setCharacterSize(LINE_FONT_SIZE);
        m_textlevel.setFont(*m_gameFontPtr);
        m_textlevel.setCharacterSize(LEVELF_FONT_SIZE);

        srand((unsigned int)time(NULL));
        for (int i = 0; i < sizeof(m_ishapesQueue) / sizeof(int); i++) {
            int s = rand() % 7;
            m_ishapesQueue[i] = s;
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
    void setGameOverShape(int shapeIndex, int row, int col, int currentRotation);
    void render(float shiftLeft,
                float shiftHeight,
                int& framesCount,
                int frameRate);
    void drawCurrentShape(float shiftLeft, float shiftHeight);
    void drawGameOverShape(float shiftLeft,
                          float shiftHeight,
                          const sf::Color& color);
    void checkKeyboard();
    void drawFreezedAndGhostSquares(float shiftLeft, float shiftHeight);
    void clear();
    void clearCurrentShape();
    void findCurrentBottomShiftShape();
    void findCurrentRightShiftShape();
    void findCurrentLeftShiftShape();
    bool checkIfCurrentBottomShiftShapeCollide();
    bool checkIfCurrentLeftShiftShapeCollide();
    bool checkIfCurrentRightShiftShapeCollide();
    bool checkIfCurrentBottomShapeNearToCollide();
    int getAltitude();
    void freezeCurrentShape();
    bool removeFullLines();
    bool removeEmptyLines();
    void receiveLinesFromOpponent(int count);
    void receiveLineFromOpponent();
    void scrollEverythingDown(int fromLine);
    void scrollEverythingUp(int fromLine);
    void right();
    void left();
    void accelerate();
    void normalSpeed();
    void warp();
    void rotate();
    int getNextShape();
    void drawNextShapes();
    void drawLevel();
    void drawScore();
    void updateGhost();
    void looser(bool isHumanLoose);
    void playIntro();
    void downSpecifics();
    tt::rect locateCurrentShape();
    float getNextLevelMultiplier();
    float getCurrentLevelMultiplier();
    void iaMoveThread();
    void freeAutoplayThread();
};
