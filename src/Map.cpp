#include "Map.h"

#include <TmxParser/Tmx.h>

#include "error.h"

Map::Map(std::string file) : world(b2Vec2(0, 50)){
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
	}

	for(i=0; i<map.GetNumImageLayers(); i++){
		const Tmx::ImageLayer *layer = map.GetImageLayer(i);

		layers[layer->GetZOrder()] = new ImageLayer(this, layer->GetImage()->GetSource());
	}

	for(i=0; i<map.GetNumObjectGroups(); i++){
		const Tmx::ObjectGroup *layer = map.GetObjectGroup(i);

		for(j = 0; j < layer->GetNumObjects(); j++){
			const Tmx::Object* object = layer->GetObject(j);

			FAIL_ON(layer->GetZOrder() == 0 || layers[layer->GetZOrder() - 1]->type != Layer::Tile, "Object layer %d must follow tile layer", layer->GetZOrder());

			if(object->GetType() == "Collision"){
				if(object->GetPolygon() != NULL){
					b2BodyDef groundBodyDef;
					groundBodyDef.position = convert(sf::Vector2f(object->GetX(), object->GetY()), this);

					b2Body* groundBody = world.CreateBody(&groundBodyDef);

					b2PolygonShape groundBox;
					b2Vec2 points[object->GetPolygon()->GetNumPoints()];
					for(k=0;k<object->GetPolygon()->GetNumPoints();k++){
						points[k] = convert(object->GetPolygon()->GetPoint(k), this);
					}
					groundBox.Set(points, object->GetPolygon()->GetNumPoints());

					groundBody->CreateFixture(&groundBox, 0);
				} else if(object->GetPolyline() != NULL){
					b2BodyDef groundBodyDef;
					groundBodyDef.position = convert(sf::Vector2f(object->GetX(), object->GetY()), this);

					b2Body* groundBody = world.CreateBody(&groundBodyDef);

					b2ChainShape chain;
					b2Vec2 points[object->GetPolyline()->GetNumPoints()];
					for(k=0;k<object->GetPolyline()->GetNumPoints();k++){
						points[k] = convert(object->GetPolyline()->GetPoint(k), this);
					}
					chain.CreateChain(points, object->GetPolyline()->GetNumPoints());

					groundBody->CreateFixture(&chain, 0);
				} else {
					FAIL("Objects of type 'Collision' must be a polygon or a polyline");
				}
			} else if(object->GetType() == "Player"){
				FAIL_ON(player, "There can only be one player instance")
				player.reset(new Player(this, convert(sf::Vector2f(object->GetX(), object->GetY()), this), layer->GetZOrder() - 1));
			}

			layers[layer->GetZOrder()] = NULL;
		}
	}

	for(i=0;i<n;i++){
		if(layers[i] == NULL){
			layers.erase(layers.begin() + i);
		}
	}

	n=layers.size();

	for(i=0;i<n;i++){
		layers[i]->scale = (float)(i + 1) / n;
		layers[i]->color = (float)(i + 1) / n;
	}

	world.SetContactListener(&listener);

	debug = NULL;
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

		desaturate->setParameter("saturation", layers[i]->scale);
		desaturate->setParameter("alpha", layers[i]->alpha);

		state.transform.translate( - position * layers[i]->scale);
		state.shader = &*desaturate;

		target.draw(*layers[i], state);

		if(i == player->getLayer())
			target.draw(*player, state);
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
	world.Step(frame, 8, 3);
	player->step(frame, time);

	if(debug){
		debug->clear();
		world.SetDebugDraw(debug);
		world.DrawDebugData();
		debug->display();
	}
}

void ContactListener::BeginContact(b2Contact* contact) {
	if(contact->GetFixtureA()->GetUserData() != NULL)
		((b2ContactListener*)contact->GetFixtureA()->GetUserData())->BeginContact(contact);
	if(contact->GetFixtureB()->GetUserData() != NULL)
		((b2ContactListener*)contact->GetFixtureB()->GetUserData())->BeginContact(contact);
}

void ContactListener::EndContact(b2Contact* contact) {
	if(contact->GetFixtureA()->GetUserData() != NULL)
		((b2ContactListener*)contact->GetFixtureA()->GetUserData())->EndContact(contact);
	if(contact->GetFixtureB()->GetUserData() != NULL)
		((b2ContactListener*)contact->GetFixtureB()->GetUserData())->EndContact(contact);
}

float Map::getScale(uint16_t layer) {
	return layers[layer]->scale;
}
