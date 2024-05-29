#include "menu.h"
#include "constants.h"

void
Menu::computeHeight()
{
    sf::String title = TITLE;
    sf::Text t(title, *m_menuFontPtr, 30);
    // m_fheight = t.getLocalBounds().height + 2 * ITEM_HEIGHT_INCREMENT;
    // m_fwidth = t.getLocalBounds().width + 2 * GAP;
    m_fwidth = 0;
    m_fheight = 0;
    int count = 0;
    for (auto it = m_vecItems.begin(); it != m_vecItems.end(); it++) {
        sf::String s = it->text;
        //sf::Text i(s, m_menuFont, 30);
        sf::Text i(s, *m_menuFontPtr, 30);

        m_fheight += i.getLocalBounds().height;
        if (count < m_vecItems.size() - 1)
            m_fheight += SPACING;

        if (i.getGlobalBounds().width > m_fwidth)
            m_fwidth = i.getGlobalBounds().width;
        count++;
    }
}

void
Menu::addItem(item i)
{
    m_vecItems.push_back(i);
    computeHeight();
}

void
Menu::render()
{
    sf::String title = TITLE;
    sf::Text t(title, *m_menuFontPtr, FONT_SIZE);

    sf::RectangleShape box(
      sf::Vector2f(m_fwidth + 2 * GAP_W, m_fheight + 2 * GAP_H));
    box.setPosition(WINDOW_W / 2 - box.getSize().x / 2,
                    WINDOW_H / 2 - box.getSize().y / 2);

    box.setFillColor(sf::Color(255, 255, 255, 128));
    box.setOutlineColor(sf::Color(255, 255, 255, 255));
    box.setOutlineThickness(2);
    m_prenderWindow->draw(box);

    t.setPosition(WINDOW_W / 2 - m_fwidth / 2 + GAP_W,
                  WINDOW_H / 2 - m_fheight / 2);
    t.setFillColor(sf::Color::Black);
    // m_prenderWindow->draw(t);

    float y = 0;
    int count = 0;
    for (auto it = m_vecItems.begin(); it != m_vecItems.end(); it++) {
        sf::String s = it->text;
        sf::Text i(s, *m_menuFontPtr, FONT_SIZE);
        i.setPosition(WINDOW_W / 2 - box.getSize().x / 2 + GAP_W,
                      WINDOW_H / 2 - m_fheight / 2 + y -
                        m_vecItems.size() * FONT_LEADING);
        i.setFillColor(sf::Color::Black);
        i.setOutlineColor(sf::Color::Black);
        i.setOutlineThickness(0);
        m_prenderWindow->draw(i);
        y += i.getGlobalBounds().height;

        if (count == m_iselectedItem) {
            sf::RectangleShape selectionBox(
              sf::Vector2f(m_fwidth + 2 * GAP_W, i.getGlobalBounds().height));
            selectionBox.setPosition(WINDOW_W / 2 - box.getSize().x / 2,
                                     WINDOW_H / 2 - m_fheight / 2 + y -
                                       i.getGlobalBounds().height - A_NUMBER);

            selectionBox.setFillColor(sf::Color(255, 0, 0, 64));
            selectionBox.setOutlineColor(sf::Color(255, 0, 0, 64));
            selectionBox.setOutlineThickness(4);
            m_prenderWindow->draw(selectionBox);
        }

        if (count < m_vecItems.size() - 1)
            y += SPACING;

        count++;
    }
}

int
Menu::checkKeyboard()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && m_boolkeyReleased) {
        m_iselectedItem =
          (int)(m_iselectedItem >= m_vecItems.size() - 1 ? m_vecItems.size() - 1
                                                         : m_iselectedItem + 1);
        if (m_boolcanSound) {
            resources::getSoundMove()->play();
            m_boolcanSound = false;
        }
        m_boolkeyReleased = false;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && m_boolkeyReleased) {
        m_iselectedItem = (m_iselectedItem <= 0 ? 0 : m_iselectedItem - 1);
        if (m_boolcanSound) {
            resources::getSoundMove()->play();
            m_boolcanSound = false;
        }
        m_boolkeyReleased = false;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && m_boolkeyReleased) {
        if (m_boolcanSound) {
            resources::getSoundWarp()->play();
            m_boolcanSound = false;
        }
        m_boolkeyReleased = false;
        return m_vecItems[m_iselectedItem].value;
    }

    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Up) &&
        !sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        m_boolkeyReleased = true;
    }

    return NO_ITEM_SELECTED;
}
