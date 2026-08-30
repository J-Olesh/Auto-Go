#ifndef _GameRenderer
#define _GameRenderer

#include "GoGame.h"

// defines how the game's state should be rendered
class GameRenderer
{
public:
    void render(sf::RenderWindow &window,
                const GoGame &game);
    GameRenderer();
    float getBoardHeightPx(const GoGame &game);
    float getBoardWidthPx(const GoGame &game);

private:
    float stoneSize_;
};
#endif