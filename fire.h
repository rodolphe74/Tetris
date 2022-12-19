#pragma once

#include "constants.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Glsl.hpp>
#include <stdint.h>
#include <vector>

// define the width and height of the screen and the buffers
const int screenWidth = 640;
const int screenHeight = 96;

class Fire
{
  private:
    // Y-coordinate first because we use horizontal scanlines
    uint32_t* m_iarfire = NULL;

    // this buffer will contain the m_iarfire
    // uint32_t buffer[screenHeight][screenWidth];
    sf::Texture* m_bufferTexture = NULL;
    sf::Image* m_bufferImage = NULL;

    // this is the buffer to be
    // drawn to the screen
    sf::Color
      m_arcolorPalette[256]; // this will contain the color m_arcolorPalette
    int m_ilife = -1;
    bool m_boolreadyForRemoving = false;
    uint32_t m_ix = 0, m_iy = 0, m_ih = screenHeight - 1, m_iw = screenWidth;
    float m_fstepy = 0, m_fmiddley = 0, m_fmiddlex = 0, m_fdx = 0,
          m_falphay = 0;
    uint8_t m_ialpha = 0;
    static std::vector<Fire*> m_vecallocatedFires;
    inline uint32_t getOffset(uint32_t y, uint32_t x) { return m_iw * y + x; }

  public:
    Fire();
    sf::Color hsv(int hue, float sat, float val);
    uint8_t getL(sf::Color rgb);
    static void addFire(uint32_t x,
                        uint32_t y,
                        uint32_t w,
                        uint32_t h,
                        int lifetime);
    void freeAllocatedBufferAndTexture();
    static void freeExtinguishedFires();
    static void freeBurningFires();
    static void nextFrame(int& frameCount);
    static void render(sf::RenderWindow& window);
};
