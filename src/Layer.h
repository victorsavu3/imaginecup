#ifndef LAYER_H_
#define LAYER_H_

class Layer;

#include <SFML/Graphics.hpp>

#include <TmxParser/Tmx.h>

#include <boost/shared_ptr.hpp>

using boost::shared_ptr;

#include <SFML/Graphics.hpp>

#include <set>
#include <vector>

#include <stdint.h>

#include "Map.h"

class Layer : public sf::Drawable{
public:
	Layer() : alpha(1), scale(1), color(1){};
	virtual ~Layer() {};

	virtual void draw(sf::RenderTarget& target, sf::RenderStates state) const = 0;

	float alpha;
	float scale;
	float color;
};

class TileLayer : public Layer{
public:
	TileLayer(Map* map, const Tmx::Layer *layer);
	virtual ~TileLayer() {}

	virtual void draw(sf::RenderTarget& target, sf::RenderStates state) const;

	friend class Map; /* allow the map to fill out the vertex arrays */
private:
	Map* map;
	std::vector<sf::VertexArray> arrays;
};

class ImageLayer : public Layer{
public:
	ImageLayer(Map* map, std::string file);
	virtual ~ImageLayer() {}

	virtual void draw(sf::RenderTarget& target, sf::RenderStates state) const;
private:
	shared_ptr<sf::Texture> texture;
	sf::VertexArray array;
};

class ObjectLayer : public Layer{
public:
	ObjectLayer() {}
	virtual ~ObjectLayer() {}

	virtual void draw(sf::RenderTarget& target, sf::RenderStates state) const {}
};

#endif /* LAYER_H_ */
