#include "GameRenderer.h"

void GameRenderer::render(sf::RenderWindow &window, const GoGame &game)
{
    float stoneDiameter = stoneSize_ * 2;

    for (int y = 0; y < game.height; y++)
    {
        for (int x = 0; x < game.width; x++)
        {
            if (game.board[x][y].state != ' ')
            {
                sf::CircleShape shape(stoneSize_);
                sf::Color color = (game.board[x][y].state == 'B') ? blackColour_ : whiteColour_;
                shape.setFillColor(color);
                shape.setOrigin(shape.getGeometricCenter());
                shape.setPosition({(x * stoneDiameter) + stoneSize_, (y * stoneDiameter + stoneSize_)});
                window.draw(shape);
            }

            // highlight last move
            if (game.moveCount > 0)
            {
                if (game.lastMove.x == x && game.lastMove.y == y)
                {
                    sf::CircleShape marker(3.f);
                    marker.setFillColor(sf::Color::Green);
                    marker.setOutlineColor(sf::Color::Green);
                    marker.setOutlineThickness(2.f);
                    marker.setOrigin(marker.getGeometricCenter());
                    marker.setPosition({(x * stoneDiameter) + stoneSize_, (y * stoneDiameter + stoneSize_)});
                    window.draw(marker);
                }
            }
        }
    }
}

GameRenderer::GameRenderer(sf::Color blackColour, sf::Color whiteColor) : blackColour_(blackColour), whiteColour_(whiteColor)
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
