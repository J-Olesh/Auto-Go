#include <SFML/Graphics.hpp>
#include <GoScene.h>

int main()
{
	// todo: does recursive liberty search traverse the whole group every time??
	sf::Font font("../../assets/fonts/inconsolata-lgc/Inconsolata-LGC.otf");
	sf::RenderWindow window(sf::VideoMode({1500, 900}), "SFML works!");
	GoGame game(40, 40);
	GoScene scene = GoScene(window, font, game);
	scene.run();
}