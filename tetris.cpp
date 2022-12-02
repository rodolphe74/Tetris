#include "tetris.h"
#include "animatedBackground.h"
#include "constants.h"

void
playThemeOne()
{
    audioStream.load(soundBufferMainOne);
    audioStream.play();
    while (audioStream.isPlaying && audioStream.getStatus() == AudioStream::Playing) {
        sf::sleep(sf::seconds(0.2f));
    }
}

void
playThemeTwo()
{
    audioStream.load(soundBufferMainTwo);
    audioStream.play();
    while (audioStream.isPlaying && audioStream.getStatus() == AudioStream::Playing) {
        sf::sleep(sf::seconds(0.2f));
    }
}


void
redrawBoard(sf::RenderWindow& window,
            Board& board,
            Particles& particles,
            int& countFrames)
{
    window.pushGLStates();

    board.render(WINDOW_W / 2 - (GRID_W * PIXEL_SQUARE_SIZE) / 2,
                 WINDOW_H / 2 - (GRID_H * PIXEL_SQUARE_SIZE) / 2,
                 countFrames,
                 FRAME_RATE);

    window.popGLStates();
    window.display();
}

void
init()
{
    soundBufferMainOne.loadFromFile("tetris99main.ogg");
    soundBufferMainTwo.loadFromFile("tetris99main2.ogg");

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
}

void
freeAndExit()
{
    audioStream.stop();
    delete musicThread;
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
    bool shouldRotate = 0;
    bool shouldWarp = 0;

    musicThread = new sf::Thread(&playThemeOne);
    musicThread->launch();

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
                    break;

                case sf::Event::Resized:
                    glViewport(0, 0, event.size.width, event.size.height);
                    break;

                default:
                    break;
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) &&
            keyLatencyClock.getElapsedTime().asMilliseconds() >=
              REPEAT_KEYBOARD_LATENCY_MS &&
            board->m_egameState != none) {
            board->left();
            keyLatencyClock.restart();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) &&
            keyLatencyClock.getElapsedTime().asMilliseconds() >=
              REPEAT_KEYBOARD_LATENCY_MS &&
            board->m_egameState != none) {
            board->right();
            keyLatencyClock.restart();
        }

        if (shouldRotate && sf::Keyboard::isKeyPressed(sf::Keyboard::Up) &&
            board->m_egameState != none) {
            board->rotate();
            shouldRotate = false;
        }

        if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !shouldRotate &&
            board->m_egameState != none) {
            shouldRotate = true;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) &&
            board->m_boolCanAccelerate && board->m_egameState != none) {
            board->accelerate();
            board->downSpecifics();
        }
        if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Down) &&
            !board->m_boolCanAccelerate && board->m_egameState != none) {
            board->m_boolCanAccelerate = true;
        }
        if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Down) &&
            board->m_egameState != none) {
            board->normalSpeed();
        }

        if (shouldWarp && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) &&
            board->m_egameState != none) {
            board->warp();
            shouldWarp = false;
        }
        if (!sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) &&
            !shouldWarp && board->m_egameState != none) {
            shouldWarp = true;
        }

        background->m_unPeuDeRhythme = audioStream.m_fpeakAmp;
        background->renderBackground(models.at(randomModel) + 1,
                                     (const size_t)models.at(randomModel)[0],
                                     materials.at(randomMaterial));

        redrawBoard(*window, *board, particles, countFrames);

        countFrames++;
    }

    freeAndExit();

    return 0;
}
