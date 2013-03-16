#ifndef MAP_H_
#define MAP_H_

class Map;

#include <SFML/Graphics.hpp>

#include <Box2D/Box2D.h>

#include <vector>
#include <string>

#include <stdint.h>

#include "Layer.h"
#include "Player.h"
#include "DebugDraw.h"



class Map : public sf::Drawable{
public:
	Map(std::string file);
	virtual ~Map();

	uint32_t getWidth(){
		return w;
	}

	uint32_t getHeight(){
		return h;
	}

	uint32_t getTileWidth(){
		return tw;
	}

	uint32_t getTileHeight(){
		return th;
	}

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	shared_ptr<sf::Texture> getTexture(std::string file);

	void setPosition(sf::Vector2f position){
		this->position = position;
	}

	void step(float frame, float time);

	float getScale(uint16_t layer);

	shared_ptr<Player> player;

private:
	sf::Vector2f position;
	uint32_t w, h;
	uint32_t tw, th;
	std::vector<shared_ptr<sf::Texture> > tilesets;
	std::vector<Layer*> layers;
	std::map<std::string, shared_ptr<sf::Texture> > textures;
	shared_ptr<sf::Shader> desaturate;

	friend class TileLayer;
	friend class ObjectLayer;
};

#endif /* MAP_H_ */
