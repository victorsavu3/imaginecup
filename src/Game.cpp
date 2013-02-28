#include "Game.h"

#include "Map.h"

Game::Game(){

	//window.create(sf::VideoMode(1920, 1080), "Imaginecup", sf::Style::Fullscreen);
	window.create(sf::VideoMode(800, 600), "Imaginecup");
}

Game::~Game() {
}

void Game::start() {
	uint32_t x = 0;

	Map map("assets/test.tmx");

	window.setVerticalSyncEnabled(true);

	sf::Vector2f position;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if(event.type == sf::Event::KeyPressed)
				window.close();
		}

		window.clear();

		position.x++;
		map.setPosition(position);

		window.draw(map);

		window.display();
	}
}
