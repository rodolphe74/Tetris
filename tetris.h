#pragma once

#include "animatedBackground.h"
#include "audioStream.h"
#include "board.h"
#include "particles.h"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

static sf::SoundBuffer soundBufferMainOne;
static sf::SoundBuffer soundBufferMainTwo;
static AudioStream audioStream;
static sf::ContextSettings contextSettings;
static sf::Event event;
static sf::Clock keyLatencyClock;
static sf::RenderWindow* window;
static Board* board;
static sf::Thread* musicThread;
static AnimatedBackground* background;
static Particles particles;

void
playThemeOne();

void
playThemeTwo();

void
playThemeThree();

void
redrawBoard(sf::RenderWindow& window,
            Board& board,
            Particles& particles,
            int& countFrames);

void
init();

void
freeAndExit();