#pragma once

#include <SFML/Graphics.hpp>

namespace tt {
struct rectStruct
{
    int x, y, w, h;
};

typedef rectStruct rect;

const sf::Color gShapesColor[7] = {
    sf::Color(0, 127, 4, 255),    sf::Color(246, 159, 0, 255),
    sf::Color(163, 44, 38, 255),  sf::Color(254, 1, 0, 255),
    sf::Color(5, 253, 255, 255),  sf::Color(6, 4, 237, 255),
    sf::Color(250, 175, 190, 255)
};

const sf::Color gFreezedColor = sf::Color(192, 192, 192, 255);
const sf::Color gGameOverColor = sf::Color(92, 92, 92, 255);
const sf::Color gLevelColor = sf::Color(192, 192, 0, 255);


//const float gRotationCenters[7][4][2] = {
//    { { 0.625, 0.625 }, { 0.375, 0.375 }, { 0.625, 0.625 }, { 0.375, 0.375 } },
//    { { 0.625, 0.625 }, { 0.375, 0.375 }, { 0.625, 0.625 }, { 0.375, 0.375 } },
//    { { 0.375, 0.375 }, { 0.375, 0.375 }, { 0.625, 0.625 }, { 0.625, 0.625 } },
//    { { 0.500, 0.500 }, { 0.500, 0.500 }, { 0.500, 0.500 }, { 0.500, 0.500 } },
//    { { 0.375, 0.375 }, { 0.625, 0.625 }, { 0.375, 0.375 }, { 0.625, 0.625 } },
//    { { 0.375, 0.375 }, { 0.625, 0.625 }, { 0.625, 0.625 }, { 0.375, 0.375 } },
//    { { 0.375, 0.375 }, { 0.625, 0.625 }, { 0.375, 0.375 }, { 0.625, 0.625 } }
//};



const int gDepthRotationSearch[7] = { 2, 4, 4, 1, 2, 4, 2 };



// clang-format off
const int gShapesArray[7][4][16] = {
    {
        {       0, 0, 0, 0,
                1, 1, 1, 1,
                0, 0, 0, 0,
                0, 0, 0, 0
        },
        {       0, 1, 0, 0,
                0, 1, 0, 0,
                0, 1, 0, 0,
                0, 1, 0, 0
        },
        {       0, 0, 0, 0,
                1, 1, 1, 1,
                0, 0, 0, 0,
                0, 0, 0, 0
        },
        {       0, 1, 0, 0,
                0, 1, 0, 0,
                0, 1, 0, 0,
                0, 1, 0, 0
        },
    },
    {
        {       1, 0, 0, 0,
                1, 1, 1, 0,
                0, 0, 0, 0,
                0, 0, 0, 0
        },
        {       1, 1, 0, 0,
                1, 0, 0, 0,
                1, 0, 0, 0,
                0, 0, 0, 0
        },
        {       0, 0, 0, 0,
                1, 1, 1, 0,
                0, 0, 1, 0,
                0, 0, 0, 0
        },
        {       0, 1, 0, 0,
                0, 1, 0, 0,
                1, 1, 0, 0,
                0, 0, 0, 0
        },

    },
    {
        {       0, 0, 1, 0,
                1, 1, 1, 0,
                0, 0, 0, 0,
                0, 0, 0, 0
        },
        {       0, 1, 0, 0,
                0, 1, 0, 0,
                0, 1, 1, 0,
                0, 0, 0, 0
        },
        {       0, 0, 0, 0,
                1, 1, 1, 0,
                1, 0, 0, 0,
                0, 0, 0, 0
        },
        {       1, 1, 0, 0,
                0, 1, 0, 0,
                0, 1, 0, 0,
                0, 0, 0, 0
        },
    },
    {
        {       1, 1, 0, 0,
                1, 1, 0, 0,
                0, 0, 0, 0,
                0, 0, 0, 0
        },
        {       1, 1, 0, 0,
                1, 1, 0, 0,
                0, 0, 0, 0,
                0, 0, 0, 0
        },
        {       1, 1, 0, 0,
                1, 1, 0, 0,
                0, 0, 0, 0,
                0, 0, 0, 0
        },
        {       1, 1, 0, 0,
                1, 1, 0, 0,
                0, 0, 0, 0,
                0, 0, 0, 0
        }
    },
    {
        {       0, 1, 1, 0,
                1, 1, 0, 0,
                0, 0, 0, 0,
                0, 0, 0, 0
        },
        {       1, 0, 0, 0,
                1, 1, 0, 0,
                0, 1, 0, 0,
                0, 0, 0, 0
        },
        {       0, 1, 1, 0,
                1, 1, 0, 0,
                0, 0, 0, 0,
                0, 0, 0, 0
        },
        {       1, 0, 0, 0,
                1, 1, 0, 0,
                0, 1, 0, 0,
                0, 0, 0, 0
        }
    },
    {
        {       0, 1, 0, 0,
                1, 1, 1, 0,
                0, 0, 0, 0,
                0, 0, 0, 0
        },
        {       0, 1, 0, 0,
                0, 1, 1, 0,
                0, 1, 0, 0,
                0, 0, 0, 0
        },
        {       0, 0, 0, 0,
                1, 1, 1, 0,
                0, 1, 0, 0,
                0, 0, 0, 0
        },
        {       0, 1, 0, 0,
                1, 1, 0, 0,
                0, 1, 0, 0,
                0, 0, 0, 0
        },
    },
    {
        {       1, 1, 0, 0,
                0, 1, 1, 0,
                0, 0, 0, 0,
                0, 0, 0, 0
        },
        {       0, 0, 1, 0,
                0, 1, 1, 0,
                0, 1, 0, 0,
                0, 0, 0, 0
        },
        {       1, 1, 0, 0,
                0, 1, 1, 0,
                0, 0, 0, 0,
                0, 0, 0, 0
        },
        {       0, 1, 0, 0,
                0, 1, 1, 0,
                0, 0, 1, 0,
                0, 0, 0, 0
        }
    }
};
// clang-format on
}

class shapes
{
  public:
    shapes() {}
};
