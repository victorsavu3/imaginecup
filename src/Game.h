#ifndef GAME_H_
#define GAME_H_

#include <SFML/Graphics.hpp>

#include <boost/shared_ptr.hpp>

using boost::shared_ptr;

#include <stdint.h>

#include "Map.h"

class Game {
public:
	Game();
	virtual ~Game();

	void start();

	uint16_t getHeigth();
	uint16_t getWidth();

	sf::Vector2f getIdealCamera();
	sf::Vector2f getCamera(float frame);
private:
	sf::RenderWindow window;
	sf::Texture background;
	sf::Vector2f camera;
	Map map;
};

#endif /* GAME_H_ */
