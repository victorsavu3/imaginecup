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

class Player : public sf::Sprite, public b2ContactListener {
public:
	Player(Map* map, b2Vec2 position, uint16_t layer);
	virtual ~Player();

	uint16_t getLayer(){
		return layer;
	}

	void startImpulseLeft();
	void startImpulseRight();
	void stopImpulseLeft();
	void stopImpulseRight();

	void jump();

	bool isGrounded();

	void step(float frame, float time);

	void BeginContact(b2Contact* contact);
	void EndContact(b2Contact* contact);
private:
	b2Body* body;
	b2Fixture* wheel;
	uint16_t layer;
	shared_ptr<sf::Texture> texture;
	Map* map;

	enum State{
		PlayerStanding,
		PlayerRunning,
		PlayerJumping,
		PlayerPreJump,
		PlayerFalling
	} state;

	enum Impulse{
		ImpulseNone,
		ImpulseLeft,
		ImpulseRight,
		ImpulseBoth
	} impulse;

	void doJump();
	b2Vec2 clamp(b2Vec2 what);

	float jumpStart;
	float time;

	void sync();
	void applyImpulse(float frame);

	void setState(State state);

	uint32_t grounded;
};

#endif /* PLAYER_H_ */
