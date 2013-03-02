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
	dynamicBox.SetAsBox(0.5f, 0.75f);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;

	body->CreateFixture(&fixtureDef);

	b2CircleShape circle;
	circle.m_p.Set(0, 0.75f);
	circle.m_radius = 0.5;

	fixtureDef.shape = &circle;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;

	body->CreateFixture(&fixtureDef);

	texture = map->getTexture("player.png");
	this->setTexture(*texture);
	this->setOrigin(32, 24);
}

void Player::impulse(b2Vec2 direction) {
	body->ApplyForceToCenter(direction);
}

void Player::jump() {
	b2Vec2 speed = body->GetLinearVelocity();
	speed.y -= 10;
	body->SetLinearVelocity(speed);
}

void Player::sync() {
	this->setPosition(convert(body->GetPosition(), map));
}
