#include "fog.h"
#include "constants.h"

#define FOG_TIME 8.0f

Fog::Fog()
{
    m_pvglpos = sf::Glsl::Vec3();
    m_pflife = 0.0f;
    m_fogSprite = NULL;
    m_iWidth = 1;

    if (!m_fogTexture.loadFromFile("warp.png")) {
        perror("can't load texture");
    }

    m_fogSpriteOne.setTexture(m_fogTexture);
    m_frxOne = PIXEL_SQUARE_SIZE / (float)m_fogTexture.getSize().x;
    m_fryOne = PIXEL_FOG_SIZE / (float)m_fogTexture.getSize().y;
    m_fogSpriteOne.setScale(m_frxOne, m_fryOne);

    m_fogSpriteTwo.setTexture(m_fogTexture);
    m_frxTwo = (PIXEL_SQUARE_SIZE * 2) / (float)m_fogTexture.getSize().x;
    m_fryTwo = PIXEL_FOG_SIZE / (float)m_fogTexture.getSize().y;
    m_fogSpriteTwo.setScale(m_frxTwo, m_fryTwo);

    m_fogSpriteThree.setTexture(m_fogTexture);
    m_frxThree = (PIXEL_SQUARE_SIZE * 3) / (float)m_fogTexture.getSize().x;
    m_fryThree = PIXEL_FOG_SIZE / (float)m_fogTexture.getSize().y;
    m_fogSpriteThree.setScale(m_frxThree, m_fryThree);

    m_fogSpriteFour.setTexture(m_fogTexture);
    m_frxFour = (PIXEL_SQUARE_SIZE * 4) / (float)m_fogTexture.getSize().x;
    m_fryFour = PIXEL_FOG_SIZE / (float)m_fogTexture.getSize().y;
    m_fogSpriteFour.setScale(m_frxFour, m_fryFour);
}

Fog::~Fog() {}

void
Fog::addFog(float x, float y, float w)
{
    m_pvglpos = sf::Glsl::Vec3(x, y, 0);
    m_pflife = FOG_TIME;
    m_iWidth = (int)w;

    if (m_iWidth == 1) {
        m_fogSpriteOne.setScale(m_frxOne, m_fryOne);
    } else if (m_iWidth == 2) {
        m_fogSpriteTwo.setScale(m_frxTwo, m_fryTwo);
    } else if (m_iWidth == 3) {
        m_fogSpriteThree.setScale(m_frxThree, m_fryThree);
    } else if (m_iWidth == 4) {
        m_fogSpriteFour.setScale(m_frxFour, m_fryFour);
    }
}

void
Fog::moveFog()
{
    m_pflife--;
}

void
Fog::renderFog(sf::RenderWindow& window)
{
    if (m_pflife > 0) {
        if (m_iWidth == 1) {
            m_fogSprite = &m_fogSpriteOne;
        } else if (m_iWidth == 2) {
            m_fogSprite = &m_fogSpriteTwo;
        } else if (m_iWidth == 3) {
            m_fogSprite = &m_fogSpriteThree;
        } else if (m_iWidth == 4) {
            m_fogSprite = &m_fogSpriteFour;
        }

        float transparency = (128.0f / FOG_TIME) * m_pflife;

        m_fogSprite->setColor(sf::Color(128, 128, 128, (int) transparency));
        m_fogSprite->scale(1.0f, 1.20f);
        m_fogSprite->setPosition(
          m_pvglpos.x,
          m_pvglpos.y - 4 * FOG_TIME * (FOG_TIME - m_pflife) - FOG_SHIFT_UP);
        window.draw(*m_fogSprite);
    }
}