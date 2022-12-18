#pragma once

#include "constants.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Glsl.hpp>
#include <stdint.h>
#include <vector>

// define the width and height of the screen and the buffers
const int screenWidth = 640;
const int screenHeight = 100;

class Fire
{
  private:
    // Y-coordinate first because we use horizontal scanlines
    uint32_t fire[screenHeight][screenWidth] = {};

    // this buffer will contain the fire
    // uint32_t buffer[screenHeight][screenWidth];
    sf::Texture* bufferTexture = NULL;
    sf::Image* bufferImage = NULL;

    // this is the buffer to be
    // drawn to the screen
    sf::Color palette[256]; // this will contain the color palette
    int oldFrameCount = 0;
    int m_ilife = -1;
    bool readyForRemoving = false;
    uint32_t m_ix = 0, m_iy = 0, m_ih = screenHeight - 1, m_iw = screenWidth;
    float stepy = 0, middley = 0, middlex = 0, dx = 0, dxr = 0, alphay = 0;
    uint8_t alpha = 0;
    static std::vector<Fire*> allocatedFires;

  public:
    Fire();
    sf::Color hsv(int hue, float sat, float val);
    uint8_t getL(sf::Color rgb);
    static void addFire(uint32_t x, uint32_t y, uint32_t w, uint32_t h, int lifetime);
    void freeAllocatedTexture();
    static void freeExtinguishedFires();
    static void freeBurningFires();
    static void nextFrame(int& frameCount);
    static void render(sf::RenderWindow& window);
};
