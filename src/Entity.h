#ifndef ENTITY_H_
#define ENTITY_H_

#include <SFML/Graphics.hpp>

#include "Updatable.h"

class Entity : public Updatable, public sf::Drawable{
public:
	virtual ~Entity() {}
};

#endif /* ENTITY_H_ */
