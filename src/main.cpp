#include <SFML/Graphics.hpp>
#include <GoScene.h>

int main()
{
	
	// todo: move overall stats out of scene, iterate through board sizes here
	sf::Font font("../../assets/fonts/inconsolata-lgc/Inconsolata-LGC.otf");
	sf::RenderWindow window(sf::VideoMode({1500, 900}), "SFML works!");
	GoGame game(9, 9);
	GoScene scene = GoScene(window, font, game);
	scene.run();
}