#ifndef LAYER_H_
#define LAYER_H_

class Layer;
class TileLayer;
class ImageLayer;
class ObjectLayer;

#include <SFML/Graphics.hpp>

#include <TmxParser/Tmx.h>

#include <boost/shared_ptr.hpp>

using boost::shared_ptr;

#include <SFML/Graphics.hpp>

#include <set>
#include <vector>

#include <stdint.h>

#include "Map.h"
#include "Entity.h"
#include "ContactListener.h"

class Layer : public sf::Drawable, public Updatable{
public:
	enum LayerType{
		Tile,
		Image,
		Object,
		Decoration
	} type;

	Layer(LayerType type) : alpha(1), scale(1), saturation(1), type(type){};
	virtual ~Layer() {};

	virtual void draw(sf::RenderTarget& target, sf::RenderStates state) const = 0;
	virtual void step(float frame, float time) = 0;

	float alpha;
	float scale;
	float saturation;
};

class TileLayer : public Layer{
public:
	TileLayer(Map* map, const Tmx::Layer *layer);
	virtual ~TileLayer() {}

	virtual void draw(sf::RenderTarget& target, sf::RenderStates state) const;
	virtual void step(float frame, float time){}

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
	virtual void step(float frame, float time){}
private:
	shared_ptr<sf::Texture> texture;
	sf::VertexArray array;
};

class ObjectLayer : public Layer{
public:
	ObjectLayer(Map* map, const Tmx::ObjectGroup* layer, const Tmx::Map* tmx);
	virtual ~ObjectLayer() {}

	virtual void draw(sf::RenderTarget& target, sf::RenderStates state) const;
	virtual void step(float frame, float time);

	b2World world;
	Map* map;
private:
	std::set<shared_ptr<Entity> > objects;
	ContactListener listener;
};

class DecorationLayer : public Layer{
public:
	DecorationLayer(Map* map, const Tmx::ObjectGroup* layer, const Tmx::Map* tmx);
	virtual ~DecorationLayer() {}

	virtual void draw(sf::RenderTarget& target, sf::RenderStates state) const;
	virtual void step(float frame, float time);
private:
	sf::VertexArray array;
	uint16_t tilesetID;
	Map* map;
};

#endif /* LAYER_H_ */
