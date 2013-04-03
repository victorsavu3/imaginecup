#include "Layer.h"

#include "conversions.h"
#include "error.h"

#include "PickUp.h"

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

ObjectLayer::ObjectLayer(Map* map, const Tmx::ObjectGroup* layer, const Tmx::Map* tmx) : Layer(Layer::Object), world(b2Vec2(0, 50)), map(map), environment(Collider::Environment) {
	debug.reset(new DebugDraw());

	world.SetContactListener(&listener);
	world.SetDebugDraw(&*debug);

	uint16_t j,k;

	for(j = 0; j < layer->GetNumObjects(); j++){
		const Tmx::Object* object = layer->GetObject(j);

		if(object->GetType() == "Collision"){
			if(object->GetEllipse() != NULL) {
				FAIL("Objects of type 'Collision' must be a polygon, polyline or box");
			} else if(object->GetPolygon() != NULL){
				b2BodyDef groundBodyDef;
				groundBodyDef.position = convert(sf::Vector2f(object->GetX(), object->GetY()), map);

				b2Body* body = world.CreateBody(&groundBodyDef);
				body->SetUserData(&environment);

				b2PolygonShape groundBox;
				b2Vec2 points[object->GetPolygon()->GetNumPoints()];
				for(k=0;k<object->GetPolygon()->GetNumPoints();k++){
					points[k] = convert(object->GetPolygon()->GetPoint(k), map);
				}
				groundBox.Set(points, object->GetPolygon()->GetNumPoints());

				body->CreateFixture(&groundBox, 0);
			} else if(object->GetPolyline() != NULL){
				b2BodyDef groundBodyDef;
				groundBodyDef.position = convert(sf::Vector2f(object->GetX(), object->GetY()), map);

				b2Body* body = world.CreateBody(&groundBodyDef);
				body->SetUserData(&environment);

				b2ChainShape chain;
				b2Vec2 points[object->GetPolyline()->GetNumPoints()];
				for(k=0;k<object->GetPolyline()->GetNumPoints();k++){
					FAIL_ON(object->GetPolyline()->GetPoint(k).x < 0, "Coordinates must be positive");
					FAIL_ON(object->GetPolyline()->GetPoint(k).y < 0, "Coordinates must be positive");
					points[k] = convert(object->GetPolyline()->GetPoint(k), map);
				}
				chain.CreateChain(points, object->GetPolyline()->GetNumPoints());

				body->CreateFixture(&chain, 0);
			} else {
				b2BodyDef groundBodyDef;
				groundBodyDef.position = convert(sf::Vector2f(object->GetX(), object->GetY()), map);

				b2Body* body = world.CreateBody(&groundBodyDef);
				body->SetUserData(&environment);

				b2PolygonShape dynamicBox;
				dynamicBox.SetAsBox(convert(sf::Vector2f(object->GetWidth(), object->GetHeight()), map).x / 2,
						convert(sf::Vector2f(object->GetWidth(), object->GetHeight()), map). y / 2,
						convert(sf::Vector2f(object->GetWidth() / 2, object->GetHeight() / 2), map), 0);

				body->CreateFixture(&dynamicBox, 0);
			}
		} else if(object->GetType() == "Portal"){
			FAIL_ON(!object->GetProperties().HasProperty("Direction"), "A Portal must have a Direction");

			if(object->GetProperties().GetLiteralProperty("Direction") == "Up")
				portals.push_back(Portal(Portal::Up));
			else if(object->GetProperties().GetLiteralProperty("Direction") == "Down")
				portals.push_back(Portal(Portal::Down));
			else
				FAIL("Direction must be Up or Down");


			b2BodyDef groundBodyDef;
			groundBodyDef.position = convert(sf::Vector2f(object->GetX(), object->GetY()), map);

			b2Body* body = world.CreateBody(&groundBodyDef);
			body->SetUserData(&portals[portals.size() - 1]);

			b2PolygonShape dynamicBox;
			dynamicBox.SetAsBox(convert(sf::Vector2f(object->GetWidth(), object->GetHeight()), map).x / 2,
					convert(sf::Vector2f(object->GetWidth(), object->GetHeight()), map). y / 2,
					convert(sf::Vector2f(object->GetWidth() / 2, object->GetHeight() / 2), map), 0);

			b2FixtureDef fixtureDef;
			fixtureDef.shape = &dynamicBox;
			fixtureDef.density = 0;
			fixtureDef.friction = 0;
			fixtureDef.isSensor = true;

			body->CreateFixture(&fixtureDef);
		} else if(object->GetType() == "Player"){
			FAIL_ON(map->player, "There can only be one player instance")
			map->player.reset(new Player(this, convert(sf::Vector2f(object->GetX(), object->GetY()), map), layer->GetZOrder()));
			objects.insert(map->player);
		} else if(object->GetType() == "Coin") {
			objects.insert(shared_ptr<Entity>(new PickUp(this, PickUp::Coin, "coin", sf::Vector2f(object->GetX(), object->GetY()))));
		} else  {
			FAIL("Object type unrecognized");
		}
	}
}

void ObjectLayer::draw(sf::RenderTarget& target, sf::RenderStates state) const {
	std::set<shared_ptr<Entity> >::iterator it;
	for(it=objects.begin(); it !=objects.end(); it++){
		target.draw(**it, state);
	}
}

ObjectLayer::~ObjectLayer() {
	objects.clear();
}

void ObjectLayer::step(float frame, float time) {
	world.Step(frame, 8, 3);
	std::set<shared_ptr<Entity> >::iterator it;
	for(it=objects.begin(); it !=objects.end(); it++){
		(*it)->step(frame, time);
	}

	/*debug->clear();
	world.DrawDebugData();
	debug->display();*/
}

DecorationLayer::DecorationLayer(Map* map, const Tmx::ObjectGroup* layer,
		const Tmx::Map* tmx) : Layer(Layer::Decoration), map(map), array(sf::Quads){

	uint16_t j;
	for(j = 0; j < layer->GetNumObjects(); j++){
		const Tmx::Object* object = layer->GetObject(j);

		FAIL_ON(object->GetGid() == 0, "Decoration layer can only contain images");

		tilesetID = tmx->FindTilesetIndex(object->GetGid());

		Tmx::MapTile tile(object->GetGid(), tmx->GetTileset(tilesetID)->GetFirstGid(), tilesetID);

		float x = object->GetX();
		float y = object->GetY() - map->th;

		uint16_t tpw = map->tilesets[tile.tilesetId]->getSize().x / map->tw;

		uint16_t ox, oy;

		ox = (tile.id % tpw) * map->tw;
		oy = (tile.id / tpw) * map->th;

		sf::Vertex vertex;

		vertex.position = sf::Vector2f(x, y);
		vertex.texCoords = sf::Vector2f(ox, oy);
		array.append(vertex);

		vertex.position = sf::Vector2f(x + map->tw, y);
		vertex.texCoords = sf::Vector2f(ox + map->tw, oy);
		array.append(vertex);

		vertex.position = sf::Vector2f(x + map->tw, y + map->th);
		vertex.texCoords = sf::Vector2f(ox + map->tw, oy + map->th);
		array.append(vertex);

		vertex.position = sf::Vector2f(x, y + map->th);
		vertex.texCoords = sf::Vector2f(ox, oy + map->th);
		array.append(vertex);
	}
}

void DecorationLayer::draw(sf::RenderTarget& target,
		sf::RenderStates state) const {

	state.texture = &(*map->tilesets[tilesetID]);

	target.draw(array, state);
}

void DecorationLayer::step(float frame, float time) {}
