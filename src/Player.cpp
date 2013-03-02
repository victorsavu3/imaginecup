#include "Player.h"

Player::~Player() {
}

Player::Player(Map* map, b2Vec2 position, uint16_t layer) {
	this->map = map;
	this->layer = layer;

	b2BodyDef bodyDef;

	bodyDef.type = b2_dynamicBody;
	bodyDef.position = position;
	bodyDef.fixedRotation = true;

	body = map->world.CreateBody(&bodyDef);

	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(0.47f, 0.75f, b2Vec2(0.5, 0.75), 0);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 0.1f;
	fixtureDef.friction = 0;

	body->CreateFixture(&fixtureDef);

	b2CircleShape circle;
	circle.m_p.Set(0.5, 1.5f);
	circle.m_radius = 0.49;

	fixtureDef.shape = &circle;
	fixtureDef.density = 0.1f;
	fixtureDef.friction = 2.f;
	fixtureDef.userData = this;

	body->CreateFixture(&fixtureDef);

	texture = map->getTexture("player.png");
	this->setTexture(*texture);
}

void Player::impulseLeft() {
	b2Vec2 direction(-10, 0);
	if(body->GetLinearVelocity().x > -10)
		body->ApplyForceToCenter(direction);
}

void Player::impulseRight() {
	b2Vec2 direction(10, 0);
	if(body->GetLinearVelocity().x < 10)
		body->ApplyForceToCenter(direction);
}

void Player::jump() {
	if(isGrounded()){
		b2Vec2 direction;
		if(body->GetLinearVelocity().y > -0.01)
			direction = b2Vec2(0, -250);
		else
			direction = b2Vec2(0, -125);
		body->ApplyForceToCenter(direction);
	}
}

bool Player::isGrounded() {
	b2Contact* contacts = map->world.GetContactList();

	while(contacts != NULL){
		if(contacts->IsTouching()){
			if(contacts->GetFixtureA()->GetUserData() == this || contacts->GetFixtureB()->GetUserData() == this)
				return true;
		}

		contacts = contacts->GetNext();
	}

	return false;
}

void Player::sync() {
	this->setPosition(convert(body->GetPosition(), map));
}
