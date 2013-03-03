#include "Game.h"

#include "Map.h"

Game::Game() : map("assets/test.tmx"), camera(0,0){

	//window.create(sf::VideoMode(1920, 1080), "Imaginecup", sf::Style::Fullscreen);
	window.create(sf::VideoMode(800, 600), "Imaginecup");
}

Game::~Game() {
}

void Game::start() {
	uint32_t x = 0;

	window.setVerticalSyncEnabled(true);
	window.setKeyRepeatEnabled(false);sf::Vector2f position;

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
				else if(event.key.code == sf::Keyboard::Escape)
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

		float framesecond = frame.getElapsedTime().asMicroseconds() / (float) 1000000;

		map.step(framesecond, time.getElapsedTime().asMilliseconds() / (float) 1000);

		frame.restart();

		map.setPosition(getCamera(framesecond / 1000000));

		window.draw(map);

		window.display();
	}
}

uint16_t Game::getHeigth() {
	return window.getSize().y;
}

uint16_t Game::getWidth() {
	return window.getSize().x;
}

sf::Vector2f Game::getIdealCamera() {
	sf::Vector2f ret = map.player->getPosition();

	if(map.player->getDirection() == Player::Right){
		ret.x = ret.x - getWidth() / 5;
		ret.y = ret.y - 4 * getHeigth() / 5;
	} else {
		ret.x = ret.x - 4 * getWidth() / 5;
		ret.y = ret.y - 4 * getHeigth() / 5;
	}

	if(ret.x < 0)
		ret.x = 0;
	if(ret.y < 0)
		ret.y = 0;

	if(ret.x + getWidth() > map.getWidth())
		ret.x = map.getWidth() - getWidth();

	if(ret.y + getHeigth() > map.getHeight())
		ret.y = map.getHeight() - getHeigth();

	ret /= map.getScale(map.player->getLayer());

	return ret;
}

sf::Vector2f Game::getCamera(float frame) {
	sf::Vector2f delta = getIdealCamera() - camera;

	delta /= (float)10;

	camera += delta;

	return camera;
}
