#include "tetris.h"
#include "animatedBackground.h"
#include "constants.h"

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

    board = new Board(*window);
    board->clear();
    board->setCurrentShape(
      board->getNextShape(), 0, GRID_W / 2 - SHAPE_SIZE / 2, 0);
    board->findCurrentBottomShiftShape();
    board->findCurrentLeftShiftShape();
    board->findCurrentRightShiftShape();
    board->m_egameState = scrollDown;
    board->normalSpeed();

    background = new AnimatedBackground(*window);

    menu = new Menu(*window);
    menu->addItem({ "New game", 1, sf::Keyboard::F1 });
    menu->addItem({ "Exit", 3, sf::Keyboard::F2 });
}

void
freeAndExit()
{
    stopCurrentAudioStream();
    if (introThread != NULL)
        delete introThread;

    if (musicThread != NULL)
        delete musicThread;

    delete menu;
    delete background;
    delete board;
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
    if (gameMode == MENU) {
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
                        board->m_boolOnceMoveSound = true;
                    menu->m_boolcanSound = true;
                    break;

                case sf::Event::Resized:
                    glViewport(0, 0, event.size.width, event.size.height);
                    break;

                default:
                    break;
            }
        }

        // background->renderBackground(models.at(randomModel) + 1,
        //                              (const size_t)models.at(randomModel)[0],
        //                              materials.at(randomMaterial));

        int model = board->m_ilevel % models.size();
        int material = board->m_ilevel % materials.size();
        background->renderBackground(models.at(model) + 1,
                                     (const size_t)models.at(model)[0],
                                     materials.at(material));

        // draw menu or board
        window->pushGLStates();

        if (gameMode == MENU) {
            item = menu->checkKeyboard();
            if (item == 1) {
                gameMode = NEW_GAME;
                stopCurrentAudioStream();
                delete musicThread;
                musicThread = NULL;
                introThread = new sf::Thread(&gameIntro);
                introThread->launch();
                newGameAnimWait.restart();
            } else if (item == 3) {
                gameMode = EXIT;
            }
            menu->render();
        }

        if (gameMode == GAME) {
            board->checkKeyboard();
            board->render(WINDOW_W / 2 - (GRID_W * PIXEL_SQUARE_SIZE) / 2,
                          WINDOW_H / 2 - (GRID_H * PIXEL_SQUARE_SIZE) / 2,
                          countFrames,
                          FRAME_RATE);
        }

        if (gameMode == NEW_GAME) {
            board->m_egameState = none;
            board->render(WINDOW_W / 2 - (GRID_W * PIXEL_SQUARE_SIZE) / 2,
                          WINDOW_H / 2 - (GRID_H * PIXEL_SQUARE_SIZE) / 2,
                          countFrames,
                          FRAME_RATE);

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
            printf("%d\n", newGameAnimWait.getElapsedTime().asMilliseconds());
            if (newGameAnimWait.getElapsedTime().asMilliseconds() >= 4000) {
                delete introThread;
                introThread = NULL;
                musicThread = new sf::Thread(&playThemeOne);
                musicThread->launch();
                board->m_egameState = scrollDown;
                gameMode = GAME;
            }
        }

        if (gameMode == EXIT) {
            break;
        }

        window->display();
        window->popGLStates();

        countFrames++;
    }

    freeAndExit();

    return 0;
}
