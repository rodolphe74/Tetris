#pragma once
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include "resources.h"

#define GAP_W 40
#define GAP_H 30
#define SPACING 13
#define A_NUMBER 6
#define FONT_LEADING 4.0f
#define ITEM_HEIGHT_INCREMENT 30
#define FONT_SIZE 30
#define TITLE "Choose !"
#define NO_ITEM_SELECTED -1

struct itemStruct
{
    sf::String text;
    int value;
    sf::Keyboard::Key shortcut;
};

typedef itemStruct item;

class Menu
{
  private:
    //sf::Font m_menuFont;
    sf::Font* m_menuFontPtr;
    sf::RenderWindow* m_prenderWindow;
    std::vector<item> m_vecItems;
    float m_fheight = 0.0f;
    float m_fwidth = 0.0f;
    int m_iselectedItem = 0;
    //sf::SoundBuffer m_soundBufferMove;
    //sf::Sound m_soundMove;
    //sf::SoundBuffer m_soundBufferWarp;
    //sf::Sound m_soundWarp;
    bool m_boolkeyReleased = true;

  public:
    bool m_boolcanSound = true;
    Menu(sf::RenderWindow& window)
    {
        m_prenderWindow = &window;

        // load font
        //if (!m_menuFont.loadFromFile("resources/Brick3DRegular-nRJR4.ttf")) {
        //    perror("can't load font");
        //}
        m_menuFontPtr = resources::getFont();

        computeHeight();

        //if (!m_soundBufferMove.loadFromFile("resources/move.ogg")) {
        //    perror("can't load sound");
        //}
        //m_soundMove.setBuffer(m_soundBufferMove);

        //if (!m_soundBufferWarp.loadFromFile("resources/warp.ogg")) {
        //    perror("can't load sound");
        //}
        //m_soundWarp.setBuffer(m_soundBufferWarp);
    }
    void computeHeight();
    void addItem(item i);
    void render();
    int checkKeyboard();
};
