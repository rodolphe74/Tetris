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

    m_soundBufferExplode.loadFromFile("resources/explode.ogg");
    m_soundExplode.setBuffer(m_soundBufferExplode);
    m_soundBufferRotate.loadFromFile("resources/rotate.ogg");
    m_soundRotate.setBuffer(m_soundBufferRotate);
    m_soundBufferMove.loadFromFile("resources/move.ogg");
    m_soundMove.setBuffer(m_soundBufferMove);
    m_soundBufferLine.loadFromFile("resources/line.ogg");
    m_soundLine.setBuffer(m_soundBufferLine);
    m_soundBufferWarp.loadFromFile("resources/warp.ogg");
    m_soundWarp.setBuffer(m_soundBufferWarp);
    m_soundBufferIntro.loadFromFile("resources/intro.ogg");
    m_soundIntro.setBuffer(m_soundBufferIntro);
    m_soundBufferHurry.loadFromFile("resources/hurry.ogg");
    m_soundHurry.setBuffer(m_soundBufferHurry);
    m_soundBufferGameOver.loadFromFile("resources/gameover.ogg");
    m_soundGameOver.setBuffer(m_soundBufferGameOver);

    m_squareTexture.loadFromFile("resources/big_square_clear.png");
    m_borderTexture.loadFromFile("resources/borderb.png");

    m_soundBufferMove.loadFromFile("resources/move.ogg");
    m_soundMove.setBuffer(m_soundBufferMove);

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
    m_soundBufferMainOne.loadFromFile(path);
    CFRelease(cstrrefTetris99main);

    CFStringRef cstrrefTetris99main2 = CFStringCreateWithCString(
      NULL, "tetris99main2.ogg", kCFStringEncodingUTF8);
    getResourcePath(cstrrefTetris99main2, path);
    std::cout << "***path***:" << path << std::endl;
    m_soundBufferMainTwo.loadFromFile(path);
    CFRelease(cstrrefTetris99main2);

    CFStringRef cstrrefTDrift =
      CFStringCreateWithCString(NULL, "TDrift.ogg", kCFStringEncodingUTF8);
    getResourcePath(cstrrefTDrift, path);
    std::cout << "***path***:" << path << std::endl;
    m_soundBufferMainThree.loadFromFile(path);
    CFRelease(cstrrefTDrift);

    CFStringRef cstrrefOne =
      CFStringCreateWithCString(NULL, "one.ogg", kCFStringEncodingUTF8);
    getResourcePath(cstrrefOne, path);
    std::cout << "***path***:" << path << std::endl;
    m_soundBufferOne.loadFromFile(path);
    m_soundOne.setBuffer(m_soundBufferOne);
    CFRelease(cstrrefOne);

    CFStringRef cstrrefTwo =
      CFStringCreateWithCString(NULL, "two.ogg", kCFStringEncodingUTF8);
    getResourcePath(cstrrefTwo, path);
    std::cout << "***path***:" << path << std::endl;
    m_soundBufferTwo.loadFromFile(path);
    m_soundTwo.setBuffer(m_soundBufferOne);
    CFRelease(cstrrefTwo);

    CFStringRef cstrrefThree =
      CFStringCreateWithCString(NULL, "three.ogg", kCFStringEncodingUTF8);
    getResourcePath(cstrrefThree, path);
    std::cout << "***path***:" << path << std::endl;
    m_soundBufferThree.loadFromFile(path);
    m_soundThree.setBuffer(m_soundBufferThree);
    CFRelease(cstrrefThree);

    CFStringRef cstrrefReady =
      CFStringCreateWithCString(NULL, "ready.ogg", kCFStringEncodingUTF8);
    getResourcePath(cstrrefReady, path);
    std::cout << "***path***:" << path << std::endl;
    m_soundBufferReady.loadFromFile(path);
    m_soundReady.setBuffer(m_soundBufferReady);
    CFRelease(cstrrefReady);

    CFStringRef cstrrefGo =
      CFStringCreateWithCString(NULL, "go.ogg", kCFStringEncodingUTF8);
    getResourcePath(cstrrefGo, path);
    std::cout << "***path***:" << path << std::endl;
    m_soundBufferGo.loadFromFile(path);
    m_soundGo.setBuffer(m_soundBufferGo);
    CFRelease(cstrrefGo);
    
    CFStringRef cstrrefMove =
      CFStringCreateWithCString(NULL, "move.ogg", kCFStringEncodingUTF8);
    getResourcePath(cstrrefMove, path);
    std::cout << "***path***:" << path << std::endl;
    m_soundBufferMove.loadFromFile(path);
    m_soundMove.setBuffer(m_soundBufferMove);
    CFRelease(cstrrefMove);

    CFStringRef cstrrefWarp =
      CFStringCreateWithCString(NULL, "warp.ogg", kCFStringEncodingUTF8);
    getResourcePath(cstrrefWarp, path);
    std::cout << "***path***:" << path << std::endl;
    m_soundBufferWarp.loadFromFile(path);
    m_soundWarp.setBuffer(m_soundBufferWarp);
    CFRelease(cstrrefWarp);
    
    CFStringRef cstrrefAsterW =
      CFStringCreateWithCString(NULL, "asterw.png", kCFStringEncodingUTF8);
    getResourcePath(cstrrefAsterW, path);
    std::cout << "***path***:" << path << std::endl;
    m_asterTexture.loadFromFile(path);
    m_asterSprite.setTexture(m_asterTexture);
    
    CFStringRef cstrrefExplode =
      CFStringCreateWithCString(NULL, "explode.ogg", kCFStringEncodingUTF8);
    getResourcePath(cstrrefExplode, path);
    std::cout << "***path***:" << path << std::endl;
    m_soundBufferExplode.loadFromFile(path);
    m_soundExplode.setBuffer(m_soundBufferExplode);
    CFRelease(cstrrefExplode);

    CFStringRef cstrrefRotate =
      CFStringCreateWithCString(NULL, "rotate.ogg", kCFStringEncodingUTF8);
    getResourcePath(cstrrefRotate, path);
    std::cout << "***path***:" << path << std::endl;
    m_soundBufferRotate.loadFromFile(path);
    m_soundRotate.setBuffer(m_soundBufferRotate);
    CFRelease(cstrrefRotate);

    CFStringRef cstrrefLine =
      CFStringCreateWithCString(NULL, "line.ogg", kCFStringEncodingUTF8);
    getResourcePath(cstrrefLine, path);
    std::cout << "***path***:" << path << std::endl;
    m_soundBufferLine.loadFromFile(path);
    m_soundLine.setBuffer(m_soundBufferLine);
    CFRelease(cstrrefLine);

    CFStringRef cstrrefIntro =
      CFStringCreateWithCString(NULL, "intro.ogg", kCFStringEncodingUTF8);
    getResourcePath(cstrrefIntro, path);
    std::cout << "***path***:" << path << std::endl;
    m_soundBufferIntro.loadFromFile(path);
    m_soundIntro.setBuffer(m_soundBufferIntro);
    CFRelease(cstrrefIntro);

    CFStringRef cstrrefHurry =
      CFStringCreateWithCString(NULL, "hurry.ogg", kCFStringEncodingUTF8);
    getResourcePath(cstrrefHurry, path);
    std::cout << "***path***:" << path << std::endl;
    m_soundBufferHurry.loadFromFile(path);
    m_soundHurry.setBuffer(m_soundBufferHurry);
    CFRelease(cstrrefHurry);

    CFStringRef cstrrefGameOver =
      CFStringCreateWithCString(NULL, "gameover.ogg", kCFStringEncodingUTF8);
    getResourcePath(cstrrefHurry, path);
    std::cout << "***path***:" << path << std::endl;
    m_soundBufferGameOver.loadFromFile(path);
    m_soundGameOver.setBuffer(m_soundBufferGameOver);
    CFRelease(cstrrefGameOver);

    CFStringRef cstrrefSquare =
      CFStringCreateWithCString(NULL, "big_square_clear.png", kCFStringEncodingUTF8);
    getResourcePath(cstrrefSquare, path);
    std::cout << "***path***:" << path << std::endl;
    m_squareTexture.loadFromFile(path);
    CFRelease(cstrrefSquare);

    CFStringRef cstrrefBorder = CFStringCreateWithCString(
      NULL, "borderb.png", kCFStringEncodingUTF8);
    getResourcePath(cstrrefBorder, path);
    std::cout << "***path***:" << path << std::endl;
    m_borderTexture.loadFromFile(path);
    CFRelease(cstrrefBorder);
}


void
resources::getResourcePath(const CFStringRef resourceName, char *resourcePath)
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

sf::Sound*
resources::getSoundIntro()
{
    return &(getInstance()->m_soundIntro);
}

sf::Sound*
resources::getSoundExplode()
{
    return &(getInstance()->m_soundExplode);
}

sf::Sound*
resources::getSoundRotate()
{
    return &(getInstance()->m_soundRotate);
}

sf::Sound*
resources::getSoundLine()
{
    return &(getInstance()->m_soundLine);
}

sf::Sound*
resources::getSoundHurry()
{
    return &(getInstance()->m_soundHurry);
}

sf::Sound*
resources::getSoundGameOver()
{
    return &(getInstance()->m_soundGameOver);
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

sf::Texture*
resources::getSquare()
{
    return &(getInstance()->m_squareTexture);
}

sf::Texture*
resources::getBorder()
{
    return &(getInstance()->m_borderTexture);
}
