#include "fire.h"
#include <math.h>

std::vector<Fire*> Fire::allocatedFires;

Fire::Fire()
{
    // generate the palette
    sf::Color color;
    for (int x = 0; x < 256; x++) {
        // color = hsv(x / 3, 255, (float)std::min(255, x * 2));
        color = sf::Color(0, x, std::min(255, x * 2));
        palette[x] = color;
    }
}

sf::Color
Fire::hsv(int hue, float sat, float val)
{
    // hue: 0-360°; sat: 0.f-1.f; val: 0.f-1.f

    hue %= 360;
    while (hue < 0)
        hue += 360;

    if (sat < 0.f)
        sat = 0.f;
    if (sat > 1.f)
        sat = 1.f;

    if (val < 0.f)
        val = 0.f;
    if (val > 1.f)
        val = 1.f;

    int h = hue / 60;
    float f = float(hue) / 60 - h;
    float p = val * (1.f - sat);
    float q = val * (1.f - sat * f);
    float t = val * (1.f - sat * (1 - f));

    switch (h) {
        default:
        case 0:
        case 6:
            return sf::Color(
              (uint8_t)(val * 255), (uint8_t)(t * 255), (uint8_t)(p * 255));
        case 1:
            return sf::Color(
              (uint8_t)(q * 255), (uint8_t)(val * 255), (uint8_t)(p * 255));
        case 2:
            return sf::Color(
              (uint8_t)(p * 255), (uint8_t)(val * 255), (uint8_t)(t * 255));
        case 3:
            return sf::Color(
              (uint8_t)(p * 255), (uint8_t)(q * 255), (uint8_t)(val * 255));
        case 4:
            return sf::Color(
              (uint8_t)(t * 255), (uint8_t)(p * 255), (uint8_t)(val * 255));
        case 5:
            return sf::Color(
              (uint8_t)(val * 255), (uint8_t)(p * 255), (uint8_t)(q * 255));
    }
}

uint8_t
Fire::getL(sf::Color rgb)
{
    return rgb.r > rgb.g ? (rgb.r > rgb.b ? rgb.r : rgb.b)
                         : (rgb.g > rgb.b ? rgb.g : rgb.b);
}

void
Fire::addFire(uint32_t x, uint32_t y, uint32_t w, uint32_t h, int lifetime)
{
    Fire* f = new Fire();
    f->m_ix = x;
    f->m_iy = y;
    f->m_iw = w;
    f->m_ih = h;

    for (uint32_t y = 0; y < f->m_ih; y++)
        for (uint32_t x = 0; x < f->m_iw; x++)
            f->fire[y][x] = 0;

    f->bufferTexture = new sf::Texture();
    f->bufferTexture->create(f->m_iw, f->m_ih);

    f->m_ilife = lifetime;
    f->readyForRemoving = false;

    allocatedFires.push_back(f);
}

void
Fire::freeAllocatedTexture()
{
    m_ilife = -1;
    delete bufferTexture;
}

void
Fire::freeExtinguishedFires()
{
    for (std::vector<Fire*>::iterator f = allocatedFires.begin();
         f != allocatedFires.end();
         f++) {
        if ((*f)->m_ilife < 0 && !(*f)->readyForRemoving) {
            (*f)->m_ilife = -1;
            (*f)->freeAllocatedTexture();
            (*f)->readyForRemoving = true;
        }
    }

    std::vector<Fire*> newAllocatedFires;
    for (std::vector<Fire*>::iterator f = allocatedFires.begin();
         f != allocatedFires.end();
         f++) {
        if (!(*f)->readyForRemoving)
            newAllocatedFires.push_back(*f);
    }

    allocatedFires.clear();
    allocatedFires = newAllocatedFires;
}

void
Fire::freeBurningFires()
{
    for (std::vector<Fire*>::iterator f = allocatedFires.begin();
         f != allocatedFires.end();
         f++) {
        if ((*f)->m_ilife >= 0 && !(*f)->readyForRemoving) {
            (*f)->m_ilife = -1;
            (*f)->freeAllocatedTexture();
            (*f)->readyForRemoving = true;
        }
    }

    std::vector<Fire*> newAllocatedFires;
    for (std::vector<Fire*>::iterator f = allocatedFires.begin();
         f != allocatedFires.end();
         f++) {
        if (!(*f)->readyForRemoving)
            newAllocatedFires.push_back(*f);
    }

    allocatedFires.clear();
    allocatedFires = newAllocatedFires;
}

void
Fire::nextFrame(int& frameCount)
{
    for (std::vector<Fire*>::iterator f = allocatedFires.begin();
         f != allocatedFires.end();
         f++) {

        if ((*f)->m_ilife < 0)
            continue;

        // bevelling sides
        int modh = (*f)->m_ih + 1;
        (*f)->bufferImage = new sf::Image();
        *(*f)->bufferImage = (*f)->bufferTexture->copyToImage();
        float dtr =
          -exp(abs(.5f * ((*f)->m_ilife - FIRE_TIME)) - FIRE_TIME / 2.0f) + 1;
        (*f)->middley = ((*f)->m_ih - 1) / 2.0f;
        (*f)->stepy = 255.0f / (*f)->m_ih;
        (*f)->middlex = ((*f)->m_iw - 1) / 2.0f;

        // compute fire only when it is alive
        if (dtr >= 0.0f) {

            // randomize the bottom row of the fire buffer
            for (uint32_t x = 0; x < (*f)->m_iw; x++)
                (*f)->fire[(*f)->m_ih][x] = abs(32768 + rand()) % 256;

            // yinc inc in every frame animation speed
            static const int yinc = 2;

            // do the fire calculations for every pixel, from top to bottom
            for (int i = 0; i < yinc; i++) {
                for (uint32_t y = 0; y < (*f)->m_ih /*- 1*/; y++) {
                    for (uint32_t x = 0; x < (*f)->m_iw; x++) {
                        (*f)->fire[y][x] =
                          (((*f)->fire[(y + 1) % modh]
                                      [(x - 1 + (*f)->m_iw) % (*f)->m_iw] +
                            (*f)->fire[(y + 1) % modh][(x) % (*f)->m_iw] +
                            (*f)->fire[(y + 1) % (*f)->m_ih]
                                      [(x + 1) % (*f)->m_iw] +
                            (*f)->fire[(y + 2) % modh][(x) % (*f)->m_iw]) *
                           32) /
                          129;
                    }
                }
            }

            // feed the buffered image with the previous buffer
            for (uint32_t y = 0; y < (*f)->m_ih; y++) {
                int yy = (int)(y - (*f)->middley);
                (*f)->alphay = -exp(abs(yy * .33f) - (*f)->middley / 3.0f) + 1;
                (*f)->alphay *= 255.0f;
                for (uint32_t x = 0; x < (*f)->m_iw; x++) {
                    int xx = (int)(x - (*f)->middlex);
                    (*f)->dx = -exp(abs(xx * .10f) - (*f)->middlex / 10.0f) + 1;
                    (*f)->dxr = 0.0f;
                    (*f)->alpha = (uint8_t)(dtr * (*f)->alphay * (*f)->dx);

                    sf::Color c((*f)->palette[(*f)->fire[y][x]].r,
                                (*f)->palette[(*f)->fire[y][x]].g,
                                (*f)->palette[(*f)->fire[y][x]].b,
                                (*f)->alpha);

                    if ((*f)->getL(c) < 32) {
                        // nothing when lum < 32
                    } else {
                        (*f)->bufferImage->setPixel(x, y, c);
                    }
                }
            }
        }

        (*f)->bufferTexture->loadFromImage(*(*f)->bufferImage);
        delete (*f)->bufferImage;

        (*f)->m_ilife--;
    }
}

void
Fire::render(sf::RenderWindow& window)
{
    for (std::vector<Fire*>::iterator f = allocatedFires.begin();
         f != allocatedFires.end();
         f++) {

        if ((*f)->m_ilife < 0)
            continue;

        sf::Sprite sprite(*(*f)->bufferTexture);
        sprite.setPosition((float)(*f)->m_ix, (float)(*f)->m_iy);
        window.draw(sprite);
    }
}
