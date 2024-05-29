#pragma once
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#if defined(__APPLE__)
#include <CoreFoundation/CoreFoundation.h>
#include <iostream>
#endif


class resources
{
  private:
    sf::Font gameFont;
    sf::SoundBuffer m_soundBufferMainOne;
    sf::SoundBuffer m_soundBufferMainTwo;
    sf::SoundBuffer m_soundBufferMainThree;
    sf::SoundBuffer m_soundBufferOne;
    sf::SoundBuffer m_soundBufferTwo;
    sf::SoundBuffer m_soundBufferThree;
    sf::SoundBuffer m_soundBufferGo;
    sf::SoundBuffer m_soundBufferReady;
    sf::Sound m_soundOne;
    sf::Sound m_soundTwo;
    sf::Sound m_soundThree;
    sf::Sound m_soundReady;
    sf::Sound m_soundGo;

    sf::SoundBuffer m_soundBufferMove;
    sf::Sound m_soundMove;
    sf::SoundBuffer m_soundBufferWarp;
    sf::Sound m_soundWarp;
    sf::Sprite m_asterSprite;
    sf::Texture m_asterTexture;

    sf::Texture m_fogTexture;

  public:
    resources();
    void init();
    static resources* instance;
    static resources* getInstance()
    {
        if (instance == nullptr) {
            instance = new resources;
#if __APPLE__
            instance->initApple();
#else
            instance->init();
#endif
            return instance;
        } else {
            return instance;
        }
    }

    static sf::SoundBuffer *getSoundBufferMainOne();
    static sf::SoundBuffer *getSoundBufferMainTwo();
    static sf::SoundBuffer* getSoundBufferMainThree();
    static sf::SoundBuffer* getSoundBufferOne();
    static sf::SoundBuffer* getSoundBufferTwo();
    static sf::SoundBuffer* getSoundBufferThree();
    static sf::SoundBuffer* getSoundBufferGo();
    static sf::SoundBuffer* getSoundBufferReady();

    static sf::SoundBuffer* getSoundBufferMove();
    static sf::SoundBuffer* getSoundBufferWarp();

    static sf::Sound* getSoundOne();
    static sf::Sound* getSoundTwo();
    static sf::Sound* getSoundThree();
    static sf::Sound* getSoundGo();
    static sf::Sound* getSoundReady();
    static sf::Sound* getSoundMove();
    static sf::Sound* getSoundWarp();


    static sf::Texture* getTextureAster();
    static sf::Sprite* getSpriteAster();
    static sf::Texture* getTextureFog();

    static sf::Font* getFont();

#if __APPLE__
    void initApple();
    void getResourcePath(const CFStringRef resourceName, char* resourcePath);
#endif
};
