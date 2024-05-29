#pragma once

// clang-format off
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <array>

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include <glm/glm.hpp>
#include "shapes3d.h"
#include "constants.h"
// clang-format on

class AnimatedBackground
{
  private:
    sf::RenderWindow* m_prenderWindow;
    float m_fcountAngle;
    float m_fa;
    

  public:
    float m_unPeuDeRhythme = .0f;
    AnimatedBackground(sf::RenderWindow& window)
    {
        m_prenderWindow = &window;
        m_fcountAngle = 0.0f;
        m_fa = 0.0f;
    }
    void renderBackground(const float* shape, const size_t size, const float *material);
};
