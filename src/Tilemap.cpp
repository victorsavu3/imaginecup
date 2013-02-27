#include "Tilemap.h"

#include <TmxParser/Tmx.h>

#include "error.h"

Tilemap::Tilemap(std::string file) {
	Tmx::Map map;
	map.ParseFile(file);

	if (map.HasError()) {
		printf("error code: %d\n", map.GetErrorCode());
		printf("error text: %s\n", map.GetErrorText().c_str());

		FAIL("Failed to parse %s", file.c_str());
	}

	tilesets.resize(map.GetNumTilesets());

	//Load tileset images
	for (int i = 0; i < map.GetNumTilesets(); i++) {
		tilesets[i].loadFromFile("assets/" + map.GetTileset(i)->GetImage()->GetSource());

		FAIL_ON(map.GetTileset(i)->GetSpacing() > 0, "Tileset has spacing, not supported");
		FAIL_ON(map.GetTileset(i)->GetMargin() > 0, "Tileset has margin, not supported");
	}

	uint16_t tw, th;

	th = map.GetTileHeight();
	tw = map.GetTileWidth();

	for (uint16_t i = 0; i < map.GetNumLayers(); i++) {
		layers.push_back(std::vector<sf::VertexArray>(map.GetNumTilesets(), sf::VertexArray(sf::Quads)));

		const Tmx::Layer *layer = map.GetLayer(i);

		uint16_t x,y;

		for (y = 0;	y < layer->GetHeight(); y++)
			for (x = 0;	x < layer->GetWidth(); x++) {
				Tmx::MapTile tile = layer->GetTile(x, y);
				uint16_t id = tile.id;

				if (tile.tilesetId < 0) /* No tile here */
					continue;

				const Tmx::Tileset * tileset = map.GetTileset(tile.tilesetId);

				uint16_t tpw = tilesets[tile.tilesetId].getSize().x / tw;

				uint16_t ox, oy;

				ox = (id % tpw) * tw;
				oy = (id / tpw) * th;

				sf::Vertex vertex;

				/* fliping is done via texture coordinates */

				if(!tile.flippedVertically && !tile.flippedHorizontally){
					vertex.position = sf::Vector2f(x * tw, y * th);
					vertex.texCoords = sf::Vector2f(ox, oy);
					layers[i][tile.tilesetId].append(vertex);

					vertex.position = sf::Vector2f(x * tw + tw, y * th);
					vertex.texCoords = sf::Vector2f(ox + tw, oy);
					layers[i][tile.tilesetId].append(vertex);

					vertex.position = sf::Vector2f(x * tw + tw, y * th + th);
					vertex.texCoords = sf::Vector2f(ox + tw, oy + th);
					layers[i][tile.tilesetId].append(vertex);

					vertex.position = sf::Vector2f(x * tw, y * th + th);
					vertex.texCoords = sf::Vector2f(ox, oy + th);
					layers[i][tile.tilesetId].append(vertex);
				} else if(!tile.flippedVertically && tile.flippedHorizontally){
					vertex.position = sf::Vector2f(x * tw, y * th);
					vertex.texCoords = sf::Vector2f(ox + tw, oy);
					layers[i][tile.tilesetId].append(vertex);

					vertex.position = sf::Vector2f(x * tw + tw, y * th);
					vertex.texCoords = sf::Vector2f(ox, oy);
					layers[i][tile.tilesetId].append(vertex);

					vertex.position = sf::Vector2f(x * tw + tw, y * th + th);
					vertex.texCoords = sf::Vector2f(ox, oy + th);
					layers[i][tile.tilesetId].append(vertex);

					vertex.position = sf::Vector2f(x * tw, y * th + th);
					vertex.texCoords = sf::Vector2f(ox + tw, oy + th);
					layers[i][tile.tilesetId].append(vertex);
				} else if (tile.flippedVertically && tile.flippedHorizontally){
					vertex.position = sf::Vector2f(x * tw, y * th);
					vertex.texCoords = sf::Vector2f(ox + tw, oy + th);
					layers[i][tile.tilesetId].append(vertex);

					vertex.position = sf::Vector2f(x * tw + tw, y * th);
					vertex.texCoords = sf::Vector2f(ox, oy + th);
					layers[i][tile.tilesetId].append(vertex);

					vertex.position = sf::Vector2f(x * tw + tw, y * th + th);
					vertex.texCoords = sf::Vector2f(ox, oy);
					layers[i][tile.tilesetId].append(vertex);

					vertex.position = sf::Vector2f(x * tw, y * th + th);
					vertex.texCoords = sf::Vector2f(ox + tw, oy);
					layers[i][tile.tilesetId].append(vertex);
				} else if (tile.flippedVertically && !tile.flippedHorizontally){
					vertex.position = sf::Vector2f(x * tw, y * th);
					vertex.texCoords = sf::Vector2f(ox, oy + th);
					layers[i][tile.tilesetId].append(vertex);

					vertex.position = sf::Vector2f(x * tw + tw, y * th);
					vertex.texCoords = sf::Vector2f(ox + tw, oy + th);
					layers[i][tile.tilesetId].append(vertex);

					vertex.position = sf::Vector2f(x * tw + tw, y * th + th);
					vertex.texCoords = sf::Vector2f(ox + tw, oy);
					layers[i][tile.tilesetId].append(vertex);

					vertex.position = sf::Vector2f(x * tw, y * th + th);
					vertex.texCoords = sf::Vector2f(ox, oy);
					layers[i][tile.tilesetId].append(vertex);
				}
			}
	}
}

Tilemap::~Tilemap() {
}

void Tilemap::drawLayer(sf::RenderTarget& target, sf::RenderStates states, uint16_t layer) const {
	uint16_t i;
	for(i=0;i<tilesets.size();i++){
		states.texture = &tilesets[i];

		target.draw(layers[layer][i], states);
	}
}
