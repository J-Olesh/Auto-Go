#include <SFML/Graphics.hpp>
#include <GoGame.h>
#include <iostream>
// todo: either capture count or move count is wrong. its counting captures again for each adj enemy stone?
int main()
{
	sf::Font font;
	font.openFromFile("C:/Windows/Fonts/arial.ttf");
	sf::Text text(font); // a font is required to make a text object
	text.setPosition({90.f, 650.f});
	// set the string to display
	// text.setString("");

	// set the character size
	text.setCharacterSize(24); // in pixels, not points!

	// set the color
	text.setFillColor(sf::Color::Red);

	// set the text style
	text.setStyle(sf::Text::Bold);

	sf::Clock clock;
	sf::RenderWindow window(sf::VideoMode({800, 800}), "SFML works!");

	const int BOARD_W = 9;
	const int BOARD_H = 9;
	GoGame game = GoGame(BOARD_W, BOARD_H);

	vector<sf::CircleShape> stones;
	bool autoPlay = false;
	int moveCount = 0;
	vector<sf::Vector2i> moves = {{0, 0}, {0, 1}, {5, 5}, {1, 0}};
	// vector<sf::Vector2i> moves = {{2, 2}, {1, 2}, {5, 5}, {2, 1}, {6, 6}, {3, 2}, {7, 7}, {2, 3}};
	while (window.isOpen())
	{
		while (const std::optional event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
			{
				window.close();
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Right))
			{
				autoPlay = !autoPlay;
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Up))
			{
				for (auto move : moves)
				{
					game.placeStone(move);
					stones.clear();
					moveCount++;
					for (int y = 0; y < BOARD_H; y++)
					{
						for (int x = 0; x < BOARD_W; x++)
						{
							if (game.board[x][y].state == 'B')
							{

								sf::CircleShape shape(10.f);
								shape.setFillColor(sf::Color::Blue);
								shape.setOrigin(shape.getGeometricCenter());
								shape.setPosition({(x * 20.f) + 10, (y * 20.f + 10)});
								stones.push_back(shape);
							}
							else if (game.board[x][y].state == 'W')
							{

								sf::CircleShape shape(10.f);
								shape.setFillColor(sf::Color::Magenta);
								shape.setOrigin(shape.getGeometricCenter());
								shape.setPosition({(x * 20.f) + 10, (y * 20.f + 10)});

								stones.push_back(shape);
							}
						}
					}

					text.setString("Black captures: " + std::to_string(game.black_captures) + "\nWhite captures: " + std::to_string(game.white_captures) + "\nMoves: " + std::to_string(moveCount));
				}
			}
		}
		sf::Time elapsed = clock.getElapsedTime();

		if (elapsed.asMilliseconds() > 50 && autoPlay)
		{
			vector<sf::Vector2i> empties = game.getEmptySpaces();
			int size = empties.size();
			int move = rand() % size;
			if (game.placeStone(empties[move]))
			{
				clock.restart();
				stones.clear();
				moveCount++;
				for (int y = 0; y < BOARD_H; y++)
				{
					for (int x = 0; x < BOARD_W; x++)
					{
						if (game.board[x][y].state == 'B')
						{

							sf::CircleShape shape(10.f);
							shape.setFillColor(sf::Color::Blue);
							shape.setOrigin(shape.getGeometricCenter());
							shape.setPosition({(x * 20.f) + 10, (y * 20.f + 10)});
							stones.push_back(shape);
						}
						else if (game.board[x][y].state == 'W')
						{

							sf::CircleShape shape(10.f);
							shape.setFillColor(sf::Color::Magenta);
							shape.setOrigin(shape.getGeometricCenter());
							shape.setPosition({(x * 20.f) + 10, (y * 20.f + 10)});

							stones.push_back(shape);
						}
					}
				}

				text.setString("Black captures: " + std::to_string(game.black_captures) + "\nWhite captures: " + std::to_string(game.white_captures) + "\nMoves: " + std::to_string(moveCount));
			}
		}

		window.clear();
		for (auto s : stones)
		{
			window.draw(s);
		}
		window.draw(text);

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
