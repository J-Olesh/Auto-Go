#ifndef _GoScene
#define _GoScene

#include <SFML/Graphics.hpp>
#include <GoGame.h>
#include "GameRenderer.h"

// responsible for making game moves, starting, pausing etc
class GoScene
{
public:
    void run();
    GoScene(sf::RenderWindow &window, sf::Font &font);

private:
    sf::Clock timer_;

    sf::Text text;

    void updateUi();
    void render();
    void reset();
    bool paused_;

    sf::RenderWindow &window_;
    GoGame game_;
    GameRenderer renderer_;
};

#endif