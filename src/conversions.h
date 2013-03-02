#ifndef CONVERSIONS_H_
#define CONVERSIONS_H_

#include <SFML/Graphics.hpp>

#include <Box2D/Box2D.h>

#include <TmxParser/Tmx.h>

#include <boost/shared_ptr.hpp>

using boost::shared_ptr;

#include "Map.h"

b2Vec2 convert(const Tmx::Point& from, Map* const map);

b2Vec2 convert(const sf::Vector2f& from, shared_ptr<Map> map);
sf::Vector2f convert(const b2Vec2& from, shared_ptr<Map> map);

b2Vec2 convert(const sf::Vector2f& from, Map* map);
sf::Vector2f convert(const b2Vec2& from, Map* map);


#endif /* CONVERSIONS_H_ */
