#include "PickUp.h"

#include <unistd.h>

#include <set>
#include <fstream>

#include "conversions.h"

/* Global atlas */
shared_ptr<spine::Atlas> atlas;

PickUp::PickUp(ObjectLayer* layer, Type type, std::string name, sf::Vector2f position) : type(type), name(name), position(position), Collider(Collider::PickUp), layer(layer), deleted(false){
	chdir("assets/pickup");

	if(!atlas){
		std::ifstream atlasFile("pack.atlas");
		atlas.reset(new spine::Atlas(atlasFile));
	}

	spine::SkeletonJson skeletonJson(atlas.get());

	std::string skeletonstr = name + "/skeleton-skeleton.json";

	std::ifstream skeletonFile(skeletonstr.c_str());
	skeletonData.reset(skeletonJson.readSkeletonData(skeletonFile));

	std::string animationstr = name + "/skeleton-animation.json";

	std::ifstream animationFile(animationstr.c_str());
	run.reset(skeletonJson.readAnimation(animationFile, skeletonData.get()));

	skeleton.reset(new spine::Skeleton(skeletonData.get()));

	skeleton->flipX = false;
	skeleton->flipY = false;
	skeleton->setToBindPose();
	skeleton->updateWorldTransform();

	b2BodyDef bodyDef;

	bodyDef.type = b2_staticBody;
	bodyDef.position = convert(position, layer->map);

	body = layer->world.CreateBody(&bodyDef);
	body->SetUserData((Collider*)this);

	b2CircleShape circle;
	circle.m_p.Set(0, 0);
	circle.m_radius = 0.5f;

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &circle;
	fixtureDef.density = 0.1f;
	fixtureDef.friction = 0.f;
	fixtureDef.isSensor = true;
	body->CreateFixture(&fixtureDef);

	chdir("../..");
}

void PickUp::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform.translate(position);
	target.draw(*skeleton, states);
}

PickUp::~PickUp() {
	layer->world.DestroyBody(body);
}

void PickUp::step(float frame, float time) {
	run->apply(skeleton.get(), time, true);
	skeleton->updateWorldTransform();

	if(deleted){
		std::set<shared_ptr<Entity> >::iterator it;
		for(it=((ObjectLayer*)layer)->objects.begin(); it != ((ObjectLayer*)layer)->objects.end(); it++){
			if(it->get() == this){
				((ObjectLayer*)layer)->objects.erase(it);
				break;
			}
		}
	}
}
