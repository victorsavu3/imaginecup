#ifndef DEBUGDRAW_H_
#define DEBUGDRAW_H_

#include <Box2D/Box2D.h>

#include <SFML/Graphics.hpp>

class DebugDraw : public b2Draw{
public:
	DebugDraw();
	virtual ~DebugDraw();

	virtual void DrawPolygon(const b2Vec2*, int32, const b2Color&);
	virtual void DrawSolidPolygon(const b2Vec2*, int32, const b2Color&);
	virtual void DrawCircle(const b2Vec2&, float32, const b2Color&);
	virtual void DrawSolidCircle(const b2Vec2&, float32, const b2Vec2&, const b2Color&);
	virtual void DrawSegment(const b2Vec2&, const b2Vec2&, const b2Color&);
	virtual void DrawTransform(const b2Transform&);

	void clear(){
		window.clear(sf::Color(100, 100, 100, 255));
	}

	void display(){
		window.display();
	}

private:
	sf::RenderWindow window;
};

#endif /* DEBUGDRAW_H_ */
