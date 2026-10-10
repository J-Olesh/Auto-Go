#ifndef GoScene_H
#define GoScene_H

#include <SFML/Graphics.hpp>
#include <GoGame.h>
#include "GameRenderer.h"
#include <random>
#include <map>

// responsible for making game moves, starting, pausing etc
class GoScene
{
public:
    void update();
    GoScene(sf::RenderWindow &window, int w, int h);
    void togglePause();
    void restart(int w, int h);
    void reset();
    void toggleAutoReset();
    bool autoReset;
    bool finished;
    bool enableRender;
    bool paused;
    GoGame game;
    std::map<int, float> capture_plot;

private:
    std::linear_congruential_engine<uint_fast32_t, 16807U, 0U, 2147483647U> _rng;
    sf::RenderWindow &_window;
    GameRenderer _renderer;
};

#endif