#ifndef ENTITY_H_
#define ENTITY_H_

#include <SFML/Graphics.hpp>

#include "Updatable.h"

class Entity : public Updatable, public sf::Sprite{
public:
	virtual ~Entity() {}
	virtual void step(float frame, float time) {}
};

#endif /* ENTITY_H_ */
