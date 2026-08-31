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