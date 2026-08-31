#ifndef _GameRenderer
#define _GameRenderer

#include "GoGame.h"

class GameRenderer
{
public:
    void render(sf::RenderWindow &window,
                const GoGame &game);
    GameRenderer(sf::Color blackColour, sf::Color whiteColor);
    float getBoardHeightPx(const GoGame &game);
    float getBoardWidthPx(const GoGame &game);

    sf::Color blackColour_;
    sf::Color whiteColour_;

private:
    float stoneSize_;
};
#endif