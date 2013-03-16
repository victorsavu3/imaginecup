#include "Layer.h"

#include "conversions.h"
#include "error.h"

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

ObjectLayer::ObjectLayer(Map* map, const Tmx::ObjectGroup* layer, const Tmx::Map* tmx) : Layer(Layer::Object), world(b2Vec2(0, 50)), map(map){
	world.SetContactListener(&listener);

	uint16_t j,k;

	for(j = 0; j < layer->GetNumObjects(); j++){
		const Tmx::Object* object = layer->GetObject(j);

		if(object->GetGid() != 0){
			int tilesetID = tmx->FindTilesetIndex(object->GetGid());

			Tmx::MapTile tile(object->GetGid(), tmx->GetTileset(tilesetID)->GetFirstGid(), tilesetID);

			shared_ptr<Entity> entity(new Entity());

			entity->setTexture(*(map->tilesets[tilesetID]));
			entity->setPosition(sf::Vector2f(object->GetX(), object->GetY()));
			entity->setOrigin(sf::Vector2f(0, map->th));

			uint16_t tpw = map->tilesets[tile.tilesetId]->getSize().x / map->tw;

			uint16_t ox, oy;

			ox = (tile.id % tpw) * map->tw;
			oy = (tile.id / tpw) * map->th;

			entity->setTextureRect(sf::IntRect(ox, oy, map->tw, map->th));

			objects.insert(entity);
		} else {
			if(object->GetType() == "Collision"){
				if(object->GetPolygon() != NULL){
					b2BodyDef groundBodyDef;
					groundBodyDef.position = convert(sf::Vector2f(object->GetX(), object->GetY()), map);

					b2Body* groundBody = world.CreateBody(&groundBodyDef);

					b2PolygonShape groundBox;
					b2Vec2 points[object->GetPolygon()->GetNumPoints()];
					for(k=0;k<object->GetPolygon()->GetNumPoints();k++){
						points[k] = convert(object->GetPolygon()->GetPoint(k), map);
					}
					groundBox.Set(points, object->GetPolygon()->GetNumPoints());

					groundBody->CreateFixture(&groundBox, 0);
				} else if(object->GetPolyline() != NULL){
					b2BodyDef groundBodyDef;
					groundBodyDef.position = convert(sf::Vector2f(object->GetX(), object->GetY()), map);

					b2Body* groundBody = world.CreateBody(&groundBodyDef);

					b2ChainShape chain;
					b2Vec2 points[object->GetPolyline()->GetNumPoints()];
					for(k=0;k<object->GetPolyline()->GetNumPoints();k++){
						points[k] = convert(object->GetPolyline()->GetPoint(k), map);
					}
					chain.CreateChain(points, object->GetPolyline()->GetNumPoints());

					groundBody->CreateFixture(&chain, 0);
				} else {
					FAIL("Objects of type 'Collision' must be a polygon or a polyline");
				}
			} else if(object->GetType() == "Player"){
				FAIL_ON(map->player, "There can only be one player instance")
				map->player.reset(new Player(this, convert(sf::Vector2f(object->GetX(), object->GetY()), map), layer->GetZOrder()));
				objects.insert(map->player);
			}
		}
	}
}

void ObjectLayer::draw(sf::RenderTarget& target, sf::RenderStates state) const {
	std::set<shared_ptr<Entity> >::iterator it;
	for(it=objects.begin(); it !=objects.end(); it++){
		target.draw(**it, state);
	}
}

void ObjectLayer::step(float frame, float time) {
	world.Step(frame, 8, 3);
	std::set<shared_ptr<Entity> >::iterator it;
	for(it=objects.begin(); it !=objects.end(); it++){
		(*it)->step(frame, time);
	}
}
