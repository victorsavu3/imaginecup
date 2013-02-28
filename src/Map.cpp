#include "Map.h"

#include <TmxParser/Tmx.h>

#include "error.h"

Map::Map(std::string file) {
	Tmx::Map map;
	map.ParseFile(file);

	if (map.HasError()) {
		printf("error code: %d\n", map.GetErrorCode());
		printf("error text: %s\n", map.GetErrorText().c_str());

		FAIL("Failed to parse %s", file.c_str());
	}

	//FAIL_ON(!desaturate.loadFromFile("assets/desaturate.sfx", sf::Shader::Vertex), "Failed to load shared");

	tilesets.resize(map.GetNumTilesets());

	//Load tileset images
	for (int i = 0; i < map.GetNumTilesets(); i++) {
		tilesets[i] = getTexture(map.GetTileset(i)->GetImage()->GetSource());

		FAIL_ON(map.GetTileset(i)->GetSpacing() > 0, "Tileset has spacing, not supported");
		FAIL_ON(map.GetTileset(i)->GetMargin() > 0, "Tileset has margin, not supported");
	}

	th = map.GetTileHeight();
	tw = map.GetTileWidth();

	uint16_t n = map.GetNumLayers() + map.GetNumImageLayers();

	layers.resize(n);

	uint16_t i;

	w = h = 0;

	for (i=0; i<map.GetNumLayers(); i++) {
		const Tmx::Layer *layer = map.GetLayer(i);

		layers[layer->GetZOrder()] = new TileLayer(this, layer);

		if(layer->GetHeight() * th > h)
			h = layer->GetHeight() * th;

		if(layer->GetWidth() * tw > w)
			w = layer->GetWidth() * tw;
	}

	for(i=0; i<map.GetNumImageLayers(); i++){
		const Tmx::ImageLayer *layer = map.GetImageLayer(i);

		layers[layer->GetZOrder()] = new ImageLayer(this, layer->GetImage()->GetSource());
	}

	for(i=1;i<=n;i++)
		layers[n - i]->scale = (float)(i + 1) / (n + 2);
}

Map::~Map() {
	uint16_t i;

	for(i=0;i<layers.size();i++)
		delete layers[i];
}

void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	uint16_t i;

	for(i=0;i<layers.size();i++){
		sf::RenderStates state = states;

		state.transform.translate( - position / layers[i]->scale);

		target.draw(*layers[i], state);
	}
}

shared_ptr<sf::Texture> Map::getTexture(std::string file) {

	std::map<std::string, shared_ptr<sf::Texture> >::iterator it = textures.find(file);

	if(it != textures.end())
		return it->second;
	else{
		shared_ptr<sf::Texture> ptr(new sf::Texture);

		INFO("Loading %s", file.c_str());

		FAIL_ON(!ptr->loadFromFile("assets/" + file), "Could not load texture");

		textures[file] = ptr;

		return ptr;
	}
}
