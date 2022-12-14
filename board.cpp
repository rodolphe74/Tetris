#include "board.h"
#include "globals.h"
#include "ia.h"
#include "shapes.h"
#include <string>

// Static members
sf::SoundBuffer Board::m_soundBufferMainTwo;
sf::SoundBuffer Board::m_soundBufferMainOne;
AudioStream Board::m_audioStream;

void
Board::renderSquare(const tt::rect& squareRect,
                    const sf::Color& color,
                    bool isGhost)
{
    m_squareSprite.setColor(color);

    if (isGhost) {
        m_squareSprite.setColor(sf::Color(color.r, color.g, color.b, 64));
    }

    m_squareSprite.setPosition(
      sf::Vector2f((float)squareRect.x, (float)squareRect.y));
    m_prenderWindow->draw(m_squareSprite);
}

void
Board::renderSmallSquare(const tt::rect& squareRect, const sf::Color& color)
{
    m_smallSquareSprite.setColor(color);

    m_smallSquareSprite.setPosition(
      sf::Vector2f((float)squareRect.x, (float)squareRect.y));
    m_prenderWindow->draw(m_smallSquareSprite);
}

void
Board::renderMediumSquare(const tt::rect& squareRect, const sf::Color& color)
{
    m_mediumSquareSprite.setColor(color);

    m_mediumSquareSprite.setPosition(
      sf::Vector2f((float)squareRect.x, (float)squareRect.y));
    m_prenderWindow->draw(m_mediumSquareSprite);
}

bool
Board::setCurrentShape(int shapeIndex, int row, int col, int rotation)
{
    m_currentShape = shapeIndex;
    m_currentShapeRow = row;
    m_currentShapeCol = col;
    m_currentShapeRotation = rotation;
    const int* shape = tt::gShapesArray[shapeIndex][rotation];
    int squarecount = 0;
    for (int y = row; y < SHAPE_SIZE + row; y++) {
        for (int x = col; x < SHAPE_SIZE + col; x++) {
            if (y < GRID_H && x < GRID_W && shape[squarecount] == 1) {
                if (m_argrid[y][x] != NOTHING_IN_SQUARE)
                    return false;
                m_arshape[y][x] = SHAPE_IN_SQUARE;
            }
            squarecount++;
        }
    }
    return true;
}

void
Board::render(float shiftLeft,
              float shiftHeight,
              int& framesCount,
              int frameRate)
{
    // Save current render state
    m_currentShiftLeft = shiftLeft;
    m_currentShiftHeight = shiftHeight;
    m_currentFrameRate = frameRate;
    m_currentFrameCount = framesCount;

    // Draw humanBoard
    sf::RectangleShape r(
      sf::Vector2f(GRID_W * PIXEL_SQUARE_SIZE, GRID_H * PIXEL_SQUARE_SIZE));
    r.setFillColor(sf::Color(sf::Color::Black));
    r.setPosition(sf::Vector2f(shiftLeft, shiftHeight));
    m_prenderWindow->draw(r);

    for (int y = 0; y <= GRID_H; y++) {
        for (int x = 0; x <= GRID_W; x++) {
            sf::Vertex line[] = {
                sf::Vertex(sf::Vector2f(shiftLeft + x * PIXEL_SQUARE_SIZE,
                                        shiftHeight + y),
                           sf::Color(64, 64, 64, 128)),
                sf::Vertex(sf::Vector2f(shiftLeft + x * PIXEL_SQUARE_SIZE,
                                        shiftHeight + y * PIXEL_SQUARE_SIZE),
                           sf::Color(64, 64, 64, 128))
            };
            m_prenderWindow->draw(line, 2, sf::Lines);
        }
        sf::Vertex line[] = {
            sf::Vertex(
              sf::Vector2f(shiftLeft + 0, shiftHeight + y * PIXEL_SQUARE_SIZE),
              sf::Color(64, 64, 64, 128)),
            sf::Vertex(sf::Vector2f(shiftLeft + GRID_W * PIXEL_SQUARE_SIZE,
                                    shiftHeight + y * PIXEL_SQUARE_SIZE),
                       sf::Color(64, 64, 64, 128))
        };
        m_prenderWindow->draw(line, 2, sf::Lines);
    }

    // Draw border
    m_borderSprite.setPosition(shiftLeft + BORDER_SHIFT_LEFT_RELATIVE_TO_GRID,
                               shiftHeight +
                                 BORDER_SHIFT_HEIGHT_RELATIVE_TO_GRID);
    m_prenderWindow->draw(m_borderSprite);

    // Draw next shapes
    drawNextShapes();

    // Check bounds
    if (m_currentShapeRow > GRID_H - (m_currentBottomShiftShape + 2 /*5*/)) {

        // allowed time to move away
        if (!m_boolAllowedTimeStarted) {
            m_boolAllowedTimeStarted = true;
            m_allowedTimeToMoveAwayClock.restart();
            // printf("Tu peux encore bouger ...\n");
        }

        else if (m_allowedTimeToMoveAwayClock.getElapsedTime()
                   .asMilliseconds() > STICKY_TIME_MS) {
            // printf("Tu peux plus bouger !\n");
            freezeCurrentShape();
            bool linesRemoved = removesFullLines();
            // printf("linesRemoved:%d\n", linesRemoved);

            setCurrentShape(getNextShape(), 0, GRID_W / 2 - SHAPE_SIZE / 2, 0);
            findCurrentBottomShiftShape();
            m_ftimeMultiplier = getCurrentLevelMultiplier();
            m_boolAllowedTimeStarted = false;
            m_boolCanAccelerate = false;
            m_iscore += 7;
            m_strscore = std::to_string(m_iscore);
        }
    }

    // Check other shapes
    if (checkIfCurrentBottomShiftShapeCollide()) {

        // allowed time to move away
        if (!m_boolAllowedTimeStarted) {
            m_boolAllowedTimeStarted = true;
            m_allowedTimeToMoveAwayClock.restart();
            // printf("Tu peux encore bouger ...\n");
        } else if (m_allowedTimeToMoveAwayClock.getElapsedTime()
                     .asMilliseconds() > STICKY_TIME_MS) {

            // printf("Tu peux plus bouger !\n");
            freezeCurrentShape();
            bool linesRemoved = removesFullLines();
            // printf("linesRemoved:%d\n", linesRemoved);

            setCurrentShape(getNextShape(), 0, GRID_W / 2 - SHAPE_SIZE / 2, 0);
            findCurrentBottomShiftShape();
            m_ftimeMultiplier = getCurrentLevelMultiplier();
            m_boolAllowedTimeStarted = false;
            m_boolCanAccelerate = false;
            m_iscore += 7;
            m_strscore = std::to_string(m_iscore);
        }
    }

    // Draw score
    drawScore();

    // Draw level
    drawLevel();

    // Draw ghost
    updateGhost();

    // Draw squares other than current moving shape
    drawFreezedAndGhostSquares(shiftLeft, shiftHeight);

    // Particles to draw ?
    m_particles.moveParticles();
    m_particles.cleanParticlesArray();
    m_particles.renderParticles(*m_prenderWindow);

    // Fog to draw ?
    m_fog.moveFog();
    m_fog.renderFog(*m_prenderWindow);

    // Game states
    if (m_egameState == scrollDown) {

        // Draw current shape
        for (int y = 0; y < GRID_H; y++) {
            for (int x = 0; x < GRID_W; x++) {
                if (m_arshape[y][x] == SHAPE_IN_SQUARE) {
                    renderSquare({ (int)shiftLeft + x * PIXEL_SQUARE_SIZE,
                                   (int)shiftHeight + y * PIXEL_SQUARE_SIZE,
                                   0,
                                   0 },
                                 tt::gShapesColor[m_currentShape],
                                 false);
                }
            }
        }

        if (framesCount % ((int)(frameRate * m_ftimeMultiplier)) == 0) {
            clearCurrentShape();

            // possibly m_boolAllowedTimeStarted : hardening near tetrominoes
            // tests
            if (!checkIfCurrentBottomShiftShapeCollide())
                if (m_currentShapeRow < GRID_H - m_currentBottomShiftShape - 1)
                    m_currentShapeRow++;

            bool isPlaced = setCurrentShape(m_currentShape,
                                            m_currentShapeRow,
                                            m_currentShapeCol,
                                            m_currentShapeRotation);

            if (!isPlaced) {
                // Game Over effect
                for (int y = 0; y < GRID_H; y++) {
                    for (int x = 0; x < GRID_W; x++) {
                        if (m_argrid[y][x] == SOMETHING_IN_SQUARE) {
                            sf::Vector2f v = sf::Vector2f(
                              m_currentShiftLeft + x * PIXEL_SQUARE_SIZE,
                              m_currentShiftHeight + y * PIXEL_SQUARE_SIZE);
                            m_particles.addParticles(v.x, v.y, 10, 1);
                            m_argrid[y][x] = GAME_OVER_SQUARE;
                        }
                    }
                }
                m_egameState = gameOver;
                m_soundGameOver.play();
            }
        }
    }

    // Check if full lines to remove
    if (m_egameState == scrollLine) {
        for (int y = 0; y < GRID_H; y++) {
            if (m_arlinesToRemove[y]) {
                for (int x = 0; x < GRID_W; x++) {
                    m_argrid[y][x] = NOTHING_IN_SQUARE;
                }
                sf::RectangleShape line(
                  sf::Vector2f(m_currentLineExtension, 2.0f));
                if (m_waitClock.getElapsedTime().asMilliseconds() <
                    LINE_TIME_MS) {
                    line.setPosition(
                      shiftLeft + (GRID_W * PIXEL_SQUARE_SIZE) / 2.0f -
                        m_currentLineExtension / 2.0f,
                      m_currentShiftHeight + y * PIXEL_SQUARE_SIZE +
                        PIXEL_SQUARE_SIZE / 2.0f - 1.0f);
                    line.setFillColor(sf::Color(255, 192, 192));
                    m_prenderWindow->draw(line);
                }
            }
        }

        m_currentLineExtension += m_currentLineExtensionStep;
        if (!m_boolonceLineSound) {
            m_soundLine.play();
            m_boolonceLineSound = true;
        }

        //  Wait a bit before scrolling lines down
        m_icountScrolledDown = 0;
        if (m_waitClock.getElapsedTime().asMilliseconds() >
            WAIT_BEFORE_SCROLLING_DOWN_MS) {
            for (int y = 0; y < GRID_H; y++) {
                if (m_arlinesToRemove[y]) {
                    scrollEverythingDown(y);
                    m_icountScrolledDown++;
                    m_icountLines++;
                    m_icountLinesPerLevel++;
                    m_strcountLines = std::to_string(m_icountLines);
                    m_boolparticlesLevel = true;
                    if (m_icountLinesPerLevel > LINES_PER_LEVEL) {
                        m_ilevel++;
                        m_ftimeMultiplier = getNextLevelMultiplier();
                        m_icountLinesPerLevel = 0;
                        m_strlevel = std::to_string(m_ilevel);
                    }
                }
            }
            if (m_icountScrolledDown == 1) {
                m_iscore += m_ilevel * 100;
            } else if (m_icountScrolledDown == 2) {
                m_iscore += m_ilevel * 300;
            } else if (m_icountScrolledDown == 3) {
                m_iscore += m_ilevel * 500;
            } else if (m_icountScrolledDown == 4) {
                m_iscore += m_ilevel * 800;
            }
            m_strscore = std::to_string(m_iscore);

            // printf(">>>>>%d %d %d = %f\n",
            //        m_ilevel,
            //        m_icountLines,
            //        m_icountLinesPerLevel,
            //        m_ftimeMultiplier);

            m_soundExplode.play();
            // m_ftimeMultiplier = NORMAL_TIME_MULTIPLIER - (m_ilevel * 0.2f);
            m_ftimeMultiplier = getCurrentLevelMultiplier();
            m_egameState = scrollDown;
            m_boolonceLineSound = false;
            // in case of AUTOPLAY authorize computer to play next shape after
            // lines scrolling down effect
            m_waitNextTurn = false;
        }
    }

    // shall we free some allocated resources
    if (!m_bisComputerMoving && m_moveComputerThreadAllocated) {
        printf("freeing thread allocation\n");
        m_moveComputerThreadAllocated = false;
        delete m_moveComputerThread;
    }

    // Are we in autoplay ?
    if (m_boolAutoplay && !m_bisComputerMoving && !m_waitNextTurn &&
        m_egameState != gameOver && m_egameState != none) {
        iaMoveThread();
        m_waitNextTurn = true;
    }

    if (m_egameState == gameOver) {
        m_waitNextTurn = true;
        youLoose();
    }

    if (framesCount == frameRate * 1000) {
        framesCount = 0;
    }
}

void
Board::checkKeyboard()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) &&
        m_keyLatencyClock.getElapsedTime().asMilliseconds() >=
          REPEAT_KEYBOARD_LATENCY_MS &&
        m_egameState != none) {
        left();
        m_keyLatencyClock.restart();
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) &&
        m_keyLatencyClock.getElapsedTime().asMilliseconds() >=
          REPEAT_KEYBOARD_LATENCY_MS &&
        m_egameState != none) {
        right();
        m_keyLatencyClock.restart();
    }

    if (m_boolshouldRotate && sf::Keyboard::isKeyPressed(sf::Keyboard::Up) &&
        m_egameState != none) {
        rotate();
        m_boolshouldRotate = false;
    }

    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !m_boolshouldRotate &&
        m_egameState != none) {
        m_boolshouldRotate = true;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && m_boolCanAccelerate &&
        m_egameState != none) {
        accelerate();
        downSpecifics();
    }
    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Down) &&
        !m_boolCanAccelerate && m_egameState != none) {
        m_boolCanAccelerate = true;
    }
    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Down) &&
        m_egameState != none) {
        normalSpeed();
    }

    if (m_boolshouldWarp &&
        sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) &&
        m_egameState != none) {
        m_boolshouldWarp = false;
        warp();
    }
    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) &&
        !m_boolshouldWarp && m_egameState != none) {
        m_boolshouldWarp = true;
    }

    // if (sf::Keyboard::isKeyPressed(sf::Keyboard::I) && !m_boolshouldIa) {
    //     printf("IA\n");
    //     iaMoveThread();
    //     m_boolshouldIa = true;
    // }
    // if (!sf::Keyboard::isKeyPressed(sf::Keyboard::I) && m_boolshouldIa) {
    //     m_boolshouldIa = false;
    // }
}

void
Board::drawFreezedAndGhostSquares(float shiftLeft, float shiftHeight)
{
    for (int y = 0; y < GRID_H; y++) {
        for (int x = 0; x < GRID_W; x++) {
            if (m_argrid[y][x] == SOMETHING_IN_SQUARE) {
                renderSquare({ (int)shiftLeft + x * PIXEL_SQUARE_SIZE,
                               (int)shiftHeight + y * PIXEL_SQUARE_SIZE,
                               0,
                               0 },
                             tt::gFreezedColor,
                             false);
            } else if (m_arghost[y][x] == GHOST_IN_SQUARE &&
                       m_egameState != gameOver) {
                renderSquare({ (int)shiftLeft + x * PIXEL_SQUARE_SIZE,
                               (int)shiftHeight + y * PIXEL_SQUARE_SIZE,
                               0,
                               0 },
                             tt::gShapesColor[m_currentShape],
                             true);
            } else if (m_argrid[y][x] == GAME_OVER_SQUARE) {
                renderSquare({ (int)shiftLeft + x * PIXEL_SQUARE_SIZE,
                               (int)shiftHeight + y * PIXEL_SQUARE_SIZE,
                               0,
                               0 },
                             tt::gGameOverColor,
                             true);
            }
        }
    }
}

void
Board::clear()
{
    for (int y = 0; y < GRID_H; y++) {
        for (int x = 0; x < GRID_W; x++) {
            m_argrid[y][x] = NOTHING_IN_SQUARE;
        }
    }
}

void
Board::clearCurrentShape()
{
    for (int y = 0; y < GRID_H; y++) {
        for (int x = 0; x < GRID_W; x++) {
            if (m_arshape[y][x] == SHAPE_IN_SQUARE)
                m_arshape[y][x] = NOTHING_IN_SQUARE;
        }
    }
}

void
Board::findCurrentBottomShiftShape()
{
    for (int y = SHAPE_SIZE - 1; y >= 0; y--) {
        int firstLine = 0;
        for (int x = 0; x < SHAPE_SIZE; x++) {
            if (tt::gShapesArray[m_currentShape][m_currentShapeRotation]
                                [y * SHAPE_SIZE + x] > 0) {
                firstLine = 1;
            }
        }
        if (firstLine == 1) {
            m_currentBottomShiftShape = y;
            return;
        }
    }
}

void
Board::findCurrentRightShiftShape()
{
    for (int x = SHAPE_SIZE - 1; x >= 0; x--) {
        int firstCol = 0;
        for (int y = 0; y < SHAPE_SIZE; y++) {
            if (tt::gShapesArray[m_currentShape][m_currentShapeRotation]
                                [y * SHAPE_SIZE + x] > 0) {
                firstCol = 1;
            }
        }
        if (firstCol == 1) {
            m_currentRightShiftShape = x;
            return;
        }
    }
}

void
Board::findCurrentLeftShiftShape()
{
    for (int x = 0; x < SHAPE_SIZE; x++) {
        int firstCol = 0;
        for (int y = 0; y < SHAPE_SIZE; y++) {
            if (tt::gShapesArray[m_currentShape][m_currentShapeRotation]
                                [y * SHAPE_SIZE + x] > 0) {
                firstCol = 1;
            }
        }
        if (firstCol == 1) {
            m_currentLeftShiftShape = x;
            return;
        }
    }
}

bool
Board::checkIfCurrentBottomShiftShapeCollide()
{
    for (int y = SHAPE_SIZE - 1; y >= 0; y--) {
        for (int x = 0; x < SHAPE_SIZE; x++) {
            if (tt::gShapesArray[m_currentShape][m_currentShapeRotation]
                                [y * SHAPE_SIZE + x] > 0 &&
                m_argrid[m_currentShapeRow + y + 1][m_currentShapeCol + x] ==
                  SOMETHING_IN_SQUARE) {

                return true;
            }
        }
    }
    return false;
}

bool
Board::checkIfCurrentLeftShiftShapeCollide()
{
    for (int y = SHAPE_SIZE - 1; y >= 0; y--) {
        for (int x = 0; x < SHAPE_SIZE; x++) {
            if (tt::gShapesArray[m_currentShape][m_currentShapeRotation]
                                [y * SHAPE_SIZE + x] > 0 &&
                m_argrid[m_currentShapeRow + y][m_currentShapeCol + x - 1] ==
                  SOMETHING_IN_SQUARE) {
                return true;
            }
        }
    }
    return false;
}

bool
Board::checkIfCurrentRightShiftShapeCollide()
{
    for (int y = SHAPE_SIZE - 1; y >= 0; y--) {
        for (int x = 0; x < SHAPE_SIZE; x++) {
            if (tt::gShapesArray[m_currentShape][m_currentShapeRotation]
                                [y * SHAPE_SIZE + x] > 0 &&
                m_argrid[m_currentShapeRow + y][m_currentShapeCol + x + 1] ==
                  SOMETHING_IN_SQUARE) {

                return true;
            }
        }
    }
    return false;
}

void
Board::freezeCurrentShape()
{
    for (int y = 0; y < GRID_H; y++) {
        for (int x = 0; x < GRID_W; x++) {
            if (m_arshape[y][x] == SHAPE_IN_SQUARE) {
                m_argrid[y][x] = SOMETHING_IN_SQUARE;
            }
        }
    }
    clearCurrentShape();
}

bool
Board::removesFullLines()
{
    m_boolshouldWarp = false;
    memset(m_arlinesToRemove, 0, sizeof(m_arlinesToRemove));
    for (int y = GRID_H - 1; y >= 0; y--) {
        int fullLine = 1;
        for (int x = 0; x < GRID_W; x++) {
            if (m_argrid[y][x] == NOTHING_IN_SQUARE) {
                fullLine = 0;
                continue;
            }
        }
        if (fullLine) {
            m_arlinesToRemove[y] = 1;
        }
    }

    bool isLinesToRemove = false;

    // add lines and particles effects
    for (int i = 0; i < GRID_H; i++) {
        if (m_arlinesToRemove[i]) {
            isLinesToRemove = true;
            for (int x = 0; x < GRID_W; x++) {
                sf::Vector2f v =
                  sf::Vector2f(m_currentShiftLeft + x * PIXEL_SQUARE_SIZE,
                               m_currentShiftHeight + i * PIXEL_SQUARE_SIZE);
                m_particles.addParticles(v.x, v.y, 10, 1);
            }
        }
    }

    if (isLinesToRemove == true) {
        // Add a wait effect before
        m_waitClock.restart();
        m_egameState = scrollLine;
        m_currentLineExtension = 0.0f;
        m_currentLineExtensionStep =
          ((GRID_W + 2) * PIXEL_SQUARE_SIZE) /
          ((float)(LINE_TIME_MS) / (float)(FRAME_RATE));
    } else {
        // in case of AUTOPLAY authorize computer to play next shape since a
        // shape
        // was freezed before calling this function
        m_waitNextTurn = false;
    }

    return isLinesToRemove;
}

void
Board::scrollEverythingDown(int fromLine)
{
    for (int y = fromLine - 1; y >= 0; y--) {
        for (int x = 0; x < GRID_W; x++) {
            m_argrid[y + 1][x] = m_argrid[y][x];
        }
    }
}

void
Board::right()
{
    if (m_currentShapeCol >= GRID_W - m_currentRightShiftShape - 1)
        return;

    if (checkIfCurrentRightShiftShapeCollide())
        return;

    if (m_boolOnceMoveSound) {
        m_soundMove.play();
        m_boolOnceMoveSound = false;
    }

    m_currentShapeCol++;
    clearCurrentShape();
    setCurrentShape(m_currentShape,
                    m_currentShapeRow,
                    m_currentShapeCol,
                    m_currentShapeRotation);
    findCurrentRightShiftShape();
}

void
Board::left()
{
    if (m_currentShapeCol <= 0 - m_currentLeftShiftShape)
        return;

    if (checkIfCurrentLeftShiftShapeCollide())
        return;

    if (m_boolOnceMoveSound) {
        m_soundMove.play();
        m_boolOnceMoveSound = false;
    }
    m_currentShapeCol--;
    clearCurrentShape();
    setCurrentShape(m_currentShape,
                    m_currentShapeRow,
                    m_currentShapeCol,
                    m_currentShapeRotation);
    findCurrentLeftShiftShape();
}

void
Board::accelerate()
{
    m_ftimeMultiplier = QUICK_TIME_MULTIPLIER;
}

void
Board::normalSpeed()
{
    // m_ftimeMultiplier = NORMAL_TIME_MULTIPLIER - (m_ilevel * 0.2f);
    // m_ftimeMultiplier = getNextLevelMultiplier();
    m_ftimeMultiplier = getCurrentLevelMultiplier();
}

void
Board::warp()
{
    while (!checkIfCurrentBottomShiftShapeCollide() &&
           m_currentShapeRow < GRID_H - m_currentBottomShiftShape - 1)
        m_currentShapeRow++;

    m_soundWarp.play();

    // Clean current shape before setting it
    clearCurrentShape();

    setCurrentShape(m_currentShape,
                    m_currentShapeRow,
                    m_currentShapeCol,
                    m_currentShapeRotation);

    // Add effect
    tt::rect shapeRect = locateCurrentShape();
    m_fog.addFog(m_currentShiftLeft + shapeRect.x * PIXEL_SQUARE_SIZE,
                 m_currentShiftHeight + shapeRect.y * PIXEL_SQUARE_SIZE,
                 (float)shapeRect.w);
    for (int i = 0; i < shapeRect.w; i++) {
        m_particles.addParticles(
          m_currentShiftLeft + shapeRect.x * PIXEL_SQUARE_SIZE,
          m_currentShiftHeight + shapeRect.y * PIXEL_SQUARE_SIZE,
          2,
          2);
    }

    if (checkIfCurrentBottomShiftShapeCollide() ||
        (m_currentShapeRow >= GRID_H - m_currentBottomShiftShape - 1)) {
        m_iscore += 9;
        m_strscore = std::to_string(m_iscore);
        freezeCurrentShape();
        removesFullLines();
    }

    setCurrentShape(getNextShape(), 0, GRID_W / 2 - SHAPE_SIZE / 2, 0);
    findCurrentBottomShiftShape();
    m_ftimeMultiplier = getCurrentLevelMultiplier();
    m_boolAllowedTimeStarted = false;
    m_boolCanAccelerate = false;
}

void
Board::rotate()
{
    m_soundRotate.play();

    const int* rotatedShape =
      tt::gShapesArray[m_currentShape][(m_currentShapeRotation + 1) % 4];
    int squarecount = 0;

    for (int y = m_currentShapeRow; y < SHAPE_SIZE + m_currentShapeRow; y++) {
        for (int x = m_currentShapeCol; x < SHAPE_SIZE + m_currentShapeCol;
             x++) {
            if (y < GRID_H && x < GRID_W && rotatedShape[squarecount] == 1 &&
                m_argrid[y][x] == SOMETHING_IN_SQUARE) {
                return;
            }
            if (x > GRID_W - 1 && rotatedShape[squarecount] == 1) {
                return;
            }
            if (x < 0 && rotatedShape[squarecount] == 1) {
                return;
            }
            if (y >= GRID_H) {
                return;
            }
            squarecount++;
        }
    }

    clearCurrentShape();
    setCurrentShape(m_currentShape,
                    m_currentShapeRow,
                    m_currentShapeCol,
                    (++m_currentShapeRotation) % 4);
    findCurrentBottomShiftShape();
    findCurrentRightShiftShape();
    findCurrentLeftShiftShape();
}

int
Board::getNextShape()
{
    int shape = m_ishapesQueue[0];

    for (int i = 1; i < sizeof(m_ishapesQueue) / sizeof(int); i++) {
        m_ishapesQueue[i - 1] = m_ishapesQueue[i];
    }
    float s =
      static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 6.0f));
    int lastIndex = sizeof(m_ishapesQueue) / sizeof(int) - 1;
    m_ishapesQueue[lastIndex] = (int)round(s);

    return shape;
}

void
Board::drawNextShapes()
{
    int nextHeight = 0;
    int squareSize =
      (int)round(PIXEL_SQUARE_SIZE / SMALL_SHAPE_REDUCTION_FACTOR);
    for (int i = 0; i < sizeof(m_ishapesQueue) / sizeof(int); i++) {

        int shapeIndex = m_ishapesQueue[i];

        const int* shape = tt::gShapesArray[shapeIndex][0];
        int squarecount = 0;
        for (int y = 0; y < SHAPE_SIZE; y++) {
            for (int x = 0; x < SHAPE_SIZE; x++) {
                if (y < GRID_H && x < GRID_W && shape[squarecount] == 1) {
                    renderSmallSquare(
                      { (int)m_currentShiftLeft +
                          NEXT_SHAPES_SHIFT_LEFT_RELATIVE_TO_GRID +
                          x * squareSize,
                        (int)m_currentShiftHeight +
                          NEXT_SHAPES_SHIFT_HEIGHT_RELATIVE_TO_GRID +
                          nextHeight + y * squareSize,
                        0,
                        0 },
                      tt::gShapesColor[shapeIndex]);
                }
                squarecount++;
            }
        }
        nextHeight += squareSize * SHAPE_SIZE;
    }
}

void
Board::drawLevel()
{
    int heightIncrement = LEVEL_HEIGHT / LINES_PER_LEVEL;
    for (int i = 0; i < m_icountLinesPerLevel; i++) {
        sf::RectangleShape rectangle(
          sf::Vector2f(LEVEL_WIDTH, -(i + 1) * (float)heightIncrement));
        rectangle.setPosition(sf::Vector2f(
          m_currentShiftLeft + LEVEL_SHIFT_LEFT_RELATIVE_TO_GRID,
          m_currentShiftHeight + LEVEL_SHIFT_HEIGHT_RELATIVE_TO_GRID));
        rectangle.setFillColor(sf::Color(0, 255, 255, 255 / LINES_PER_LEVEL));
        rectangle.setOutlineColor(sf::Color(0, 255, 255, 255));
        rectangle.setOutlineThickness(1);
        m_prenderWindow->draw(rectangle);
    }
    if (m_boolparticlesLevel) {
        m_particles.addParticles(
          m_currentShiftLeft + LEVEL_SHIFT_LEFT_RELATIVE_TO_GRID,
          m_currentShiftHeight + LEVEL_SHIFT_HEIGHT_RELATIVE_TO_GRID -
            (m_icountLinesPerLevel + 1) * (float)heightIncrement,
          30,
          3);
        m_boolparticlesLevel = false;
    }
}

void
Board::drawScore()
{
    sf::Text scoreText(m_strscore, m_gameFont, SCORE_FONT_SIZE);
    float scoreWidth = scoreText.getLocalBounds().width;
    scoreText.setFillColor(sf::Color::Black);
    scoreText.setOutlineColor(sf::Color::White);
    scoreText.setOutlineThickness(0.8f);
    scoreText.setPosition(
      m_currentShiftLeft + SCORE_SHIFT_LEFT_RELATIVE_TO_GRID - scoreWidth,
      m_currentShiftHeight + SCORE_SHIFT_HEIGHT_RELATIVE_TO_GRID);
    m_prenderWindow->draw(scoreText);

    sf::Text lineText(m_strcountLines, m_gameFont, LINE_FONT_SIZE);
    float lineWidth = lineText.getLocalBounds().width;
    lineText.setFillColor(sf::Color::Black);
    lineText.setOutlineColor(sf::Color::White);
    lineText.setOutlineThickness(0.8f);
    lineText.setPosition(
      m_currentShiftLeft + LINE_SHIFT_LEFT_RELATIVE_TO_GRID - lineWidth,
      m_currentShiftHeight + LINE_SHIFT_HEIGHT_RELATIVE_TO_GRID);
    m_prenderWindow->draw(lineText);

    sf::Text levelText(m_strlevel, m_gameFont, LEVELF_FONT_SIZE);
    float levelWidth = levelText.getLocalBounds().width;
    levelText.setFillColor(sf::Color::Black);
    levelText.setOutlineColor(sf::Color::White);
    levelText.setOutlineThickness(0.8f);
    levelText.setPosition(
      m_currentShiftLeft + LEVELF_SHIFT_LEFT_RELATIVE_TO_GRID - levelWidth,
      m_currentShiftHeight + LEVELF_SHIFT_HEIGHT_RELATIVE_TO_GRID);
    m_prenderWindow->draw(levelText);
}

void
Board::updateGhost()
{
    for (int y = 0; y < GRID_H; y++) {
        for (int x = 0; x < GRID_W; x++) {
            if (m_arghost[y][x] == GHOST_IN_SQUARE) {
                m_arghost[y][x] = NOTHING_IN_SQUARE;
            }
        }
    }

    int row = 0;
    int maxRow = -1;

    for (int r = m_currentShapeRow; r < GRID_H - m_currentBottomShiftShape;
         r++) {
        bool fit = true;
        for (int y = SHAPE_SIZE - 1; y >= 0; y--) {
            for (int x = 0; x < SHAPE_SIZE; x++) {

                if (tt::gShapesArray[m_currentShape][m_currentShapeRotation]
                                    [y * SHAPE_SIZE + x] > 0 &&
                    m_argrid[r + y][m_currentShapeCol + x] ==
                      SOMETHING_IN_SQUARE) {
                    fit = false;
                }
            }
        }

        if (fit && r > maxRow) {
            maxRow = r;
        }

        if (!fit) {
            break;
        }
    }

    if (maxRow >= 0) {
        const int* shape =
          tt::gShapesArray[m_currentShape][m_currentShapeRotation];
        int squarecount = 0;
        for (int yy = maxRow; yy < SHAPE_SIZE + maxRow; yy++) {
            for (int xx = m_currentShapeCol;
                 xx < SHAPE_SIZE + m_currentShapeCol;
                 xx++) {
                if (yy < GRID_H && xx < GRID_W && shape[squarecount] == 1) {
                    m_arghost[yy][xx] = GHOST_IN_SQUARE;
                }

                squarecount++;
            }
        }
    }
}

void
Board::youLoose()
{
    sf::Text youLoose;
    youLoose.setString("You loose !");
    youLoose.setCharacterSize(30);
    youLoose.setFont(m_gameFont);
    sf::FloatRect fr = youLoose.getGlobalBounds();
    sf::FloatRect lr = youLoose.getLocalBounds();

    sf::RectangleShape box(sf::Vector2f(fr.width + 4 * PIXEL_SQUARE_SIZE,
                                        fr.height + 4 * PIXEL_SQUARE_SIZE));
    box.setPosition(WINDOW_W / 2 - box.getSize().x / 2,
                    WINDOW_H / 2 - box.getSize().y / 2 + fr.top);
    box.setFillColor(sf::Color(255, 255, 255, 128));
    box.setOutlineColor(sf::Color(255, 255, 255, 255));
    box.setOutlineThickness(2);
    m_prenderWindow->draw(box);

    youLoose.setPosition(WINDOW_W / 2 - fr.width / 2,
                         WINDOW_H / 2 - fr.height / 2);
    youLoose.setFillColor(sf::Color::Black);
    m_prenderWindow->draw(youLoose);
}

void
Board::playIntro()
{
    m_soundIntro.play();
}

void
Board::downSpecifics()
{
    if (m_boolAllowedTimeStarted) {
        if (checkIfCurrentBottomShiftShapeCollide() ||
            (m_currentShapeRow >= GRID_H - m_currentBottomShiftShape - 1)) {
            m_iscore += 8;
            m_strscore = std::to_string(m_iscore);

            freezeCurrentShape();
            removesFullLines();

            setCurrentShape(getNextShape(), 0, GRID_W / 2 - SHAPE_SIZE / 2, 0);
            findCurrentBottomShiftShape();
            // m_ftimeMultiplier = NORMAL_TIME_MULTIPLIER - (m_ilevel * 0.2f);
            // m_ftimeMultiplier = getNextLevelMultiplier();
            m_ftimeMultiplier = getCurrentLevelMultiplier();
            m_boolAllowedTimeStarted = false;
            m_boolCanAccelerate = false;
        }
    }
}

tt::rect
Board::locateCurrentShape()
{
    int xmin = GRID_W, ymin = GRID_H, wr = 0, hr = 0;
    int xmax = 0, ymax = 0;
    for (int y = GRID_H - 1; y >= 0; y--) {
        for (int x = GRID_W - 1; x >= 0; x--) {
            if (m_arshape[y][x] == SHAPE_IN_SQUARE) {
                if (y <= ymin) {
                    ymin = y;
                }
                if (x <= xmin) {
                    xmin = x;
                }
                if (y >= ymax /*&& ymin == GRID_H*/) {
                    ymax = y;
                }
                if (x >= xmax /*&& xmax == GRID_W*/) {
                    xmax = x;
                }
            }
        }
    }
    tt::rect rect = { xmin, ymin, xmax - xmin + 1, ymax - ymin + 1 };
    return rect;
}

float
Board::getNextLevelMultiplier()
{
    // 0,03 step = 28 levels
    float mult = (float)(1 - 0.03 * m_ilevel);
    if (mult <= 0.04f) {
        printf("TOP LEVEL\n");
        mult = 0.04f;
    }

    m_soundHurry.play();
    return mult;
}

float
Board::getCurrentLevelMultiplier()
{
    return (float)(1 - 0.03 * m_ilevel);
}

void
Board::iaMoveThread()
{
    if (!m_bisComputerMoving && !m_moveComputerThreadAllocated) {
        m_moveComputerThreadAllocated = true;
        m_moveComputerThread = new sf::Thread([this]() {
            m_bisComputerMoving = true;
            /*m_moveComputerThreadAllocated = true;*/
            Ia::searchCount = 0;
            Pos posIa = Ia::findBestPosition(m_argrid,
                                             m_ishapesQueue,
                                             m_currentShape,
                                             m_currentShapeRotation,
                                             m_currentBottomShiftShape,
                                             m_currentLeftShiftShape,
                                             m_currentRightShiftShape,
                                             m_currentShapeRow,
                                             m_currentShapeCol,
                                             AUTOPLAY_DEPTH,
                                             AUTOPLAY_DEPTH);
            printf("Evaluated positions:%d\n", Ia::searchCount);

            int leftCount = m_currentShapeCol - posIa.col;
            int rightCount = posIa.col - m_currentShapeCol;
            int rotateCount = abs(posIa.rotation - m_currentShapeRotation) % 4;
            printf("left:%d  right:%d   rotate:%d\n",
                   leftCount,
                   rightCount,
                   rotateCount);

            // Move shape with human clumsiness and slow speed
            if (rotateCount > 0) {
                for (int i = 0; i < rotateCount; i++) {
                    sf::sleep(
                      sf::milliseconds(SLEEP_TIME_BETWEEN_EVERY_MOVE_MS));
                    rotate();
                }
            }

            if (rightCount > 0) {
                m_boolOnceMoveSound = true;
                for (int i = 0; i < rightCount; i++) {
                    sf::sleep(
                      sf::milliseconds(SLEEP_TIME_BETWEEN_EVERY_MOVE_MS));
                    right();
                }
            } else if (leftCount > 0) {
                m_boolOnceMoveSound = true;
                for (int i = 0; i < leftCount; i++) {
                    sf::sleep(
                      sf::milliseconds(SLEEP_TIME_BETWEEN_EVERY_MOVE_MS));
                    left();
                }
            }

            sf::sleep(sf::milliseconds(SLEEP_TIME_BETWEEN_EVERY_MOVE_MS));
            warp();

            m_bisComputerMoving = false;
        });
        m_moveComputerThread->launch();
    }
}

void
Board::freeAutoplayThread()
{
    if (m_moveComputerThreadAllocated) {
        m_moveComputerThread->wait();
        delete m_moveComputerThread;
    }
}
