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

	sf::Clock clock;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if(event.type == sf::Event::KeyPressed){
				if(event.key.code == sf::Keyboard::P)
					map.debugPhysics();
				else if(event.key.code == sf::Keyboard::W)
					map.player->jump();
				else if(event.key.code == sf::Keyboard::D)
					map.player->impulse(b2Vec2(100, 0));
				else if(event.key.code == sf::Keyboard::A)
					map.player->impulse(b2Vec2(-100, 0));
				else
					window.close();
			}
		}

		window.clear();

		map.step(clock.getElapsedTime().asMilliseconds() / (float) 1000);

		clock.restart();

		map.setPosition(position);

		window.draw(map);

		window.display();
	}
}
