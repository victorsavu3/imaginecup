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
#include "Entity.h"
#include "Layer.h"
#include "Portal.h"


class Player : public b2ContactListener, public Entity, public Collider {
public:
	Player(ObjectLayer* layer, b2Vec2 position, uint16_t location);
	virtual ~Player();

	void BeginContact(b2Contact* contact);
	void EndContact(b2Contact* contact);

	virtual void draw(sf::RenderTarget&, sf::RenderStates) const;
	virtual void step(float frame, float time);

	void startImpulseLeft();
	void startImpulseRight();
	void stopImpulseLeft();
	void stopImpulseRight();

	void jump();

	enum Direction{
		Left,
		Right
	} direction;

	Direction getDirection(){
		return direction;
	}

	uint16_t getLayer() const {
		return layer;
	}

	const sf::Vector2f& getPosition() const {
		return position;
	}

	void setPosition(const sf::Vector2f& position) {
		this->position = position;
	}

private:
	uint16_t layer;
	b2Body* body;
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

	shared_ptr<spine::Atlas> atlas;
	shared_ptr<spine::Skeleton> skeleton;
	shared_ptr<spine::SkeletonData> skeletonData;
	shared_ptr<spine::Animation> run;

	bool inPortal;
	Portal::Direction portalDirection;

	sf::Vector2f position;
};

#endif /* PLAYER_H_ */
