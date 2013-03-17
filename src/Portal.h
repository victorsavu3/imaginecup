#ifndef PORTAL_H_
#define PORTAL_H_

#include "Collider.h"

class Portal : public Collider{
public:
	typedef enum {
		Up,
		Down
	} Direction;

	Portal(Direction direction) : Collider(Collider::Portal), direction(direction) {}

	Direction direction;
};

#endif /* PORTAL_H_ */
