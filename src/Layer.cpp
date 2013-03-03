#include "Layer.h"

TileLayer::TileLayer(Map* map, const Tmx::Layer* layer) : Layer(Layer::Tile){
	this->map = map;

	arrays.resize(map->tilesets.size(), sf::VertexArray(sf::Quads));

	uint16_t x, y;

	for (y = 0; y < layer->GetHeight(); y++)
		for (x = 0; x < layer->GetWidth(); x++) {
			Tmx::MapTile tile = layer->GetTile(x, y);
			uint16_t id = tile.id;

			if (tile.tilesetId < 0) /* No tile here */
				continue;

			uint16_t tpw = map->tilesets[tile.tilesetId]->getSize().x / map->tw;

			uint16_t ox, oy;

			ox = (id % tpw) * map->tw;
			oy = (id / tpw) * map->th;

			sf::Vertex vertex;

			/* fliping is done via texture coordinates */

			if (!tile.flippedVertically && !tile.flippedHorizontally) {
				vertex.position = sf::Vector2f(x * map->tw, y * map->th);
				vertex.texCoords = sf::Vector2f(ox, oy);
				arrays[tile.tilesetId].append(vertex);

				vertex.position = sf::Vector2f(x * map->tw + map->tw, y * map->th);
				vertex.texCoords = sf::Vector2f(ox + map->tw, oy);
				arrays[tile.tilesetId].append(vertex);

				vertex.position = sf::Vector2f(x * map->tw + map->tw, y * map->th + map->th);
				vertex.texCoords = sf::Vector2f(ox + map->tw, oy + map->th);
				arrays[tile.tilesetId].append(vertex);

				vertex.position = sf::Vector2f(x * map->tw, y * map->th + map->th);
				vertex.texCoords = sf::Vector2f(ox, oy + map->th);
				arrays[tile.tilesetId].append(vertex);
			} else if (!tile.flippedVertically && tile.flippedHorizontally) {
				vertex.position = sf::Vector2f(x * map->tw, y * map->th);
				vertex.texCoords = sf::Vector2f(ox + map->tw, oy);
				arrays[tile.tilesetId].append(vertex);

				vertex.position = sf::Vector2f(x * map->tw + map->tw, y * map->th);
				vertex.texCoords = sf::Vector2f(ox, oy);
				arrays[tile.tilesetId].append(vertex);

				vertex.position = sf::Vector2f(x * map->tw + map->tw, y * map->th + map->th);
				vertex.texCoords = sf::Vector2f(ox, oy + map->th);
				arrays[tile.tilesetId].append(vertex);

				vertex.position = sf::Vector2f(x * map->tw, y * map->th + map->th);
				vertex.texCoords = sf::Vector2f(ox + map->tw, oy + map->th);
				arrays[tile.tilesetId].append(vertex);
			} else if (tile.flippedVertically && tile.flippedHorizontally) {
				vertex.position = sf::Vector2f(x * map->tw, y * map->th);
				vertex.texCoords = sf::Vector2f(ox + map->tw, oy + map->th);
				arrays[tile.tilesetId].append(vertex);

				vertex.position = sf::Vector2f(x * map->tw + map->tw, y * map->th);
				vertex.texCoords = sf::Vector2f(ox, oy + map->th);
				arrays[tile.tilesetId].append(vertex);

				vertex.position = sf::Vector2f(x * map->tw + map->tw, y * map->th + map->th);
				vertex.texCoords = sf::Vector2f(ox, oy);
				arrays[tile.tilesetId].append(vertex);

				vertex.position = sf::Vector2f(x * map->tw, y * map->th + map->th);
				vertex.texCoords = sf::Vector2f(ox + map->tw, oy);
				arrays[tile.tilesetId].append(vertex);
			} else if (tile.flippedVertically && !tile.flippedHorizontally) {
				vertex.position = sf::Vector2f(x * map->tw, y * map->th);
				vertex.texCoords = sf::Vector2f(ox, oy + map->th);
				arrays[tile.tilesetId].append(vertex);

				vertex.position = sf::Vector2f(x * map->tw + map->tw, y * map->th);
				vertex.texCoords = sf::Vector2f(ox + map->tw, oy + map->th);
				arrays[tile.tilesetId].append(vertex);

				vertex.position = sf::Vector2f(x * map->tw + map->tw, y * map->th + map->th);
				vertex.texCoords = sf::Vector2f(ox + map->tw, oy);
				arrays[tile.tilesetId].append(vertex);

				vertex.position = sf::Vector2f(x * map->tw, y * map->th + map->th);
				vertex.texCoords = sf::Vector2f(ox, oy);
				arrays[tile.tilesetId].append(vertex);
			}
		}
}

void TileLayer::draw(sf::RenderTarget& target, sf::RenderStates state) const {
	uint16_t i;
	for(i=0;i<arrays.size();i++){
		state.texture = &(*map->tilesets[i]);

		target.draw(arrays[i], state);
	}
}

ImageLayer::ImageLayer(Map* map, std::string file) : array(sf::Quads), Layer(Layer::Image){
	texture = map->getTexture(file);

	uint32_t x, y;
	uint32_t w, h;

	w = texture->getSize().x;
	h = texture->getSize().y;

	sf::Vertex vertex;

	for(x=0; x < map->getWidth(); x += w)
		for(y=0; y < map->getWidth(); y += h){
			vertex.position = sf::Vector2f(x, y);
			vertex.texCoords = sf::Vector2f(0, 0);
			array.append(vertex);

			vertex.position = sf::Vector2f(x + w, y);
			vertex.texCoords = sf::Vector2f(w, 0);
			array.append(vertex);

			vertex.position = sf::Vector2f(x + w, y + h);
			vertex.texCoords = sf::Vector2f(w, h);
			array.append(vertex);

			vertex.position = sf::Vector2f(x, y + h);
			vertex.texCoords = sf::Vector2f(0, h);
			array.append(vertex);
		}
}

void ImageLayer::draw(sf::RenderTarget& target, sf::RenderStates state) const {
	state.texture = &(*texture);
	target.draw(array, state);
}
