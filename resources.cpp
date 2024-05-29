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

#ifdef __APPLE__
void
resources::initApple()
{
    char path[1024];
    CFStringRef cstrrefFont = CFStringCreateWithCString(
      NULL, "Brick3DRegular-nRJR4.ttf", kCFStringEncodingUTF8);
    getResourcePath(cstrrefFont, path);
    gameFont.loadFromFile(path);
    CFRelease(cstrrefFont);

    CFStringRef cstrrefTetris99main = CFStringCreateWithCString(
      NULL, "tetris99main.ogg", kCFStringEncodingUTF8);
    getResourcePath(cstrrefTetris99main, path);
    std::cout << "***path***:" << path << std::endl;
    soundBufferMainOne.loadFromFile(path);
    CFRelease(cstrrefTetris99main);

    CFStringRef cstrrefTetris99main2 = CFStringCreateWithCString(
      NULL, "tetris99main2.ogg", kCFStringEncodingUTF8);
    getResourcePath(cstrrefTetris99main2, path);
    std::cout << "***path***:" << path << std::endl;
    soundBufferMainTwo.loadFromFile(path);
    CFRelease(cstrrefTetris99main2);

    CFStringRef cstrrefTDrift =
      CFStringCreateWithCString(NULL, "TDrift.ogg", kCFStringEncodingUTF8);
    getResourcePath(cstrrefTDrift, path);
    std::cout << "***path***:" << path << std::endl;
    soundBufferMainThree.loadFromFile(path);
    CFRelease(cstrrefTDrift);

    CFStringRef cstrrefOne =
      CFStringCreateWithCString(NULL, "one.ogg", kCFStringEncodingUTF8);
    getResourcePath(cstrrefOne, path);
    std::cout << "***path***:" << path << std::endl;
    soundBufferOne.loadFromFile(path);
    soundOne.setBuffer(soundBufferOne);
    CFRelease(cstrrefOne);

    CFStringRef cstrrefTwo =
      CFStringCreateWithCString(NULL, "two.ogg", kCFStringEncodingUTF8);
    getResourcePath(cstrrefTwo, path);
    std::cout << "***path***:" << path << std::endl;
    soundBufferTwo.loadFromFile(path);
    soundTwo.setBuffer(soundBufferOne);
    CFRelease(cstrrefTwo);

    CFStringRef cstrrefThree =
      CFStringCreateWithCString(NULL, "three.ogg", kCFStringEncodingUTF8);
    getResourcePath(cstrrefThree, path);
    std::cout << "***path***:" << path << std::endl;
    soundBufferThree.loadFromFile(path);
    soundThree.setBuffer(soundBufferThree);
    CFRelease(cstrrefThree);

    CFStringRef cstrrefReady =
      CFStringCreateWithCString(NULL, "ready.ogg", kCFStringEncodingUTF8);
    getResourcePath(cstrrefReady, path);
    std::cout << "***path***:" << path << std::endl;
    soundBufferReady.loadFromFile(path);
    soundReady.setBuffer(soundBufferReady);
    CFRelease(cstrrefReady);

    CFStringRef cstrrefGo =
      CFStringCreateWithCString(NULL, "go.ogg", kCFStringEncodingUTF8);
    getResourcePath(cstrrefGo, path);
    std::cout << "***path***:" << path << std::endl;
    soundBufferGo.loadFromFile(path);
    soundGo.setBuffer(soundBufferGo);
    CFRelease(cstrrefGo);
}


void
getResourcePath(const CFStringRef resourceName, char *resourcePath)
{
    CFURLRef appUrlRef;
    appUrlRef = CFBundleCopyResourceURL(
      CFBundleGetMainBundle(), resourceName, NULL, NULL);
    CFStringRef filePathRef = CFURLCopyPath(appUrlRef);
    const char* filePath =
      CFStringGetCStringPtr(filePathRef, kCFStringEncodingUTF8);

    std::cout << "filePath:" << filePath << std::endl;
    strcpy(resourcePath, filePath);

    CFRelease(filePathRef);
    CFRelease(appUrlRef);
}
#endif // __APPLE__



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