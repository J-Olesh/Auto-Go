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
    GoScene(sf::RenderWindow &window, sf::Font &font, int boardW, int boardH);

private:
    int _boardW;
    int _boardH;

    sf::Font font_;
    sf::Text _statsText;
    sf::Text _statusText;
    void updateUi(sf::Clock &);
    void render();
    void reset();
    bool _paused;
    bool autoReset;

    sf::RenderWindow &_window;
    GoGame _game;
    GameRenderer _renderer;
};

#endif