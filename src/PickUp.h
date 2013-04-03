#ifndef PICKUP_H_
#define PICKUP_H_

class PickUp;

#include "Entity.h"
#include "Layer.h"
#include "Collider.h"

#include <string>

#include <boost/shared_ptr.hpp>

using boost::shared_ptr;

#include <spine-sfml/spine.h>

class PickUp : public Entity, public Collider{
public:
	typedef enum {
		Coin
	} Type;

	PickUp(ObjectLayer* layer, Type type, std::string name, sf::Vector2f position);
	virtual ~PickUp();

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void step(float frame, float time);

	void markDeleted(){
		deleted = true;
	}

	Type type;
private:
	std::string name;

	shared_ptr<spine::Skeleton> skeleton;
	shared_ptr<spine::SkeletonData> skeletonData;
	shared_ptr<spine::Animation> run;

	sf::Vector2f position;

	b2Body* body;
	ObjectLayer* layer;
	bool deleted;
};

#endif /* PICKUP_H_ */
