#include "particles.h"
#include "constants.h"

Particles::Particles()
{
    m_pvglarpos = new sf::Glsl::Vec3[MAX_PARTICLES_COUNT];
    m_pvglarspeed = new sf::Glsl::Vec3[MAX_PARTICLES_COUNT];
    m_pfarlife = new float[MAX_PARTICLES_COUNT];
    m_pfarrotation = new float[MAX_PARTICLES_COUNT];
    m_parcolors = new sf::Color[MAX_PARTICLES_COUNT];

    for (int i = 0; i < MAX_PARTICLES_COUNT; i++) {
        m_pvglarspeed[i] = sf::Glsl::Vec3(0.0f, 0.0f, 0.0f);
        m_pfarlife[i] = -1.0f;
        m_pvglarpos[i] = sf::Glsl::Vec3(0.0f, 0.0f, 0.0f);
        m_pfarrotation[i] = 0.f;
    }
    maxIndex = 0;

    if (!m_asterTexture.loadFromFile("resources/asterw.png")) {
        perror("can't load texture");
    }
    m_asterTexture.setSmooth(true);
    m_asterSprite.setTexture(m_asterTexture);
    float rx = PIXEL_ASTER_SIZE / (float)m_asterTexture.getSize().x;
    float ry = PIXEL_ASTER_SIZE / (float)m_asterTexture.getSize().y;
    m_asterSprite.setScale(rx, ry);
}

Particles::~Particles()
{
    delete[] m_pvglarpos;
    delete[] m_pvglarspeed;
    delete[] m_pfarlife;
    delete[] m_pfarrotation;
}

void
Particles::addParticles(float x, float y, int count, int colorScheme)
{

    if (maxIndex + count > MAX_PARTICLES_COUNT) {
        return;
    }
    
    sf::Color sparkColors[3];
    if (colorScheme == 1) {
        sparkColors[0] = sf::Color(255, 0, 0);
        sparkColors[1] = sf::Color(255, 128, 0);
        sparkColors[2] = sf::Color(255, 255, 0);
    } else if (colorScheme == 2) {
        sparkColors[0] = sf::Color(255, 255, 255);
        sparkColors[1] = sf::Color(192, 192, 192);
        sparkColors[2] = sf::Color(128, 128, 128);
    } else if (colorScheme == 3) {
        sparkColors[0] = sf::Color(72, 151, 193);
        sparkColors[1] = sf::Color(95, 69, 214);
        sparkColors[2] = sf::Color(42, 31, 169);
    }

    for (int i = maxIndex; i < maxIndex + count; i++) {
        float sx =
          static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 60.0f));
        float sy =
          static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 60.0f));
        float signx =
          static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 10.0f));
        float signy =
          static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 10.0f));
        m_pvglarspeed[i] =
          sf::Glsl::Vec3(signx > 5.0f ? sx : -sx, signy > 5.0f ? sy : -sy, 0);
    }

    for (int i = maxIndex; i < maxIndex + count; i++) {
        float l =
          static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 50.0f));
        m_pfarlife[i] = l;
    }

    for (int i = maxIndex; i < maxIndex + count; i++) {
        m_pvglarpos[i] = sf::Glsl::Vec3(x, y, 0);
    }

    for (int i = maxIndex; i < maxIndex + count; i++) {
        m_pfarrotation[i] = 0.005f;
    }

    for (int i = maxIndex; i < maxIndex + count; i++) {
        m_parcolors[i] = sparkColors[i % 3];
    }

    maxIndex += count;
}

void
Particles::moveParticles()
{
    float delta = 1.0f;
    float deltaDivTwho = delta / 2.0f;
    for (int i = 0; i < maxIndex; i++) {

        if (m_pfarlife[i] > 0.0) {

            // Decrease m_pflife
            m_pfarlife[i] -= delta;
            if (m_pfarlife[i] > 0.0) {
                // printf("  dec %d = %f\n", i, m_pflife[i]);
                //  Simulate simple physics : gravity only, no collisions

                float g = static_cast<float>(rand()) /
                          (static_cast<float>(RAND_MAX / 1.0f));

                m_pvglarspeed[i] += sf::Glsl::Vec3(0.0f, g, 0.0f);
                m_pvglarspeed[i] *= deltaDivTwho;
                m_pvglarpos[i] += (m_pvglarspeed[i] * delta);

                float rotStep = static_cast<float>(rand()) /
                                (static_cast<float>(RAND_MAX / 10.0f));
                m_pfarrotation[i] += rotStep;
            }
        }
    }
}

void
Particles::moveParticles(float mult)
{
    float delta = 1.0f * mult;
    float deltaDivTwho = delta / 2.0f;
    for (int i = 0; i < maxIndex; i++) {

        if (m_pfarlife[i] > 0.0) {

            // Decrease m_pflife
            m_pfarlife[i] -= (delta / mult);
            if (m_pfarlife[i] > 0.0) {
                // printf("  dec %d = %f\n", i, m_pflife[i]);
                //  Simulate simple physics : gravity only, no collisions

                float g = static_cast<float>(rand()) /
                          (static_cast<float>(RAND_MAX / 1.0f));

                m_pvglarspeed[i] += sf::Glsl::Vec3(0.0f, g, 0.0f);
                m_pvglarspeed[i] *= deltaDivTwho;
                m_pvglarpos[i] += (m_pvglarspeed[i] * delta);

                float rotStep = static_cast<float>(rand()) /
                                (static_cast<float>(RAND_MAX / 10.0f));
                m_pfarrotation[i] += rotStep;
            }
        }
    }
}

void
Particles::cleanParticlesArray()
{
    sf::Glsl::Vec3* newpos = new sf::Glsl::Vec3[MAX_PARTICLES_COUNT];
    sf::Glsl::Vec3* newspeed = new sf::Glsl::Vec3[MAX_PARTICLES_COUNT];
    float* newlife = new float[MAX_PARTICLES_COUNT];
    float* newsize = new float[MAX_PARTICLES_COUNT];
    sf::Color* newcolor = new sf::Color[MAX_PARTICLES_COUNT];

    int idx = 0;
    for (int i = 0; i < maxIndex; i++) {
        if (m_pfarlife[i] > 0.0f) {
            newpos[idx] = m_pvglarpos[i];
            newspeed[idx] = m_pvglarspeed[i];
            newlife[idx] = m_pfarlife[i];
            newsize[idx] = m_pfarrotation[i];
            newcolor[idx] = m_parcolors[i];
            idx++;
        }
    }
    delete[] m_pvglarpos;
    delete[] m_pvglarspeed;
    delete[] m_pfarlife;
    delete[] m_pfarrotation;
    delete[] m_parcolors;

    m_pvglarpos = newpos;
    m_pvglarspeed = newspeed;
    m_pfarlife = newlife;
    m_pfarrotation = newsize;
    m_parcolors = newcolor;
    maxIndex = idx;
}

void
Particles::renderParticles(sf::RenderWindow& window)
{
    sf::Color colors[] = { sf::Color(255, 0, 0),
                           sf::Color(0, 0, 255),
                           sf::Color(255, 255, 255) };
    for (int i = 0; i < maxIndex; i++) {
        sf::CircleShape shape(2);
        m_asterSprite.setColor(
          sf::Color(m_parcolors[i].r,
                    m_parcolors[i].g,
                    m_parcolors[i].b,
                    (int)(m_pfarlife[i] * (228.0f / 50.0f))));

        m_asterSprite.setPosition(m_pvglarpos[i].x, m_pvglarpos[i].y);
        m_asterSprite.setRotation(m_pfarrotation[i]);
        window.draw(m_asterSprite);
    }
}
