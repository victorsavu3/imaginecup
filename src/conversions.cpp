#include "conversions.h"

b2Vec2 convert(const Tmx::Point& from, Map* const map){
	return b2Vec2((float)from.x / map->getTileWidth(), (float)from.y / map->getTileHeight());
}

b2Vec2 convert(const sf::Vector2f& from, shared_ptr<Map> map){
	return b2Vec2(from.x / map->getTileWidth(), from.y / map->getTileHeight());
}

sf::Vector2f convert(const b2Vec2& from, shared_ptr<Map> map){
	return sf::Vector2f(from.x * map->getTileWidth(), from.y * map->getTileHeight());
}

b2Vec2 convert(const sf::Vector2f& from, Map* map){
	return b2Vec2(from.x / map->getTileWidth(), from.y / map->getTileHeight());
}

sf::Vector2f convert(const b2Vec2& from, Map* map){
	return sf::Vector2f(from.x * map->getTileWidth(), from.y * map->getTileHeight());
}
