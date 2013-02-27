#ifndef TILEMAP_H_
#define TILEMAP_H_

#include <SFML/Graphics.hpp>

#include <vector>
#include <string>

#include <stdint.h>

class Tilemap{
public:
	Tilemap(std::string file);
	virtual ~Tilemap();

	void drawLayer(sf::RenderTarget& target, sf::RenderStates states, uint16_t layer) const;

	uint16_t getLayerCount(){
		return tilesets.size();
	}
private:
	std::vector<sf::Texture> tilesets;
	std::vector<std::vector<sf::VertexArray> > layers;
};

#endif /* TILEMAP_H_ */
