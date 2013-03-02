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
	fixtureDef.userData = (b2ContactListener*)this;

	body->CreateFixture(&fixtureDef);

	b2CircleShape circle;
	circle.m_p.Set(0.5, 1.5f);
	circle.m_radius = 0.49;

	fixtureDef.shape = &circle;
	fixtureDef.density = 0.1f;
	fixtureDef.friction = 2.f;
	fixtureDef.userData = (b2ContactListener*)this;

	wheel = body->CreateFixture(&fixtureDef);

	b2PolygonShape sensorBox;
	sensorBox.SetAsBox(0.45f, 0.1f, b2Vec2(0.5, 2), 0);

	fixtureDef.shape = &sensorBox;
	fixtureDef.isSensor = true;
	fixtureDef.userData = (b2ContactListener*)this;

	body->CreateFixture(&fixtureDef);

	texture = map->getTexture("player.png");
	this->setTexture(*texture);

	setState(PlayerStanding);
	impulse = ImpulseNone;
	time = 0;
	jumpStart = 0;
	grounded = 0;
}

void Player::jump() {
	setState(PlayerPreJump);
	jumpStart = time;
}

void Player::startImpulseLeft() {
	if(impulse == ImpulseRight)
		impulse = ImpulseBoth;
	else
		impulse = ImpulseLeft;
}

void Player::startImpulseRight() {
	if(impulse == ImpulseLeft)
		impulse = ImpulseBoth;
	else
		impulse = ImpulseRight;
}

void Player::stopImpulseLeft() {
	if(impulse == ImpulseBoth)
		impulse = ImpulseRight;
	else
		impulse = ImpulseNone;
}

void Player::stopImpulseRight() {
	if(impulse == ImpulseBoth)
		impulse = ImpulseLeft;
	else
		impulse = ImpulseNone;
}

void Player::doJump() {
	if(isGrounded()){
		b2Vec2 direction;
		if(body->GetLinearVelocity().y > -0.01)
			direction = b2Vec2(0, -250);
		else
			direction = b2Vec2(0, -125);
		body->ApplyForceToCenter(direction);
	}

	setState(PlayerJumping);
}

b2Vec2 Player::clamp(b2Vec2 what) {
	if(what.x > 10)
		what.x = 10;
	if(what.x < -10)
		what.x = -10;
	if(what.y > 10)
		what.y = -10;
	if(what.y < -10)
		what.y = -10;

	return what;
}

void Player::step(float frame, float time) {
	this->time = time;
	applyImpulse(frame);

	b2Vec2 speed = clamp(body->GetLinearVelocity());

	sync();

	if(speed.y > 1 && !isGrounded()) {
		setState(PlayerFalling);
	} else if(state == PlayerFalling && speed.y > -0.1){
		setState(PlayerStanding);
	} else if(state == PlayerPreJump){
		if(time > jumpStart + 0.05)
			doJump();
	} else if(state == PlayerRunning && (impulse == ImpulseNone || impulse == ImpulseBoth) && speed.x < 0.1 && speed.x > -0.1){
		setState(PlayerStanding);
	}
}

void Player::sync() {
	this->setPosition(convert(body->GetPosition(), map));
}

void Player::BeginContact(b2Contact* contact) {
	if(contact->GetFixtureA()->IsSensor() || contact->GetFixtureB()->IsSensor()){
		grounded++;
		std::cout<<"BeginContact "<<grounded<<std::endl;
	}
}

void Player::EndContact(b2Contact* contact) {
	if(contact->GetFixtureA()->IsSensor() || contact->GetFixtureB()->IsSensor()){
		grounded--;
		std::cout<<"EndContact "<<grounded<<std::endl;
	}
}

bool Player::isGrounded() {
	return grounded > 0;
}

void Player::applyImpulse(float frame) {
	if(impulse != ImpulseNone && impulse != ImpulseBoth){
		b2Vec2 direction(0,0);
		float amount;

		b2Vec2 speed = body->GetLinearVelocity();

		switch(state){
		case PlayerPreJump:
			amount = 10;
			break;
		case PlayerStanding:
		case PlayerRunning:
			amount = 1000;
			break;
		case PlayerJumping:
			amount = 500;
			break;
		case PlayerFalling:
			amount = 100;
			break;
		}

		switch(impulse){
		case ImpulseLeft:
			if(speed.x > -5)
				direction.x = -amount*frame;
			break;
		case ImpulseRight:
			if(speed.x < 5)
				direction.x = amount*frame;
			break;
		}

		body->ApplyForceToCenter(direction);

		if(state == PlayerStanding)
			setState(PlayerRunning);
	}
}

void Player::setState(State state) {
	if(this->state == state)
		return;

	switch(state){
	case PlayerPreJump:
		std::cout<<"PlayerPreJump"<<std::endl;
		break;
	case PlayerJumping:
		std::cout<<"PlayerJumping"<<std::endl;
		break;
	case PlayerFalling:
		std::cout<<"PlayerFalling"<<std::endl;
		break;
	case PlayerStanding:
		std::cout<<"PlayerStanding"<<std::endl;
		break;
	case PlayerRunning:
		std::cout<<"PlayerRunning"<<std::endl;
		break;
	}
	this->state = state;
}
