#include "fire.h"
#include <math.h>

std::vector<Fire*> Fire::m_vecallocatedFires;

Fire::Fire()
{
    // generate the m_arcolorPalette
    sf::Color color;
    for (int x = 0; x < 256; x++) {
        // color = hsv(x / 3, 255, (float)std::min(255, x * 2));
        color = sf::Color(0, x, std::min(255, x * 2));
        m_arcolorPalette[x] = color;
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
    f->m_iarfire =
      new uint32_t[w * (h + 2)]; // +2 since iterations in buffer go further
                                 // than texture height in nextFrame m_iarfire
                                 // calculations loop

    for (uint32_t y = 0; y < f->m_ih; y++)
        for (uint32_t x = 0; x < f->m_iw; x++)
            f->m_iarfire[f->getOffset(y, x)] = 0;

    f->m_bufferTexture = new sf::Texture();
    f->m_bufferTexture->create(f->m_iw, f->m_ih);

    f->m_ilife = lifetime;
    f->m_boolreadyForRemoving = false;

    m_vecallocatedFires.push_back(f);
}

void
Fire::freeAllocatedBufferAndTexture()
{
    m_ilife = -1;
    delete m_bufferTexture;
    delete[] m_iarfire;
}

void
Fire::freeExtinguishedFires()
{
    for (std::vector<Fire*>::iterator f = m_vecallocatedFires.begin();
         f != m_vecallocatedFires.end();
         f++) {
        if ((*f)->m_ilife < 0 && !(*f)->m_boolreadyForRemoving) {
            (*f)->m_ilife = -1;
            (*f)->freeAllocatedBufferAndTexture();
            (*f)->m_boolreadyForRemoving = true;
        }
    }

    std::vector<Fire*> newAllocatedFires;
    for (std::vector<Fire*>::iterator f = m_vecallocatedFires.begin();
         f != m_vecallocatedFires.end();
         f++) {
        if (!(*f)->m_boolreadyForRemoving)
            newAllocatedFires.push_back(*f);
    }

    m_vecallocatedFires.clear();
    m_vecallocatedFires = newAllocatedFires;
}

void
Fire::freeBurningFires()
{
    for (std::vector<Fire*>::iterator f = m_vecallocatedFires.begin();
         f != m_vecallocatedFires.end();
         f++) {
        if ((*f)->m_ilife >= 0 && !(*f)->m_boolreadyForRemoving) {
            (*f)->m_ilife = -1;
            (*f)->freeAllocatedBufferAndTexture();
            (*f)->m_boolreadyForRemoving = true;
        }
    }

    std::vector<Fire*> newAllocatedFires;
    for (std::vector<Fire*>::iterator f = m_vecallocatedFires.begin();
         f != m_vecallocatedFires.end();
         f++) {
        if (!(*f)->m_boolreadyForRemoving)
            newAllocatedFires.push_back(*f);
    }

    m_vecallocatedFires.clear();
    m_vecallocatedFires = newAllocatedFires;
}

void
Fire::nextFrame(int& frameCount)
{
    for (std::vector<Fire*>::iterator f = m_vecallocatedFires.begin();
         f != m_vecallocatedFires.end();
         f++) {

        if ((*f)->m_ilife < 0)
            continue;

        // bevelling sides
        int modh = (*f)->m_ih + 1;
        (*f)->m_bufferImage = new sf::Image();
        *(*f)->m_bufferImage = (*f)->m_bufferTexture->copyToImage();
        float dtr =
          -exp(abs(.5f * ((*f)->m_ilife - FIRE_TIME)) - FIRE_TIME / 2.0f) + 1;
        (*f)->m_fmiddley = ((*f)->m_ih - 1) / 2.0f;
        (*f)->m_fstepy = 255.0f / (*f)->m_ih;
        (*f)->m_fmiddlex = ((*f)->m_iw - 1) / 2.0f;

        // compute m_iarfire only when it is alive
        if (dtr >= 0.0f) {

            // randomize the bottom row of the m_iarfire buffer
            for (uint32_t x = 0; x < (*f)->m_iw; x++)
                (*f)->m_iarfire[(*f)->getOffset((*f)->m_ih, x)] =
                  abs(32768 + rand()) % 256;

            // yinc inc in every frame animation speed
            static const int yinc = 2;

            // do the m_iarfire calculations for every pixel, from top to bottom
            for (int i = 0; i < yinc; i++) {
                for (uint32_t y = 0; y < (*f)->m_ih; y++) {
                    for (uint32_t x = 0; x < (*f)->m_iw; x++) {
                        (*f)->m_iarfire[(*f)->getOffset(y, x)] =
                          (((*f)->m_iarfire[(*f)->getOffset(
                              (y + 1) % modh,
                              (x - 1 + (*f)->m_iw) % (*f)->m_iw)] +
                            (*f)->m_iarfire[(*f)->getOffset((y + 1) % modh,
                                                            (x) % (*f)->m_iw)] +
                            (*f)->m_iarfire[(*f)->getOffset(
                              (y + 1) % (*f)->m_ih, (x + 1) % (*f)->m_iw)] +
                            (*f)->m_iarfire[(*f)->getOffset(
                              (y + 2) % modh, (x) % (*f)->m_iw)]) *
                           32) /
                          129;
                    }
                }
            }

            // feed the buffered image with the previous buffer
            for (uint32_t y = 0; y < (*f)->m_ih; y++) {
                int yy = (int)(y - (*f)->m_fmiddley);
                (*f)->m_falphay =
                  -exp(abs(yy * .33f) - (*f)->m_fmiddley / 3.0f) + 1;
                (*f)->m_falphay *= 255.0f;
                for (uint32_t x = 0; x < (*f)->m_iw; x++) {
                    int xx = (int)(x - (*f)->m_fmiddlex);
                    (*f)->m_fdx =
                      -exp(abs(xx * .10f) - (*f)->m_fmiddlex / 10.0f) + 1;
                    (*f)->m_ialpha =
                      (uint8_t)(dtr * (*f)->m_falphay * (*f)->m_fdx);

                    sf::Color c(
                      (*f)
                        ->m_arcolorPalette[(*f)
                                             ->m_iarfire[(*f)->getOffset(y, x)]]
                        .r,
                      (*f)
                        ->m_arcolorPalette[(*f)
                                             ->m_iarfire[(*f)->getOffset(y, x)]]
                        .g,
                      (*f)
                        ->m_arcolorPalette[(*f)
                                             ->m_iarfire[(*f)->getOffset(y, x)]]
                        .b,
                      (*f)->m_ialpha);

                    if ((*f)->getL(c) < 32) {
                        // nothing when lum < 32
                    } else {
                        (*f)->m_bufferImage->setPixel(x, y, c);
                    }
                }
            }
        }

        (*f)->m_bufferTexture->loadFromImage(*(*f)->m_bufferImage);
        delete (*f)->m_bufferImage;

        (*f)->m_ilife--;
    }
}

void
Fire::render(sf::RenderWindow& window)
{
    for (std::vector<Fire*>::iterator f = m_vecallocatedFires.begin();
         f != m_vecallocatedFires.end();
         f++) {

        if ((*f)->m_ilife < 0)
            continue;

        sf::Sprite sprite(*(*f)->m_bufferTexture);
        sprite.setPosition((float)(*f)->m_ix, (float)(*f)->m_iy);
        window.draw(sprite);
    }
}
