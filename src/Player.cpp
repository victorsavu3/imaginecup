#include "Player.h"

#include <fstream>

#include "error.h"

#include "PickUp.h"

#define Y_OFFSET 400
#define X_OFFSET 80

Player::~Player() {
	((ObjectLayer*)map->layers[layer])->world.DestroyBody(body);
}

Player::Player(ObjectLayer* layer, b2Vec2 position, uint16_t location) : layer(location), Collider(Collider::Player), inPortal(false), portalDirection(Portal::Up){
	this->map = layer->map;

	b2BodyDef bodyDef;

	bodyDef.type = b2_dynamicBody;
	bodyDef.position = position;
	bodyDef.fixedRotation = true;

	body = layer->world.CreateBody(&bodyDef);
	body->SetUserData((Collider*)this);

	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(0.47f, 0.75f, b2Vec2(0.5, 0.75), 0);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 0.1f;
	fixtureDef.friction = 0;
	fixtureDef.userData = (b2ContactListener*)this;

	body->CreateFixture(&fixtureDef);

	b2CircleShape circle;
	circle.m_p.Set(0.5f, 1.5f);
	circle.m_radius = 0.49f;

	fixtureDef.shape = &circle;
	fixtureDef.density = 0.1f;
	fixtureDef.friction = 0.f;
	fixtureDef.userData = (b2ContactListener*)this;

	body->CreateFixture(&fixtureDef);

	b2CircleShape circlef;
	circlef.m_p.Set(0.5f, 1.9f);
	circlef.m_radius = 0.1;

	fixtureDef.shape = &circlef;
	fixtureDef.density = 0.1f;
	fixtureDef.friction = 2.f;
	fixtureDef.userData = (b2ContactListener*)this;

	body->CreateFixture(&fixtureDef);

	b2PolygonShape sensorBox;
	sensorBox.SetAsBox(0.45f, 0.1f, b2Vec2(0.5f, 2.f), 0);

	fixtureDef.shape = &sensorBox;
	fixtureDef.isSensor = true;
	fixtureDef.userData = (b2ContactListener*)this;

	body->CreateFixture(&fixtureDef);

	state = PlayerStanding;
	impulse = ImpulseNone;
	direction = Right;

	time = 0;
	jumpStart = 0;
	grounded = 0;

	std::ifstream atlasFile("assets/spineboy.atlas");
	atlas.reset(new spine::Atlas(atlasFile));

	spine::SkeletonJson skeletonJson(atlas.get());

	std::ifstream skeletonFile("assets/spineboy-skeleton.json");
	skeletonData.reset(skeletonJson.readSkeletonData(skeletonFile));

	std::ifstream animationFile("assets/spineboy-walk.json");
	run.reset(skeletonJson.readAnimation(animationFile, skeletonData.get()));

	skeleton.reset(new spine::Skeleton(skeletonData.get()));

	skeleton->flipX = false;
	skeleton->flipY = false;
	skeleton->setToBindPose();
	skeleton->getRootBone()->x = X_OFFSET;
	skeleton->getRootBone()->y = Y_OFFSET;
	skeleton->updateWorldTransform();

	sync();
}

void Player::jump() {
	if(inPortal){
		int32_t to = layer;

		if(portalDirection == Portal::Up){
			to++;
			while(to < map->layers.size()){
				if(map->layers[to]->type == Layer::Object)
					break;
				to++;
			}

			FAIL_ON(to == map->layers.size(), "No layer found going up");

		} else {
			to--;
			while(to > 0 ){
				if(map->layers[to]->type == Layer::Object)
					break;
				to--;
			}

			FAIL_ON(to < 0, "No layer found going down");
		}

		ObjectLayer* layerO = (ObjectLayer*)map->layers[to];
		Map* map = this->map; /* avoid use after free as this class is deleted */

		shared_ptr<Player> player = shared_ptr<Player>(new Player(layerO, body->GetPosition(), to));


		((ObjectLayer*)map->layers[layer])->objects.erase(map->player);

		map->player = player;

		layerO->objects.insert(map->player);

	} else {
		setState(PlayerPreJump);
		jumpStart = time;
	}
}

void Player::startImpulseLeft() {
	if(impulse == ImpulseRight)
		setImpulse(ImpulseBoth);
	else
		setImpulse(ImpulseLeft);
}

void Player::startImpulseRight() {
	if(impulse == ImpulseLeft)
		setImpulse(ImpulseBoth);
	else
		setImpulse(ImpulseRight);
}

void Player::stopImpulseLeft() {
	if(impulse == ImpulseBoth)
		setImpulse(ImpulseRight);
	else
		setImpulse(ImpulseNone);
}

void Player::stopImpulseRight() {
	if(impulse == ImpulseBoth)
		setImpulse(ImpulseLeft);
	else
		setImpulse(ImpulseNone);
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

void Player::step(float frame, float time) {
	this->time = time;
	applyImpulse(frame);

	run->apply(skeleton.get(), time, true);
	skeleton->updateWorldTransform();

	b2Vec2 speed = body->GetLinearVelocity();

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
	}

	Collider* collider = (Collider*)contact->GetFixtureA()->GetBody()->GetUserData();
	if(collider->type == Collider::Player)
		collider = (Collider*)contact->GetFixtureB()->GetBody()->GetUserData();

	switch(collider->type){
	case Collider::Portal:
	{
			inPortal = true;

			::Portal* portal = (::Portal*)collider;
			portalDirection = portal->direction;
	}
		break;
	case Collider::Environment:
		break;
	case Collider::PickUp:
		((::PickUp*)collider)->markDeleted();
		break;
	}
}

void Player::EndContact(b2Contact* contact) {
	if(contact->GetFixtureA()->IsSensor() || contact->GetFixtureB()->IsSensor()){
		grounded--;
	}

	Collider* collider = (Collider*)contact->GetFixtureA()->GetBody()->GetUserData();

	switch(collider->type){
	case Collider::Portal:
		inPortal = false;
		break;
	case Collider::Environment:
		break;
	case Collider::Player:
		break;
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
			setDirection(Left);
			break;
		case ImpulseRight:
			if(speed.x < 5)
				direction.x = amount*frame;
			setDirection(Right);
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
		DBG("Player state becomes: PlayerPreJump");
		break;
	case PlayerJumping:
		DBG("Player state becomes: PlayerJumping");
		break;
	case PlayerFalling:
		DBG("Player state becomes: PlayerFalling");
		break;
	case PlayerStanding:
		DBG("Player state becomes: PlayerStanding");
		break;
	case PlayerRunning:
		DBG("Player state becomes: PlayerRunning");
		break;
	}

	this->state = state;
}

void Player::setImpulse(Impulse impulse) {
	if(this->impulse == impulse)
		return;

	switch(impulse){
	case ImpulseNone:
		DBG("Player state becomes: ImpulseNone");
		break;
	case ImpulseLeft:
		DBG("Player state becomes: ImpulseLeft");
		break;
	case ImpulseRight:
		DBG("Player state becomes: ImpulseRight");
		break;
	case ImpulseBoth:
		DBG("Player state becomes: ImpulseBoth");
		break;
	}

	this->impulse = impulse;
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates renderStates) const {
	renderStates.transform.translate(position);
	renderStates.transform.scale(0.8, 0.8);
	target.draw(*skeleton, renderStates);
}

void Player::setDirection(Direction direction) {
	if(this->direction == direction)
		return;

	switch(direction){
	case Right:
		DBG("Direction becomes: Right");
		skeleton->flipX = false;
		skeleton->setToBindPose();
		skeleton->getRootBone()->y = Y_OFFSET;
		skeleton->getRootBone()->x = X_OFFSET;
		skeleton->updateWorldTransform();
		break;
	case Left:
		DBG("Direction becomes: Left");
		skeleton->flipX = true;
		skeleton->setToBindPose();
		skeleton->getRootBone()->y = Y_OFFSET;
		skeleton->getRootBone()->x = X_OFFSET;
		skeleton->updateWorldTransform();
		break;
	}

	this->direction = direction;
}
