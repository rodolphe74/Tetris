#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Glsl.hpp>

class Fog
{

  private:
    int count = 0;
    sf::Glsl::Vec3 m_pvglpos;
    float m_pflife;
    int m_iWidth;
    sf::Texture m_fogTexture;
    sf::Sprite *m_fogSprite = NULL;

    float m_frxOne, m_fryOne;
    float m_frxTwo, m_fryTwo;
    float m_frxThree, m_fryThree;
    float m_frxFour, m_fryFour;
    sf::Sprite m_fogSpriteOne;
    sf::Sprite m_fogSpriteTwo;
    sf::Sprite m_fogSpriteThree;
    sf::Sprite m_fogSpriteFour;

  public:
    Fog();
    ~Fog();
    int maxIndex = 0;
    void addFog(float x, float y, float z);
    void moveFog();
    void renderFog(sf::RenderWindow& window);
};
