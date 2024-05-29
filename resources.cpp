#include "resources.h"

resources* resources::instance = nullptr;

resources::resources() {}

void
resources::init()
{
    gameFont.loadFromFile("resources/Brick3DRegular-nRJR4.ttf");
    
    m_soundBufferMainOne.loadFromFile("resources/tetris99main.ogg");
    m_soundBufferMainTwo.loadFromFile("resources/tetris99main2.ogg");
    m_soundBufferMainThree.loadFromFile("resources/TDrift.ogg");
    m_soundBufferOne.loadFromFile("resources/one.ogg");
    m_soundOne.setBuffer(m_soundBufferOne);
    m_soundBufferTwo.loadFromFile("resources/two.ogg");
    m_soundTwo.setBuffer(m_soundBufferTwo);
    m_soundBufferThree.loadFromFile("resources/three.ogg");
    m_soundThree.setBuffer(m_soundBufferThree);
    m_soundBufferGo.loadFromFile("resources/ready.ogg");
    m_soundReady.setBuffer(m_soundBufferGo);
    m_soundBufferReady.loadFromFile("resources/go.ogg");
    m_soundGo.setBuffer(m_soundBufferReady);

    m_soundBufferMove.loadFromFile("resources/move.ogg");
    m_soundMove.setBuffer(m_soundBufferMove);
    m_soundBufferWarp.loadFromFile("resources/warp.ogg");
    m_soundWarp.setBuffer(m_soundBufferWarp);

    m_asterTexture.loadFromFile("resources/asterw.png");
    m_asterSprite.setTexture(m_asterTexture);

    m_fogTexture.loadFromFile("resources/warp.png");
}

void
resources::initApple()
{
}

sf::SoundBuffer*
resources::getSoundBufferMainOne()
{
    return &(getInstance()->m_soundBufferMainOne);
}

sf::SoundBuffer*
resources::getSoundBufferMainTwo()
{
    return &(getInstance()->m_soundBufferMainTwo);
}

sf::SoundBuffer*
resources::getSoundBufferMainThree()
{
    return &(getInstance()->m_soundBufferMainThree);
}

sf::SoundBuffer*
resources::getSoundBufferOne()
{
    return &(getInstance()->m_soundBufferOne);
}

sf::SoundBuffer*
resources::getSoundBufferTwo()
{
    return &(getInstance()->m_soundBufferTwo);
}

sf::SoundBuffer*
resources::getSoundBufferThree()
{
    return &(getInstance()->m_soundBufferThree);
}

sf::SoundBuffer*
resources::getSoundBufferGo()
{
    return &(getInstance()->m_soundBufferGo);
}

sf::SoundBuffer*
resources::getSoundBufferReady()
{
    return &(getInstance()->m_soundBufferReady);
}

sf::SoundBuffer*
resources::getSoundBufferMove()
{
    return &(getInstance()->m_soundBufferMove);
}

sf::SoundBuffer*
resources::getSoundBufferWarp()
{
    return &(getInstance()->m_soundBufferWarp);
}

sf::Sound*
resources::getSoundOne()
{
    return &(getInstance()->m_soundOne);
}

sf::Sound*
resources::getSoundTwo()
{
    return &(getInstance()->m_soundTwo);
}

sf::Sound*
resources::getSoundThree()
{
    return &(getInstance()->m_soundThree);
}

sf::Sound*
resources::getSoundGo()
{
    return &(getInstance()->m_soundGo);
}

sf::Sound*
resources::getSoundReady()
{
    return &(getInstance()->m_soundReady);
}

sf::Sound*
resources::getSoundMove()
{
    return &(getInstance()->m_soundMove);
}

sf::Sound*
resources::getSoundWarp()
{
    return &(getInstance()->m_soundWarp);
}

sf::Texture*
resources::getTextureAster()
{
    return &(getInstance()->m_asterTexture);
}

sf::Font*
resources::getFont()
{
    return &(getInstance()->gameFont);
}

sf::Sprite*
resources::getSpriteAster()
{
    return &(getInstance()->m_asterSprite);
}

sf::Texture*
resources::getTextureFog()
{
    return &(getInstance()->m_fogTexture);
}