#ifndef LAYER_H_
#define LAYER_H_

#include <SFML/Graphics.hpp>

#include <boost/shared_ptr.hpp>

using boost::shared_ptr;

#include <set>
#include <vector>

#include <stdint.h>

#include "Tilemap.h"

class Layer {
public:
	Layer(uint16_t id, shared_ptr<Tilemap> tilemap);
	virtual ~Layer();

	void draw(sf::RenderTarget& target, float x, float y);

	void addSprite(shared_ptr<sf::Sprite> sprite){
		sprites.insert(sprite);
	}

	void removeSprite(shared_ptr<sf::Sprite> sprite){
		sprites.erase(sprite);
	}
private:
	uint16_t id;
	float scale;
	shared_ptr<Tilemap> tilemap;
	std::set<shared_ptr<sf::Sprite> > sprites;
};

#endif /* LAYER_H_ */
