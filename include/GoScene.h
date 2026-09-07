#ifndef GoScene_H
#define GoScene_H

#include <SFML/Graphics.hpp>
#include <GoGame.h>
#include <unordered_map>
#include "GameRenderer.h"

// responsible for making game moves, starting, pausing etc
class GoScene
{
public:
    void run();
    GoScene(sf::RenderWindow &window, sf::Font &font, GoGame &game);

private:
    int _boardW;
    int _boardH;
    int _finishedCount{0};
    int _totalMoves{0};
    std::unordered_map<int, float> _avg_moves;

    sf::Text _statsText;
    sf::Text _statusText;
    void updateUi(sf::Clock &);
    void render();
    void reset();
    bool _paused;
    bool autoReset;

    sf::RenderWindow &_window;
    GoGame &_game;
    GameRenderer _renderer;
};

#endif