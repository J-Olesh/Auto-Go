#include <SFML/Graphics.hpp>
#include <GoGame.h>
#include <iostream>

int main()
{
	sf::Clock clock;
	sf::RenderWindow window(sf::VideoMode({800, 800}), "SFML works!");

	const int BOARD_W = 19;
	const int BOARD_H = 19;
	GoGame game = GoGame(BOARD_W, BOARD_H);

	vector<sf::CircleShape> stones;
	while (window.isOpen())
	{
		while (const std::optional event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
			{
				window.close();
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Up))
			{
				sf::Time elapsed = clock.getElapsedTime();

				if (elapsed.asMilliseconds() > 20)
				{
					vector<sf::Vector2i> empties = game.getEmptySpaces();
					int size = empties.size();
					int move = rand() % size;
					if (game.placeStone(empties[move]) != -1)
					{
						clock.restart();
					}
				}
			}
		}
		stones.clear();
		for (int y = 0; y < 19; y++)
		{
			for (int x = 0; x < 19; x++)
			{
				if (game.board[x][y].state == 'B')
				{

					sf::CircleShape shape(10.f);
					shape.setFillColor(sf::Color::Blue);
					shape.setOrigin(shape.getGeometricCenter());
					shape.setPosition({(x * 20) + 10, (y * 20 + 10)});
					stones.push_back(shape);
				}
				else if (game.board[x][y].state == 'W')
				{

					sf::CircleShape shape(10.f);
					shape.setFillColor(sf::Color::White);
					shape.setOrigin(shape.getGeometricCenter());
					shape.setPosition({(x * 20) + 10, (y * 20 + 10)});

					stones.push_back(shape);
				}
			}
		}
		window.clear();
		for (auto s : stones)
		{
			window.draw(s);
		}
		
		// if (stones.size() != 0)
		// {
		// 	sf::CircleShape last_move(5.0f);
		// 	last_move.setFillColor(sf::Color::Black);
		// 	last_move.setOrigin(last_move.getGeometricCenter());
		// 	last_move.setPosition(stones.back().getPosition());
		// 	window.draw(last_move);
		// }

		window.display();
	}
}
