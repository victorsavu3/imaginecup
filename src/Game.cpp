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
	window.setKeyRepeatEnabled(false);

	sf::Vector2f position;

	sf::Clock frame;
	sf::Clock time;

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
					map.player->startImpulseRight();
				else if(event.key.code == sf::Keyboard::A)
					map.player->startImpulseLeft();
				else
					window.close();
			}

			if(event.type == sf::Event::KeyReleased){
				if(event.key.code == sf::Keyboard::D)
					map.player->stopImpulseRight();
				else if(event.key.code == sf::Keyboard::A)
					map.player->stopImpulseLeft();
			}
		}

		window.clear();

		map.step(frame.getElapsedTime().asMilliseconds() / (float) 1000, time.getElapsedTime().asMilliseconds() / (float) 1000);

		frame.restart();

		map.setPosition(position);

		window.draw(map);

		window.display();
	}
}
