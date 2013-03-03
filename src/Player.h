#ifndef PLAYER_H_
#define PLAYER_H_

class Player;

#include <SFML/Graphics.hpp>

#include <spine-sfml/spine.h>

#include <Box2D/Box2D.h>

#include <boost/shared_ptr.hpp>

using boost::shared_ptr;

#include <stdint.h>

#include "Map.h"
#include "conversions.h"

class Player : public b2ContactListener, public sf::Drawable {
public:
	Player(Map* map, b2Vec2 position, uint16_t layer);
	virtual ~Player();

	void BeginContact(b2Contact* contact);
	void EndContact(b2Contact* contact);

	virtual void draw(sf::RenderTarget&, sf::RenderStates) const;

	uint16_t getLayer(){
		return layer;
	}

	void startImpulseLeft();
	void startImpulseRight();
	void stopImpulseLeft();
	void stopImpulseRight();

	void jump();

	void step(float frame, float time);

	enum Direction{
		Left,
		Right
	} direction;

	Direction getDirection(){
		return direction;
	}
	const sf::Vector2f& getPosition() const {
		return position;
	}

	void setPosition(const sf::Vector2f& position) {
		this->position = position;
	}

private:
	b2Body* body;
	uint16_t layer;
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
	bool isGrounded();

	float jumpStart;
	float time;

	void sync();
	void applyImpulse(float frame);

	void setState(State state);
	void setImpulse(Impulse impulse);
	void setDirection(Direction direction);

	uint32_t grounded;
	sf::Vector2f position;

	shared_ptr<spine::Atlas> atlas;
	shared_ptr<spine::Skeleton> skeleton;
	shared_ptr<spine::SkeletonData> skeletonData;
	shared_ptr<spine::Animation> run;
};

#endif /* PLAYER_H_ */
