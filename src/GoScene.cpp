#include "GoScene.h"
#include <iostream>

void GoScene::update()
{

    if (!_paused || !finished)
    {
        auto empties = game.getEmptySpaces();
        // probably could be more efficient
        std::shuffle(empties.begin(), empties.end(), _rng);

        bool moveMade = false;

        while (!empties.empty())
        {
            if (game.placeStone(empties.back()))
            {
                moveMade = true;
                break;
            }

            empties.pop_back();
        }

        // if no valid moves
        if (!moveMade)
        {

            if (autoReset)
            {
                reset();
            }
            else
            {
                finished = true;
                _gameTimer.stop();
                _paused = true;
            }
        }
    }
    _renderer.render(game);
}

GoScene::GoScene(sf::RenderWindow &window, int w, int h) : _renderer(sf::Color::Blue, sf::Color(255, 197, 247), window), _window(window), game(GoGame(w, h))
{
    finished = false;
    auto rd = std::random_device{};
    _rng = std::default_random_engine{rd()};
    autoReset = false;
    _paused = true;
    finished = false;
    _gameTimer.reset();
}

void GoScene::togglePause()
{
    _paused = !_paused;
    if (_paused)
    {
        _gameTimer.stop();
    }
    else
    {
        _gameTimer.start();
    }
}

void GoScene::restart(int w, int h)
{
    game = GoGame(w, h);
    _gameTimer.restart();
    _paused = false;
    finished = false;
}

void GoScene::reset()
{
    int w = game.width;
    int h = game.height;
    game = GoGame(w, h);
    _gameTimer.restart();
    _paused = false;
    finished = false;
}

void GoScene::toggleAutoReset()
{
    autoReset = !autoReset;
}