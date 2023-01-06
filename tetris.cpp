#include "tetris.h"
#include "animatedBackground.h"
#include "constants.h"
#include "fire.h"
#include "ia.h"

void
playThemeOne()
{
    AudioStream audioStream;
    audioStream.load(soundBufferMainOne);
    audioStream.play();
    audioStream.isPlaying = true;
    currentPlayingTheme = 1;
    while (currentPlayingTheme == 1 &&
           audioStream.getStatus() == AudioStream::Playing) {
        sf::sleep(sf::seconds(0.1f));
        background->m_unPeuDeRhythme = audioStream.m_fpeakAmp;
    }
    audioStream.isPlaying = false;
    isThemePlaying = false;
}

void
playThemeTwo()
{
    AudioStream audioStream;
    audioStream.load(soundBufferMainTwo);
    audioStream.play();
    audioStream.isPlaying = true;
    currentPlayingTheme = 2;
    while (currentPlayingTheme == 2 &&
           audioStream.getStatus() == AudioStream::Playing) {
        sf::sleep(sf::seconds(0.1f));
        background->m_unPeuDeRhythme = audioStream.m_fpeakAmp;
    }
    audioStream.isPlaying = false;
    isThemePlaying = false;
}

void
playThemeThree()
{
    AudioStream audioStream;
    audioStream.load(soundBufferMainThree);
    audioStream.play();
    currentPlayingTheme = 3;
    audioStream.isPlaying = true;
    while (currentPlayingTheme == 3 &&
           audioStream.getStatus() == AudioStream::Playing) {
        sf::sleep(sf::seconds(0.1f));
        background->m_unPeuDeRhythme = audioStream.m_fpeakAmp;
    }
    audioStream.isPlaying = false;
}

void
stopCurrentAudioStream()
{
    if (currentPlayingTheme > 0) {
        currentPlayingTheme = -1;
    }
}

void
gameIntro()
{
    counter = 4;
    soundReady.play();
    sf::sleep(sf::milliseconds(800));

    counter = 3;
    soundThree.play();
    sf::sleep(sf::milliseconds(800));

    counter = 2;
    soundTwo.play();
    sf::sleep(sf::milliseconds(800));

    counter = 1;
    soundOne.play();
    sf::sleep(sf::milliseconds(800));

    counter = 0;
    soundGo.play();
    sf::sleep(sf::milliseconds(800));

    counter = -1;
}

void
pauseGame()
{
    sf::Text t;
    t.setFont(gameFont);
    t.setCharacterSize(48);
    t.setString("Paused game!");
    t.rotate(-30);

    sf::FloatRect rect = t.getGlobalBounds();
    t.setPosition(WINDOW_W / 2 - rect.width / 2,
                  WINDOW_H / 2 - rect.height / 2);

    t.setFillColor(sf::Color::White);
    window->draw(t);

    humanBoard->m_equeueGameStates.pushFront(GameState::pause);
    computerBoard->m_equeueGameStates.pushFront(GameState::pause);
}

void
unpauseGame()
{
    windowPosition = window->getPosition();
    humanBoard->m_equeueGameStates.purgeQueueFromState(GameState::pause);
    computerBoard->m_equeueGameStates.purgeQueueFromState(GameState::pause);
    humanBoard->m_equeueGameStates.pushBack(scrollDown);
    computerBoard->m_equeueGameStates.pushBack(scrollDown);
}

void
init()
{
    if (!gameFont.loadFromFile("resources/Brick3DRegular-nRJR4.ttf")) {
        perror("can't load font");
    }

    soundBufferMainOne.loadFromFile("resources/tetris99main.ogg");
    soundBufferMainTwo.loadFromFile("resources/tetris99main2.ogg");
    soundBufferMainThree.loadFromFile("resources/TDrift.ogg");

    soundBufferOne.loadFromFile("resources/one.ogg");
    soundOne.setBuffer(soundBufferOne);
    soundBufferTwo.loadFromFile("resources/two.ogg");
    soundTwo.setBuffer(soundBufferTwo);
    soundBufferThree.loadFromFile("resources/three.ogg");
    soundThree.setBuffer(soundBufferThree);
    soundBufferGo.loadFromFile("resources/ready.ogg");
    soundReady.setBuffer(soundBufferGo);
    soundBufferReady.loadFromFile("resources/go.ogg");
    soundGo.setBuffer(soundBufferReady);

    contextSettings.antialiasingLevel = 8;
    contextSettings.depthBits = 24;
    contextSettings.sRgbCapable = false;

    window = new sf::RenderWindow(sf::VideoMode(WINDOW_W, WINDOW_H),
                                  "Tetris",
                                  sf::Style::Titlebar | sf::Style::Close,
                                  contextSettings);

    window->setFramerateLimit(FRAME_RATE);
    window->setVerticalSyncEnabled(true);

    windowPosition = window->getPosition();

    humanBoard = new Board(*window);
    humanBoard->clear();
    humanBoard->setCurrentShape(
      humanBoard->getNextShape(), 0, GRID_W / 2 - SHAPE_SIZE / 2, 0);
    humanBoard->findCurrentBottomShiftShape();
    printf("Alt:%d\n", humanBoard->getAltitude());
    humanBoard->m_isearchDepth =
      AUTOPLAY_DEPTH_REGARDING_ALT[humanBoard->getAltitude()];
    humanBoard->findCurrentLeftShiftShape();
    humanBoard->findCurrentRightShiftShape();
    humanBoard->m_equeueGameStates.pushBack(none);
    humanBoard->normalSpeed();

    // not always used
    computerBoard = new Board(*window);
    computerBoard->clear();
    computerBoard->setCurrentShape(
      computerBoard->getNextShape(), 0, GRID_W / 2 - SHAPE_SIZE / 2, 0);
    computerBoard->findCurrentBottomShiftShape();
    printf("Alt:%d\n", computerBoard->getAltitude());
    computerBoard->m_isearchDepth =
      AUTOPLAY_DEPTH_REGARDING_ALT[humanBoard->getAltitude()];
    computerBoard->findCurrentLeftShiftShape();
    computerBoard->findCurrentRightShiftShape();
    computerBoard->m_equeueGameStates.pushBack(none);
    computerBoard->normalSpeed();

    background = new AnimatedBackground(*window);

    menu = new Menu(*window);
    menu->addItem({ "Human alone", 1, sf::Keyboard::F1 });
    menu->addItem({ "Human vs computer", 2, sf::Keyboard::F2 });
    menu->addItem({ "Computer alone", 3, sf::Keyboard::F3 });
    menu->addItem({ "Exit", 4, sf::Keyboard::F11 });
}

void
freeAndExit()
{
    Ia::m_boolsearching = false; // release recursivity search if needed

    Fire::freeBurningFires();
    Fire::freeExtinguishedFires();

    computerBoard->m_equeueGameStates.pushBack(none);
    computerBoard->freeAutoplayThread();
    stopCurrentAudioStream();

    if (introThread != NULL)
        delete introThread;

    if (musicThread != NULL)
        delete musicThread;

    delete menu;
    delete background;
    delete humanBoard;
    delete computerBoard;
    delete window;
}

int
main()
{
    init();

    int countFrames = 0;
    std::srand(
      (int)std::time(nullptr)); // use current time as seed for random generator
    int randomModel = std::rand() % models.size();
    int randomMaterial = std::rand() % materials.size();

    // DEBUG MENU
    int item = NO_ITEM_SELECTED;
    int mode = 0;

    // Start game
    if (menuStep == MENU) {
        isThemePlaying = true;
        currentPlayingTheme = 3;
        musicThread = new sf::Thread(&playThemeThree);
        musicThread->launch();
    }

    while (window->isOpen()) {

        while (window->pollEvent(event)) {
            switch (event.type) {

                case sf::Event::Closed:
                    window->close();
                    break;

                case sf::Event::KeyPressed:
                    // handle pause in game
                    if (event.key.code == sf::Keyboard::P && !pausedGame &&
                        waitKeyReleased) {
                        printf("PAUSE!");
                        pausedGame = true;
                    } else if (event.key.code == sf::Keyboard::P &&
                               pausedGame && waitKeyReleased) {
                        printf("UNPAUSE!");
                        pausedGame = false;
                        unpauseGame();
                    }
                    waitKeyReleased = false;

                    // DEBUG
                    if (event.key.code == sf::Keyboard::S) {
                        int ar[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                     0, 0, 0, 0, 0, 0, 1, 1, 1, 1 };
                        addParticlesToSendLineFromComputer(ar);
                    }
                    if (event.key.code == sf::Keyboard::D) {
                        int ar[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                     0, 0, 0, 0, 0, 0, 1, 1, 1, 1 };
                        addParticlesToSendLineFromHuman(ar);
                    }

                    break;

                case sf::Event::KeyReleased:
                    if (event.key.code == sf::Keyboard::Left ||
                        event.key.code == sf::Keyboard::Right) {
                        humanBoard->m_boolOnceMoveSound = true;
                        humanBoard->m_ikeyRepeatCount = 0;
                    }
                    menu->m_boolcanSound = true;
                    waitKeyReleased = true;
                    break;

                case sf::Event::Resized:
                    glViewport(0, 0, event.size.width, event.size.height);
                    break;

                case sf::Event::LostFocus:
                    if (menuStep == SELECTION_MADE &&
                        humanBoard->m_ecurrentGameState != gameOver &&
                        computerBoard->m_ecurrentGameState != gameOver) {
                        pausedGame = true;
                        waitKeyReleased = true;
                    }
                    break;

                default:
                    break;
            }
        }

        // Pause if window position change
        if (windowPosition != window->getPosition() &&
            menuStep == SELECTION_MADE &&
            humanBoard->m_ecurrentGameState != gameOver &&
            computerBoard->m_ecurrentGameState != gameOver) {
            pausedGame = true;
            waitKeyReleased = true;
        }

        int model, material;
        if (gameMode != GAME_COMPUTER_ALONE) {
            model = humanBoard->m_ilevel % models.size();
            material = humanBoard->m_ilevel % materials.size();
        } else {
            model = computerBoard->m_ilevel % models.size();
            material = computerBoard->m_ilevel % materials.size();
        }

        background->renderBackground(models.at(model) + 1,
                                     (const size_t)models.at(model)[0],
                                     materials.at(material));

        // draw menu or humanBoard
        window->pushGLStates();

        if (menuStep == MENU) {
            item = menu->checkKeyboard();
            if (item == 1) {
                menuStep = NEW_GAME;
                gameMode = GAME_HUMAN_ALONE;
                newGameIntroOrchestration();
            } else if (item == 2) {
                menuStep = NEW_GAME;
                gameMode = GAME_HUMAN_VS_COMPUTER;
                newGameIntroOrchestration();
            } else if (item == 3) {
                menuStep = NEW_GAME;
                gameMode = GAME_COMPUTER_ALONE;
                newGameIntroOrchestration();
            } else if (item == 4) {
                menuStep = EXIT;
            }
            menu->render();
        }

        if (menuStep == SELECTION_MADE) {

            // Board render && keyboard check game loop
            // Two cases : only human or computer vs human
            if (gameMode == GAME_HUMAN_ALONE) {
                humanBoard->m_boolAutoplay = false;
                if (!pausedGame)
                    humanBoard->checkKeyboard();
                humanBoard->render(
                  WINDOW_W / 2 - (GRID_W * PIXEL_SQUARE_SIZE) / 2,
                  WINDOW_H / 2 - (GRID_H * PIXEL_SQUARE_SIZE) / 2,
                  countFrames,
                  FRAME_RATE);

                // gameover ?
                if (humanBoard->m_ecurrentGameState == gameOver) {
                    // Keeping only gameOver gameState in the queue
                    humanBoard->m_equeueGameStates.clear();
                    humanBoard->m_equeueGameStates.pushFront(gameOver);
                    humanBoard->looser(true);
                }

                // queued thread to dealloc ?
                humanBoard->m_equeueGameStates.sweepFinishedThreads();
            } else if (gameMode == GAME_HUMAN_VS_COMPUTER) {
                // GAME_HUMAN_VS_COMPUTER
                humanBoard->m_boolAutoplay = false;
                if (!pausedGame)
                    humanBoard->checkKeyboard();
                humanBoard->render(
                  3 * (WINDOW_W / 4) - (GRID_W * PIXEL_SQUARE_SIZE) / 2,
                  WINDOW_H / 2 - (GRID_H * PIXEL_SQUARE_SIZE) / 2,
                  countFrames,
                  FRAME_RATE);

                computerBoard->m_boolAutoplay = true;
                computerBoard->render(
                  WINDOW_W / 4 - (GRID_W * PIXEL_SQUARE_SIZE) / 2,
                  WINDOW_H / 2 - (GRID_H * PIXEL_SQUARE_SIZE) / 2,
                  countFrames,
                  FRAME_RATE);

                // send lines to the opponent in 2 players mode ?
                if (humanBoard->m_icountScrolledDown > 1) {
                    int upCount = humanBoard->m_icountScrolledDown - 1;
                    humanBoard->m_icountScrolledDown = 0;
                    computerBoard->receiveLinesFromOpponent(upCount);
                    addParticlesToSendLineFromHuman(
                      humanBoard->m_arlinesToRemove);
                }
                if (computerBoard->m_icountScrolledDown > 1) {
                    int upCount = computerBoard->m_icountScrolledDown - 1;
                    computerBoard->m_icountScrolledDown = 0;
                    humanBoard->receiveLinesFromOpponent(upCount);
                    addParticlesToSendLineFromComputer(
                      computerBoard->m_arlinesToRemove);
                }

                // Check if one player is gameOver
                if (humanBoard->m_ecurrentGameState == gameOver) {
                    humanBoard->m_equeueGameStates.clear();
                    humanBoard->m_equeueGameStates.pushFront(gameOver);
                    computerBoard->m_equeueGameStates.clear();
                    computerBoard->m_equeueGameStates.pushFront(none);
                    humanBoard->looser(true);
                } else if (computerBoard->m_ecurrentGameState == gameOver) {
                    computerBoard->m_equeueGameStates.clear();
                    computerBoard->m_equeueGameStates.pushFront(gameOver);
                    humanBoard->m_equeueGameStates.clear();
                    humanBoard->m_equeueGameStates.pushFront(none);
                    computerBoard->looser(false);
                }

                // queued thread to dealloc ?
                humanBoard->m_equeueGameStates.sweepFinishedThreads();
                computerBoard->m_equeueGameStates.sweepFinishedThreads();
            } else if (gameMode == GAME_COMPUTER_ALONE) {
                // GAME_COMPUTER_ALONE
                computerBoard->m_boolAutoplay = true;
                computerBoard->render(
                  WINDOW_W / 2 - (GRID_W * PIXEL_SQUARE_SIZE) / 2,
                  WINDOW_H / 2 - (GRID_H * PIXEL_SQUARE_SIZE) / 2,
                  countFrames,
                  FRAME_RATE);

                // gameover ?
                if (computerBoard->m_ecurrentGameState == gameOver) {
                    // Keeping only gameOver gameState in the queue
                    computerBoard->m_equeueGameStates.clear();
                    computerBoard->m_equeueGameStates.pushFront(gameOver);
                    computerBoard->looser(true);
                }

                // queued thread to dealloc ?
                computerBoard->m_equeueGameStates.sweepFinishedThreads();
            }

            if (pausedGame) {
                pauseGame();
            }

            if (isThemePlaying == false && currentPlayingTheme == 1) {
                if (musicThread != NULL) {
                    delete musicThread;
                    musicThread = NULL;
                }
                isThemePlaying = true;
                currentPlayingTheme = 2;
                musicThread = new sf::Thread(&playThemeTwo);
                musicThread->launch();
            }

            if (isThemePlaying == false && currentPlayingTheme == 2) {
                if (musicThread != NULL) {
                    delete musicThread;
                    musicThread = NULL;
                }
                isThemePlaying = true;
                currentPlayingTheme = 1;
                musicThread = new sf::Thread(&playThemeOne);
                musicThread->launch();
            }

            // Particles to draw ?
            for (int i = 0; i < SEND_LINE_PARTICLES_SPEED; i++) {
                if (!particlesQueue.empty()) {
                    ParticlePos p = particlesQueue.front();
                    particlesQueue.pop();
                    particles.addParticles(p.x, p.y, p.count, p.colorScheme);
                }
            }
            particles.moveParticles(0.6f);
            particles.cleanParticlesArray();
            particles.renderParticles(*window);
        }

        if (menuStep == NEW_GAME) {
            newGameIntroRender(countFrames);
        }

        if (menuStep == EXIT) {
            break;
        }

        window->display();
        window->popGLStates();

        countFrames++;
    }

    freeAndExit();

    return 0;
}

void
newGameIntroOrchestration()
{
    stopCurrentAudioStream();
    delete musicThread;
    musicThread = NULL;
    introThread = new sf::Thread(&gameIntro);
    introThread->launch();
    newGameAnimWait.restart();
}

void
newGameIntroRender(int& countFrames)
{

    if (gameMode == GAME_HUMAN_ALONE) {
        humanBoard->m_equeueGameStates.pushBack(none);
        humanBoard->m_waitNextTurn = true;
        humanBoard->render(WINDOW_W / 2 - (GRID_W * PIXEL_SQUARE_SIZE) / 2,
                           WINDOW_H / 2 - (GRID_H * PIXEL_SQUARE_SIZE) / 2,
                           countFrames,
                           FRAME_RATE);
    } else if (gameMode == GAME_HUMAN_VS_COMPUTER) {
        // GAME_HUMAN_VS_COMPUTER
        humanBoard->m_equeueGameStates.pushBack(none);
        humanBoard->m_waitNextTurn = true;
        humanBoard->render(3 * (WINDOW_W / 4) -
                             (GRID_W * PIXEL_SQUARE_SIZE) / 2,
                           WINDOW_H / 2 - (GRID_H * PIXEL_SQUARE_SIZE) / 2,
                           countFrames,
                           FRAME_RATE);

        computerBoard->m_equeueGameStates.pushBack(none);
        computerBoard->m_waitNextTurn = true;
        computerBoard->render(WINDOW_W / 4 - (GRID_W * PIXEL_SQUARE_SIZE) / 2,
                              WINDOW_H / 2 - (GRID_H * PIXEL_SQUARE_SIZE) / 2,
                              countFrames,
                              FRAME_RATE);
    } else if (gameMode == GAME_COMPUTER_ALONE) {
        computerBoard->m_equeueGameStates.pushBack(none);
        computerBoard->m_waitNextTurn = true;
        computerBoard->render(WINDOW_W / 2 - (GRID_W * PIXEL_SQUARE_SIZE) / 2,
                              WINDOW_H / 2 - (GRID_H * PIXEL_SQUARE_SIZE) / 2,
                              countFrames,
                              FRAME_RATE);
    }

    sf::Text t;
    t.setFont(gameFont);
    t.setCharacterSize(48);
    if (counter == 4) {
        t.setString("Ready");
    } else if (counter == 3) {
        t.setString("3");
    } else if (counter == 2) {
        t.setString("2");
    } else if (counter == 1) {
        t.setString("1");
    } else if (counter == 0) {
        t.setString("Go !");
    }
    if (counter != -1) {
        sf::FloatRect rect = t.getLocalBounds();
        t.setPosition(WINDOW_W / 2 - rect.width / 2,
                      WINDOW_H / 2 - rect.height / 2);
        t.setFillColor(sf::Color::White);
        window->draw(t);
    }

    // Wait READY 3 2 1 GO before going into game mode
    if (newGameAnimWait.getElapsedTime().asMilliseconds() >= 4000) {
        delete introThread;
        introThread = NULL;
        isThemePlaying = true;
        currentPlayingTheme = 1;
        musicThread = new sf::Thread(&playThemeOne);
        musicThread->launch();

        humanBoard->m_equeueGameStates.pushBack(scrollDown);
        humanBoard->m_waitNextTurn = false;

        if (gameMode == GAME_HUMAN_VS_COMPUTER ||
            gameMode == GAME_COMPUTER_ALONE) {
            computerBoard->m_equeueGameStates.pushBack(scrollDown);
            computerBoard->m_waitNextTurn = false;
        }

        menuStep = SELECTION_MADE;
    }
}

void
addParticlesToSendLineFromComputer(int arlinesRemoved[GRID_H])
{
    float mult = 2;
    float xh = 3 * (WINDOW_W / 4);
    float xc = WINDOW_W / 4;
    float step = (xh - xc) / PIXEL_SQUARE_SIZE;
    step /= mult;
    int s = 0;
    for (float i = xc; i < xh; i += PIXEL_SQUARE_SIZE) {
        for (int h = 0; h < GRID_H; h++) {
            if (arlinesRemoved[h]) {
                float yh = (WINDOW_H / 2 - (GRID_H * PIXEL_SQUARE_SIZE) / 2) +
                           (GRID_H * PIXEL_SQUARE_SIZE) - PIXEL_SQUARE_SIZE;

                float yc = (WINDOW_H / 2 - (GRID_H * PIXEL_SQUARE_SIZE) / 2) +
                           ((h + 1.0f) * PIXEL_SQUARE_SIZE) -
                           PIXEL_SQUARE_SIZE / 2;
                sf::Vector2f vec = sf::Vector2f(xh - xc, yh - yc);
                float slope = yh - yc;
                float incy = slope / step;
                incy /= mult;
                ParticlePos p = { i, yc + s * incy, 10, 3 };
                particlesQueue.push(p);
            }
        }
        s++;
    }
}

void
addParticlesToSendLineFromHuman(int arlinesRemoved[GRID_H])
{
    float mult = 2;
    float xh = 3 * (WINDOW_W / 4);
    float xc = WINDOW_W / 4;
    float step = (xh - xc) / PIXEL_SQUARE_SIZE;
    step /= mult;
    int s = 0;
    for (float i = xh; i >= xc; i -= PIXEL_SQUARE_SIZE) {
        for (int h = 0; h < GRID_H; h++) {
            if (arlinesRemoved[h]) {

                float yh = (WINDOW_H / 2 - (GRID_H * PIXEL_SQUARE_SIZE) / 2) +
                           ((h + 1.0f) * PIXEL_SQUARE_SIZE) -
                           PIXEL_SQUARE_SIZE / 2;

                float yc = (WINDOW_H / 2 - (GRID_H * PIXEL_SQUARE_SIZE) / 2) +
                           (GRID_H * PIXEL_SQUARE_SIZE) - PIXEL_SQUARE_SIZE;
                sf::Vector2f vec = sf::Vector2f(xh - xc, yh - yc);
                float slope = yh - yc;
                float incy = slope / step;
                incy /= mult;
                ParticlePos p = { i, yh - s * incy, 10, 3 };
                particlesQueue.push(p);
            }
        }
        s++;
    }
}
