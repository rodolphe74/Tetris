#include "tetris.h"
#include "animatedBackground.h"
#include "constants.h"
#include "fire.h"

void
playThemeOne()
{
    AudioStream audioStream;
    audioStream.load(soundBufferMainOne);
    audioStream.play();
    currentPlayingTheme = 1;
    audioStream.isPlaying = true;
    while (currentPlayingTheme == 1 &&
           audioStream.getStatus() == AudioStream::Playing) {
        sf::sleep(sf::seconds(0.1f));
        background->m_unPeuDeRhythme = audioStream.m_fpeakAmp;
    }
}

void
playThemeTwo()
{
    AudioStream audioStream;
    audioStream.load(soundBufferMainTwo);
    audioStream.play();
    currentPlayingTheme = 2;
    audioStream.isPlaying = true;
    while (currentPlayingTheme == 2 &&
           audioStream.getStatus() == AudioStream::Playing) {
        sf::sleep(sf::seconds(0.1f));
        background->m_unPeuDeRhythme = audioStream.m_fpeakAmp;
    }
    audioStream.isPlaying = false;
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

    humanBoard = new Board(*window);
    humanBoard->clear();
    humanBoard->setCurrentShape(
      humanBoard->getNextShape(), 0, GRID_W / 2 - SHAPE_SIZE / 2, 0);
    humanBoard->findCurrentBottomShiftShape();
    humanBoard->findCurrentLeftShiftShape();
    humanBoard->findCurrentRightShiftShape();
    humanBoard->m_equeueGameStates.pushBack(none);
    humanBoard->normalSpeed();
    humanBoard->m_icurrentDepth = AUTOPLAY_DEPTH;

    // not always used
    computerBoard = new Board(*window);
    computerBoard->clear();
    computerBoard->setCurrentShape(
      computerBoard->getNextShape(), 0, GRID_W / 2 - SHAPE_SIZE / 2, 0);
    computerBoard->findCurrentBottomShiftShape();
    computerBoard->findCurrentLeftShiftShape();
    computerBoard->findCurrentRightShiftShape();
    humanBoard->m_equeueGameStates.pushBack(none);
    computerBoard->normalSpeed();
    computerBoard->m_icurrentDepth = AUTOPLAY_DEPTH;

    background = new AnimatedBackground(*window);

    menu = new Menu(*window);
    menu->addItem({ "Human alone", 1, sf::Keyboard::F1 });
    menu->addItem({ "Human vs computer", 2, sf::Keyboard::F2 });
    menu->addItem({ "Exit", 3, sf::Keyboard::F2 });
}

void
freeAndExit()
{
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
                    break;

                case sf::Event::KeyReleased:
                    if (event.key.code == sf::Keyboard::Left ||
                        event.key.code == sf::Keyboard::Right)
                        humanBoard->m_boolOnceMoveSound = true;
                    menu->m_boolcanSound = true;

                    //if (event.key.code == sf::Keyboard::F) {
                    //    Fire::addFire(10, 10, 100, 32, FIRE_TIME);
                    //}
                    break;

                case sf::Event::Resized:
                    glViewport(0, 0, event.size.width, event.size.height);
                    break;

                default:
                    break;
            }
        }

        int model = humanBoard->m_ilevel % models.size();
        int material = humanBoard->m_ilevel % materials.size();
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
                menuStep = EXIT;
            }
            menu->render();
        }

        if (menuStep == SELECTION_MADE) {

            // Board render && keyboard check game loop
            // Two cases : only human or computer vs human
            if (gameMode == GAME_HUMAN_ALONE) {
                humanBoard->m_boolAutoplay = false;
                humanBoard->checkKeyboard();
                humanBoard->render(
                  WINDOW_W / 2 - (GRID_W * PIXEL_SQUARE_SIZE) / 2,
                  WINDOW_H / 2 - (GRID_H * PIXEL_SQUARE_SIZE) / 2,
                  countFrames,
                  FRAME_RATE);

                //// Check m_iarfire deallocation
                //Fire::freeExtinguishedFires();

                //// Render m_iarfire if needed
                //Fire::nextFrame(countFrames);
                //Fire::render(*window);

                // gameover ?
                if (humanBoard->m_ecurrentGameState == gameOver) {
                    // Keeping only gameOver gameState in the queue
                    humanBoard->m_equeueGameStates.clear();
                    humanBoard->m_equeueGameStates.pushFront(gameOver);
                    humanBoard->looser(true);
                }

                // queued thread to dealloc ?
                humanBoard->m_equeueGameStates.sweepFinishedThreads();
            } else {
                // GAME_HUMAN_VS_COMPUTER
                humanBoard->m_boolAutoplay = false /*true*/;
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

                //// Check m_iarfire deallocation
                //Fire::freeExtinguishedFires();

                //// Render m_iarfire if needed
                //Fire::nextFrame(countFrames);
                //Fire::render(*window);

                // send lines to the opponent in 2 players mode ?
                if (humanBoard->m_icountScrolledDown > 1) {
                    int upCount = humanBoard->m_icountScrolledDown - 1;
                    humanBoard->m_icountScrolledDown = 0;
                    printf("##################### send lines to computer :%d "
                           "#####################\n",
                           humanBoard->m_icountScrolledDown - 1);
                    computerBoard->receiveLinesFromOpponent(upCount);
                }
                if (computerBoard->m_icountScrolledDown > 1) {
                    int upCount = computerBoard->m_icountScrolledDown - 1;
                    computerBoard->m_icountScrolledDown = 0;
                    printf("##################### send lines to human :%d "
                           "#####################\n",
                           computerBoard->m_icountScrolledDown - 1);
                    humanBoard->receiveLinesFromOpponent(upCount);
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
            }
        }

        if (menuStep == NEW_GAME) {
            newGameIntroRender(gameMode == GAME_HUMAN_ALONE ? false : true,
                               countFrames);
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
newGameIntroRender(bool isComputerPlaying, int& countFrames)
{

    if (gameMode == GAME_HUMAN_ALONE) {
        humanBoard->m_equeueGameStates.pushBack(none);
        humanBoard->m_waitNextTurn = true;
        humanBoard->render(WINDOW_W / 2 - (GRID_W * PIXEL_SQUARE_SIZE) / 2,
                           WINDOW_H / 2 - (GRID_H * PIXEL_SQUARE_SIZE) / 2,
                           countFrames,
                           FRAME_RATE);
    } else {
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
        musicThread = new sf::Thread(&playThemeOne);
        musicThread->launch();

        humanBoard->m_equeueGameStates.pushBack(scrollDown);
        humanBoard->m_waitNextTurn = false;

        if (gameMode == GAME_HUMAN_VS_COMPUTER) {
            computerBoard->m_equeueGameStates.pushBack(scrollDown);
            computerBoard->m_waitNextTurn = false;
        }

        menuStep = SELECTION_MADE;
    }
}
