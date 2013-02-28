#ifndef GAME_H_
#define GAME_H_

#include <SFML/Graphics.hpp>

#include <boost/shared_ptr.hpp>

using boost::shared_ptr;

class Game {
public:
	Game();
	virtual ~Game();

	void start();

private:
	sf::RenderWindow window;
	sf::Texture background;
};

#endif /* GAME_H_ */
