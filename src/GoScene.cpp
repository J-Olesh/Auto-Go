#include "GoScene.h"
#include <random>
#include <iostream>

void GoScene::run()
{
    while (window_.isOpen())
    {
        while (const std::optional event = window_.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window_.close();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Right))
            {
                paused_ = !paused_;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::R))
            {
                reset();
            }
        }
        window_.clear();
        if (!paused_)
        {
            // make moves at random
            vector<sf::Vector2i> empties = game_.getEmptySpaces();
            auto rd = std::random_device{};
            auto rng = std::default_random_engine{rd()};

            std::shuffle(std::begin(empties), std::end(empties), rng);
            bool noValidMoves = false;
            while (!game_.placeStone(empties.back()))
            {
                if (empties.size() == 1)
                {
                    paused_ = true;

                    break;
                }
                empties.pop_back();
            }
        }
        render();
        updateUi();
        window_.display();
    }
}

GoScene::GoScene(sf::RenderWindow &window, sf::Font &font) : game_(19, 19), renderer_(), window_(window), text(font)
{
    paused_ = true;

    text.setCharacterSize(20);
    float w = renderer_.getBoardWidthPx(game_);
    text.setPosition({w + 2, 0.f});

    text.setString("captures\nBlack: " + std::to_string(game_.black_captures) + "\nWhite: " + std::to_string(game_.white_captures) + "\nMoves: " + std::to_string(game_.moveCount));
}

void GoScene::updateUi()
{
    text.setString("captures\nBlack: " + std::to_string(game_.black_captures) + "\nWhite: " + std::to_string(game_.white_captures) + "\nMoves: " + std::to_string(game_.moveCount));
    window_.draw(text);
}

void GoScene::render()
{
    renderer_.render(window_, game_);
}

void GoScene::reset()
{
    game_ = GoGame(19, 19);
}
