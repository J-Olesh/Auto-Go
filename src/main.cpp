#include <SFML/Graphics.hpp>
#include <GoScene.h>

int main()
{
	// problem: scnene reset not working, renderer not uppdating. not a game logic issue.
	// check fork for changes, revert until find issue.
	sf::Font font("../../assets/fonts/inconsolata-lgc/Inconsolata-LGC.otf");
	sf::RenderWindow window(sf::VideoMode({1500, 900}), "SFML works!");
	GoGame game(19, 19);
	GoScene scene = GoScene(window, font, game);
	scene.run();
}