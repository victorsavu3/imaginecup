#ifndef PLAYER_H_
#define PLAYER_H_

class Player;

#include <SFML/Graphics.hpp>

#include <Box2D/Box2D.h>

#include <boost/shared_ptr.hpp>

using boost::shared_ptr;

#include <stdint.h>

#include "Map.h"
#include "conversions.h"

class Player : public sf::Sprite{
public:
	Player(Map* map, b2Vec2 position, uint16_t layer);
	virtual ~Player();

	uint16_t getLayer(){
		return layer;
	}

	uint16_t setLayer(){
		return layer;
	}

	void impulse(b2Vec2 direction);
	void jump();

	void sync();

private:
	b2Body* body;
	uint16_t layer;
	shared_ptr<sf::Texture> texture;
	Map* map;
};

#endif /* PLAYER_H_ */
