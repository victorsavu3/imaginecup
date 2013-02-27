#ifndef GAME_H_
#define GAME_H_

#include <SFML/Graphics.hpp>

#include <boost/shared_ptr.hpp>

using boost::shared_ptr;

#include "Tilemap.h"

class Game {
public:
	Game();
	virtual ~Game();

	void start();

private:
	shared_ptr<Tilemap> map;
	sf::RenderWindow window;
	sf::Texture background;
};

#endif /* GAME_H_ */
