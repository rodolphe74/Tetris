#pragma once

#include "animatedBackground.h"
#include "audioStream.h"
#include "board.h"
#include "menu.h"
#include "particles.h"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

enum modeEnum
{
    GAME,
    NEW_GAME,
    MENU,
    EXIT
};

static modeEnum gameMode = MENU;
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
static Board* board;
static AnimatedBackground* background;
static Particles particles;
static Menu* menu;
static sf::Font gameFont;
static int counter = -1;

void
playThemeOne();

void
playThemeTwo();

void
playThemeThree();

void
stopCurrentAudioStream();

void
stopAutoplayThread();

void
gameIntro();

void
init();

void
freeAndExit();