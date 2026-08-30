#include "GameRenderer.h"

void GameRenderer::render(sf::RenderWindow &window, const GoGame &game)
{

    for (int y = 0; y < game.height; y++)
    {
        for (int x = 0; x < game.width; x++)
        {

            if (game.board[x][y].state == 'B')
            {

                sf::CircleShape shape(stoneSize_);
                shape.setFillColor(sf::Color::Blue);
                shape.setOrigin(shape.getGeometricCenter());
                shape.setPosition({(x * 20.f) + 10, (y * 20.f + 10)});
                window.draw(shape);
                // stones.push_back(shape);
            }
            else if (game.board[x][y].state == 'W')
            {

                sf::CircleShape shape(stoneSize_);
                shape.setFillColor(sf::Color::Magenta);
                shape.setOrigin(shape.getGeometricCenter());
                shape.setPosition({(x * 20.f) + 10, (y * 20.f + 10)});
                window.draw(shape);
                // stones.push_back(shape);
            }
            if (game.moveCount > 0)
            {
                if (game.lastMove.x == x && game.lastMove.y == y)
                {
                    sf::CircleShape marker(3.f);
                    marker.setFillColor(sf::Color::White);
                    marker.setOutlineColor(sf::Color::White);
                    marker.setOutlineThickness(2.f);
                    marker.setOrigin(marker.getGeometricCenter());
                    marker.setPosition({(x * 20.f) + 10, (y * 20.f + 10)});
                    window.draw(marker);
                    // stones.push_back(marker);
                }
            }
        }
    }
}

GameRenderer::GameRenderer()
{
    stoneSize_ = 10.f;
}

float GameRenderer::getBoardHeightPx(const GoGame &game)
{
    return (stoneSize_ * 2) * game.height + 10;
}

float GameRenderer::getBoardWidthPx(const GoGame &game)
{
    return (stoneSize_ * 2) * game.width + 10;
}
