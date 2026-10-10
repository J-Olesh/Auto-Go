#ifndef _GameRenderer
#define _GameRenderer

#include "GoGame.h"

class GameRenderer
{
public:
    void render(const GoGame &game);

    GameRenderer(sf::Color blackColour, sf::Color whiteColor, sf::RenderWindow &window);

    float getBoardHeightPx(const GoGame &game);
    float getBoardWidthPx(const GoGame &game);

    sf::Color blackColour_;
    sf::Color whiteColour_;

private:
    float _stoneRadius;

    sf::RenderWindow &_window;
};
#endif