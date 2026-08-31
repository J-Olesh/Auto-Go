#include <SFML/Graphics.hpp>
#include <GoGame.h>
#include <iostream>
#include <algorithm>
#include <random>
#include <ranges>
#include <GoScene.h>

int main()
{
	sf::Font font("../../assets/fonts/inconsolata-lgc/Inconsolata-LGC.otf");
	sf::RenderWindow window(sf::VideoMode({1500, 900}), "SFML works!");
	GoScene scene = GoScene(window, font, 19, 19);

	scene.run();
}

// can get rid of this after the timer logic is moved to the Scene
void oldmain()
{
	/*
	GoScene scene;
	scene.run();
	*/

	sf::Font font("../../assets/fonts/inconsolata-lgc/Inconsolata-LGC.otf");
	sf::Text text(font); // a font is required to make a text object
	text.setPosition({90.f, 650.f});

	// set the character size
	text.setCharacterSize(24);

	// set the color
	text.setFillColor(sf::Color::Red);

	// set the text style
	text.setStyle(sf::Text::Bold);
	sf::Text timeText(font);
	timeText.setPosition({90.f, 625.f});
	timeText.setCharacterSize(20);
	timeText.setString("time: ");
	sf::Clock clock;
	sf::Clock timer;
	timer.reset();
	sf::RenderWindow window(sf::VideoMode({800, 800}), "SFML works!");
	sf::Vector2i lastMove;
	int BOARD_W = 19;
	int BOARD_H = 19;
	GoGame game = GoGame(BOARD_W, BOARD_H);

	vector<sf::CircleShape> stones;
	bool autoPlay = false;
	bool pause = true;
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
				if (timer.isRunning())
				{
					timer.stop();
				}
				else
				{
					timer.start();
				}
				pause = !pause;
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Up))
			{
				for (auto move : moves)
				{
					game.placeStone(move);
					stones.clear();

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

					text.setString("Black captures: " + std::to_string(game.black_captures) + "\nWhite captures: " + std::to_string(game.white_captures) + "\nMoves: " + std::to_string(game.moveCount));
				}
			}
		}
		sf::Time elapsed = clock.getElapsedTime();

		if (elapsed.asMilliseconds() > 50 && !pause)
		{
			sf::Time totalRunTime = timer.getElapsedTime();
			timeText.setString("time: " + std::to_string(totalRunTime.asSeconds()) + "s");
			vector<sf::Vector2i> empties = game.getEmptySpaces();
			auto rd = std::random_device{};
			auto rng = std::default_random_engine{rd()};

			std::shuffle(std::begin(empties), std::end(empties), rng);
			bool noValidMoves = false;
			while (!game.placeStone(empties.back()))
			{
				if (empties.size() == 1)
				{
					pause = true;
					timer.stop();
					noValidMoves = true;
					break;
				}
				empties.pop_back();
			}
			if (!noValidMoves)
			{
				lastMove = empties.back();
			}

			clock.restart();
			stones.clear();

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
					if (lastMove.x == x && lastMove.y == y)
					{
						sf::CircleShape marker(3.f);
						marker.setFillColor(sf::Color::White);
						marker.setOutlineColor(sf::Color::White);
						marker.setOutlineThickness(2.f);
						marker.setOrigin(marker.getGeometricCenter());
						marker.setPosition({(x * 20.f) + 10, (y * 20.f + 10)});
						stones.push_back(marker);
					}
				}
			}

			text.setString("Black captures: " + std::to_string(game.black_captures) + "\nWhite captures: " + std::to_string(game.white_captures) + "\nMoves: " + std::to_string(game.moveCount));
		}

		window.clear();
		for (auto s : stones)
		{
			window.draw(s);
		}
		window.draw(text);
		window.draw(timeText);

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
