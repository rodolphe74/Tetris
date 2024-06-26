#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Glsl.hpp>
#include "resources.h"

#define MAX_PARTICLES_COUNT 1000



class Particles
{

  private:
    int count = 0;
    sf::Glsl::Vec3* m_pvglarpos;
    sf::Glsl::Vec3* m_pvglarspeed;
    float* m_pfarlife;
    float* m_pfarrotation;
    sf::Color* m_parcolors;
    sf::Texture *m_asterTexturePtr;
    sf::Sprite *m_asterSpritePtr;

  public:
    Particles();
    ~Particles();
    int maxIndex = 0;
    void addParticles(float x, float y, int count, int colorScheme);
    void moveParticles();
    void moveParticles(float mult);
    void cleanParticlesArray();
    void renderParticles(sf::RenderWindow& window);
};
