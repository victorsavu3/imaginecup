#include "Map.h"

#include <TmxParser/Tmx.h>

#include "error.h"

Map::Map(std::string file){
	Tmx::Map map;
	map.ParseFile(file);

	if (map.HasError()) {
		printf("error code: %d\n", map.GetErrorCode());
		printf("error text: %s\n", map.GetErrorText().c_str());

		FAIL("Failed to parse %s", file.c_str());
	}

	desaturate.reset(new sf::Shader);
	FAIL_ON(!desaturate->loadFromFile("assets/desaturate.glfs", sf::Shader::Fragment), "Failed to load shared");

	tilesets.resize(map.GetNumTilesets());

	//Load tileset images
	for (int i = 0; i < map.GetNumTilesets(); i++) {
		tilesets[i] = getTexture(map.GetTileset(i)->GetImage()->GetSource());

		FAIL_ON(map.GetTileset(i)->GetSpacing() > 0, "Tileset has spacing, not supported");
		FAIL_ON(map.GetTileset(i)->GetMargin() > 0, "Tileset has margin, not supported");
	}

	th = map.GetTileHeight();
	tw = map.GetTileWidth();

	uint16_t n = map.GetNumLayers() + map.GetNumImageLayers() + map.GetNumObjectGroups();

	layers.resize(n);

	uint16_t i, j, k;

	w = h = 0;

	for (i=0; i<map.GetNumLayers(); i++) {
		const Tmx::Layer *layer = map.GetLayer(i);

		layers[layer->GetZOrder()] = new TileLayer(this, layer);

		if(layer->GetHeight() * th > h)
			h = layer->GetHeight() * th;

		if(layer->GetWidth() * tw > w)
			w = layer->GetWidth() * tw;

		FAIL_ON(!layer->GetProperties().HasProperty("Scale"), "Layer must have 'Scale' property");
		layers[layer->GetZOrder()]->scale = layer->GetProperties().GetFloatProperty("Scale");
		layers[layer->GetZOrder()]->alpha = layer->GetOpacity();

		if(layer->GetProperties().HasProperty("Saturation"))
			layers[layer->GetZOrder()]->saturation = layer->GetProperties().GetFloatProperty("Saturation");
		else
			layers[layer->GetZOrder()]->saturation = layers[layer->GetZOrder()]->scale;

		FAIL_ON(layers[layer->GetZOrder()]->saturation > 1 || layers[layer->GetZOrder()]->saturation < 0, "Saturation is between 0 and 1.0, actual is %f on layer %d", layers[layer->GetZOrder()]->saturation, layer->GetZOrder());
		FAIL_ON(layers[layer->GetZOrder()]->scale > 1 || layers[layer->GetZOrder()]->scale < 0, "Scale is between 0 and 1.0, actual is %f on layer %d", layers[layer->GetZOrder()]->scale, layer->GetZOrder());
	}

	for(i=0; i<map.GetNumImageLayers(); i++){
		const Tmx::ImageLayer *layer = map.GetImageLayer(i);

		layers[layer->GetZOrder()] = new ImageLayer(this, layer->GetImage()->GetSource());

		FAIL_ON(!layer->GetProperties().HasProperty("Scale"), "Layer must have 'Scale' property");
		layers[layer->GetZOrder()]->scale = layer->GetProperties().GetFloatProperty("Scale");
		layers[layer->GetZOrder()]->alpha = layer->GetOpacity();

		if(layer->GetProperties().HasProperty("Saturation"))
			layers[layer->GetZOrder()]->saturation = layer->GetProperties().GetFloatProperty("Saturation");
		else
			layers[layer->GetZOrder()]->saturation = layers[layer->GetZOrder()]->scale;

		FAIL_ON(layers[layer->GetZOrder()]->saturation > 1 || layers[layer->GetZOrder()]->saturation < 0, "Saturation is between 0 and 1.0, actual is %f on layer %d", layers[layer->GetZOrder()]->saturation, layer->GetZOrder());
				FAIL_ON(layers[layer->GetZOrder()]->scale > 1 || layers[layer->GetZOrder()]->scale < 0, "Scale is between 0 and 1.0, actual is %f on layer %d", layers[layer->GetZOrder()]->scale, layer->GetZOrder());
	}

	for(i=0; i<map.GetNumObjectGroups(); i++){
		const Tmx::ObjectGroup *layer = map.GetObjectGroup(i);

		FAIL_ON(!layer->GetProperties().HasProperty("Type"), "Object layers must have a Type");

		if(layer->GetProperties().GetLiteralProperty("Type") == "Playable")
			layers[layer->GetZOrder()] = new ObjectLayer(this, layer, &map);
		else if(layer->GetProperties().GetLiteralProperty("Type") == "Decoration")
			layers[layer->GetZOrder()] = new DecorationLayer(this, layer, &map);

		FAIL_ON(!layer->GetProperties().HasProperty("Scale"), "Layer must have 'Scale' property");
		layers[layer->GetZOrder()]->scale = layer->GetProperties().GetFloatProperty("Scale");
		layers[layer->GetZOrder()]->alpha = layer->GetOpacity();

		if(layer->GetProperties().HasProperty("Saturation"))
			layers[layer->GetZOrder()]->saturation = layer->GetProperties().GetFloatProperty("Saturation");
		else
			layers[layer->GetZOrder()]->saturation = layers[layer->GetZOrder()]->scale;

		FAIL_ON(layers[layer->GetZOrder()]->saturation > 1 || layers[layer->GetZOrder()]->saturation < 0, "Saturation is between 0 and 1.0");
		FAIL_ON(layers[layer->GetZOrder()]->scale > 1 || layers[layer->GetZOrder()]->scale < 0, "Scale is between 0 and 1.0");
	}


	FAIL_ON(!player, "Missing Player");
}

Map::~Map() {
	player.reset();

	uint16_t i;

	for(i=0;i<layers.size();i++)
		delete layers[i];
}

void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	uint16_t i;

	for(i=0;i<layers.size();i++){
		sf::RenderStates state = states;

		desaturate->setParameter("saturation", layers[i]->saturation);
		desaturate->setParameter("alpha", layers[i]->alpha);

		state.transform.translate( - position * layers[i]->scale);
		state.shader = &*desaturate;

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

void Map::step(float frame, float time) {
	uint16_t i;

	for(i=0;i<layers.size();i++){
		layers[i]->step(frame, time);
	}
}

float Map::getScale(uint16_t layer) {
	return layers[layer]->scale;
}
