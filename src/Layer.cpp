#include "Layer.h"

Layer::Layer(uint16_t id, shared_ptr<Tilemap> tilemap) : tilemap(tilemap), id(id){
	scale = (float)(id + 1) / (tilemap->getLayerCount() + 2);
}

Layer::~Layer() {
}

void Layer::draw(sf::RenderTarget& target, float x, float y) {
	sf::RenderStates states;
	states.transform.translate(-(float)x * scale, -(float)y * scale);

	if(id > 0)
		tilemap->drawLayer(target, states, id - 1);

	std::set<shared_ptr<sf::Sprite> >::iterator it;
	for(it = sprites.begin(); it != sprites.end(); it++)
		target.draw(**it, states);
}
