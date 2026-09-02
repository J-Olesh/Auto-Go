#include "GameRenderer.h"
#include <iostream>
void GameRenderer::render(const GoGame &game)
{
    float stoneDiameter = _stoneRadius * 2;
    float xLimit = (_window.getSize().x / 3) * 2;

    _stoneRadius = float(floor((xLimit / game.width) / 2));

    for (int y = 0; y < game.height; y++)
    {
        for (int x = 0; x < game.width; x++)
        {
            if (game.board[x][y].state != ' ')
            {
                sf::CircleShape shape(_stoneRadius);
                sf::Color color = (game.board[x][y].state == 'B') ? blackColour_ : whiteColour_;
                shape.setFillColor(color);
                shape.setOrigin(shape.getGeometricCenter());
                shape.setPosition({(x * stoneDiameter) + _stoneRadius, (y * stoneDiameter + _stoneRadius)});
                _window.draw(shape);
            }

            // highlight last move
            if (game.moveCount > 0)
            {
                if (game.lastMove.x == x && game.lastMove.y == y)
                {
                    sf::CircleShape marker(_stoneRadius / 2);
                    marker.setFillColor(sf::Color::Green);

                    marker.setOrigin(marker.getGeometricCenter());
                    marker.setPosition({(x * stoneDiameter) + _stoneRadius, (y * stoneDiameter + _stoneRadius)});
                    _window.draw(marker);
                }
            }
        }
    }
}

GameRenderer::GameRenderer(sf::Color blackColour, sf::Color whiteColor, sf::RenderWindow &window) : blackColour_(blackColour), whiteColour_(whiteColor),
                                                                                                    _window(window)
{
}

float GameRenderer::getBoardHeightPx(const GoGame &game)
{
    float yLimit = (_window.getSize().y / 3) * 2;
    return yLimit;
}

float GameRenderer::getBoardWidthPx(const GoGame &game)
{
    float xLimit = (_window.getSize().x / 3) * 2;
    return xLimit;
}
