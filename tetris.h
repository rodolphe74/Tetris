#pragma once

#include "animatedBackground.h"
#include "audioStream.h"
#include "board.h"
#include "menu.h"
#include "particles.h"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <queue>

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

static menuEnum menuStep = MENU;
static gameModeEnum gameMode = WAIT_SELECTION;
static sf::SoundBuffer soundBufferMainOne;
static sf::SoundBuffer soundBufferMainTwo;
static sf::SoundBuffer soundBufferMainThree;

static sf::SoundBuffer soundBufferOne;
static sf::SoundBuffer soundBufferTwo;
static sf::SoundBuffer soundBufferThree;
static sf::SoundBuffer soundBufferGo;
static sf::SoundBuffer soundBufferReady;
static sf::Sound soundOne;
static sf::Sound soundTwo;
static sf::Sound soundThree;
static sf::Sound soundReady;
static sf::Sound soundGo;

static sf::Clock newGameAnimWait;
static int currentPlayingTheme = -1;
static sf::Thread* musicThread = NULL;
static sf::Thread* introThread = NULL;
static sf::ContextSettings contextSettings;
static sf::Event event;
static sf::RenderWindow* window;
static Board* humanBoard = NULL;
static Board* computerBoard = NULL;
static AnimatedBackground* background;
static Particles particles;
static std::queue<ParticlePos> particlesQueue;
static Menu* menu;
static sf::Font gameFont;
static int counter = -1;
static int model;
static int material;
static bool waitKeyReleased = true;
static bool pausedGame = false;
static sf::Vector2<int> windowPosition;

void
playThemeOne();

void
playThemeTwo();

void
playThemeThree();

void
stopCurrentAudioStream();

void
gameIntro();

void
pauseGame();

void
unpauseGame();

void
init();

void
freeAndExit();

void
newGameIntroOrchestration();

void
newGameIntroRender(int& countFrames);

void
addParticlesToSendLineFromComputer(int arlinesRemoved[GRID_H]);

void
addParticlesToSendLineFromHuman(int arlinesRemoved[GRID_H]);