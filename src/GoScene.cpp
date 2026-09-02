#include "GoScene.h"
#include <random>
#include <iostream>

void GoScene::run()
{
    auto rd = std::random_device{};
    auto rng = std::default_random_engine{rd()};
    sf::Clock timer;
    timer.reset();

    while (_window.isOpen())
    {
        while (const std::optional event = _window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                _window.close();
            }

            if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                switch (keyPressed->scancode)
                {
                case sf::Keyboard::Scan::P:
                    _paused = !_paused;
                    if (_paused)
                    {
                        timer.stop();
                    }
                    else
                    {
                        timer.start();
                    }
                    break;

                case sf::Keyboard::Scan::Space:
                    reset();
                    timer.restart();
                    _paused = false;
                    break;

                case sf::Keyboard::Scan::R:
                    autoReset = !autoReset;
                    break;

                default:
                    break;
                }
            }
        }

        _window.clear();
        if (!_paused)
        {
            auto empties = _game.getEmptySpaces();
            // probably could be more efficient
            std::shuffle(empties.begin(), empties.end(), rng);

            bool moveMade = false;

            while (!empties.empty())
            {
                if (_game.placeStone(empties.back()))
                {
                    moveMade = true;
                    break;
                }

                empties.pop_back();
            }

            if (!moveMade)
            {
                if (autoReset)
                {
                    reset();
                    timer.restart();
                }
                else
                {
                    timer.stop();
                    _paused = true;
                }
            }
        }

        render();
        updateUi(timer);
        _window.display();
    }
}

GoScene::GoScene(sf::RenderWindow &window, sf::Font &font, GoGame &game) : _renderer(sf::Color::Blue, sf::Color(255, 197, 247), window), _window(window), _statsText(font), _statusText(font), _game(game)
{
    _boardH = game.height;
    _boardW = game.width;
    autoReset = false;
    _paused = true;
    // font_ = font;
    _statsText.setCharacterSize(20);
    float w = _renderer.getBoardWidthPx(_game);
    _statsText.setPosition({w + 2, 0.f});

    _statusText.setCharacterSize(20);
    _statusText.setPosition({w + 2, 230.f});
}

void GoScene::updateUi(sf::Clock &clock)
{

    _statsText.setString("Captures\nBlack: " + std::to_string(_game.black_captures) + "\nWhite: " + std::to_string(_game.white_captures) + "\n\nMoves: " + std::to_string(_game.moveCount));
    std::string pausedstr = (_paused) ? "Paused" : "Playing";
    std::string autoResetStr = (autoReset) ? "\nAuto reset ON" : "\nAuto reset OFF";
    float elapsedSeconds = clock.getElapsedTime().asSeconds();
    std::string elapsedSecondsStr = "\nTime: " + std::to_string(elapsedSeconds) + "s";

    pausedstr.append(autoResetStr);
    pausedstr.append(elapsedSecondsStr);
    if (elapsedSeconds > 0.f)
    {
        pausedstr.append("\nMoves per Second: " + std::to_string(_game.moveCount / elapsedSeconds));
    }

    _statusText.setString(pausedstr);

    _window.draw(_statsText);
    _window.draw(_statusText);
}

void GoScene::render()
{
    _renderer.render(_game);
}

void GoScene::reset()
{
    _game = GoGame(_boardW, _boardH);
}
