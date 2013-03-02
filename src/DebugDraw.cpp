#include "DebugDraw.h"

#include <stdint.h>

DebugDraw::DebugDraw() {
	window.create(sf::VideoMode(800, 600), "Imaginecup");
	SetFlags(e_shapeBit);
}

DebugDraw::~DebugDraw() {
}

void DebugDraw::DrawPolygon(const b2Vec2* vertices, const int32 vertexCount,
		const b2Color& color) {

	sf::ConvexShape poly;

	poly.setPointCount(vertexCount);
	poly.setOutlineColor(sf::Color(color.r, color.g, color.b, 255));
	poly.setFillColor(sf::Color(color.r, color.g, color.b, 0));

	uint16_t i;
	for(i=0;i<vertexCount;i++)
		poly.setPoint(i, sf::Vector2f(vertices[i].x, vertices[i].y));

	sf::RenderStates states;
	states.transform.scale(32, 32);

	window.draw(poly, states);
}

void DebugDraw::DrawSolidPolygon(const b2Vec2* vertices, const int32 vertexCount,
		const b2Color& color) {

	sf::ConvexShape poly;

	poly.setPointCount(vertexCount);
	poly.setOutlineColor(sf::Color(color.r, color.g, color.b, 255));
	poly.setFillColor(sf::Color(color.r, color.g, color.b, 200));

	uint16_t i;
	for(i=0;i<vertexCount;i++)
		poly.setPoint(i, sf::Vector2f(vertices[i].x, vertices[i].y));

	sf::RenderStates states;
	states.transform.scale(32, 32);

	window.draw(poly, states);
}

void DebugDraw::DrawCircle(const b2Vec2& center, const float32 radius, const b2Color& color) {
	sf::CircleShape circle;

	circle.setRadius(radius);
	circle.setPosition(center.x, center.y);

	circle.setOutlineColor(sf::Color(color.r, color.g, color.b, 255));
	circle.setFillColor(sf::Color(color.r, color.g, color.b, 0));

	sf::RenderStates states;
	states.transform.scale(32, 32);

	window.draw(circle, states);
}

void DebugDraw::DrawSolidCircle(const b2Vec2& center, const float32 radius, const b2Vec2& axis,
		const b2Color& color) {
	sf::CircleShape circle;

	circle.setRadius(radius);
	circle.setPosition(center.x, center.y);
	circle.setOrigin(radius, radius);

	circle.setOutlineColor(sf::Color(color.r, color.g, color.b, 255));
	circle.setFillColor(sf::Color(color.r, color.g, color.b, 200));

	sf::RenderStates states;
	states.transform.scale(32, 32);

	window.draw(circle, states);
}

void DebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) {
	sf::Vertex line[2] = {sf::Vector2f(p1.x, p1.y), sf::Vector2f(p2.x, p2.y)};
	line[0].color = sf::Color(color.r, color.g, color.b, 255);
	line[1].color = sf::Color(color.r, color.g, color.b, 255);

	sf::RenderStates states;
	states.transform.scale(32, 32);

	window.draw(line, 2, sf::Lines, states);
}

void DebugDraw::DrawTransform(const b2Transform&) {
}
