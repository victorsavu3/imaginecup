#include "Game.h"

#include "Layer.h"

Game::Game() : 	map(new Tilemap("assets/test.tmx")){

	window.create(sf::VideoMode(1920, 1080), "Imaginecup", sf::Style::Fullscreen);

	background.loadFromFile("assets/bg.png");
}

Game::~Game() {
}

void Game::start() {
	uint32_t x = 0;

	Layer bg(0, map);

	shared_ptr<sf::Sprite> bgs1(new sf::Sprite);

	bgs1->setPosition(0, 0);
	bgs1->setTexture(background);

	bg.addSprite(bgs1);

	shared_ptr<sf::Sprite> bgs2(new sf::Sprite);

	bgs2->setPosition(600, 0);
	bgs2->setTexture(background);

	bg.addSprite(bgs2);

	shared_ptr<sf::Sprite> bgs3(new sf::Sprite);

	bgs3->setPosition(0, 600.);
	bgs3->setTexture(background);

	bg.addSprite(bgs3);

	shared_ptr<sf::Sprite> bgs4(new sf::Sprite);

	bgs4->setPosition(600, 600.);
	bgs4->setTexture(background);

	bg.addSprite(bgs4);

	sf::Clock clock;

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

		x = clock.getElapsedTime().asMilliseconds() / (float)10;

		bg.draw(window, x, 0);
		Layer(1, map).draw(window, x, 0);
		Layer(2, map).draw(window, x, 0);
		Layer(3, map).draw(window, x, 0);

		window.display();
	}
}
