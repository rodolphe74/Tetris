#pragma once
#include "animatedBackground.h"
#include "audioStream.h"
#include "board.h"
#include "menu.h"
#include "particles.h"
#include "ia.h"
#include "fire.h"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <queue>



class game
{
    enum menuEnum
    {
        NEW_GAME,
        MENU,
        EXIT,
        SELECTION_MADE
    };

    enum gameModeEnum
    {
        WAIT_SELECTION,
        GAME_HUMAN_ALONE,
        GAME_HUMAN_VS_COMPUTER,
        GAME_COMPUTER_ALONE
    };

  private:
    menuEnum menuStep = MENU;
    gameModeEnum gameMode = WAIT_SELECTION;
    sf::SoundBuffer soundBufferMainOne;
    sf::SoundBuffer soundBufferMainTwo;
    sf::SoundBuffer soundBufferMainThree;

    sf::SoundBuffer soundBufferOne;
    sf::SoundBuffer soundBufferTwo;
    sf::SoundBuffer soundBufferThree;
    sf::SoundBuffer soundBufferGo;
    sf::SoundBuffer soundBufferReady;
    sf::Sound soundOne;
    sf::Sound soundTwo;
    sf::Sound soundThree;
    sf::Sound soundReady;
    sf::Sound soundGo;

    sf::Clock newGameAnimWait;
    int currentPlayingTheme = -1;
    sf::Thread* musicThread = NULL;
    sf::Thread* introThread = NULL;
    bool isThemePlaying = false;
    sf::ContextSettings contextSettings;
    sf::Event event;
    sf::RenderWindow* window;
    Board* humanBoard = NULL;
    Board* computerBoard = NULL;
    AnimatedBackground* background;
    Particles particles;
    std::queue<ParticlePos> particlesQueue;
    Menu* menu;
    sf::Font gameFont;
    int counter = -1;
    int model;
    int material;
    bool waitKeyReleased = true;
    bool pausedGame = false;
    sf::Vector2<int> windowPosition;
    int countFrames = 0;
    int item = NO_ITEM_SELECTED;

    void playThemeOne();
    void playThemeTwo();
    void playThemeThree();
    void stopCurrentAudioStream();
    
    
    void gameIntro();
    void pauseGame();
    void unpauseGame();
    void init();
    void newGameIntroOrchestration();
    void newGameIntroRender(int& countFrames);
    void addParticlesToSendLineFromComputer(int arlinesRemoved[GRID_H]);
    void addParticlesToSendLineFromHuman(int arlinesRemoved[GRID_H]);

  public:
    void beforeLoop();
    void loop();
    void freeAndExit();
};
