#ifndef COLLIDER_H_
#define COLLIDER_H_

class Collider {
public:
	typedef enum {
		Player,
		Portal,
		Environment,
		PickUp
	} Type;

	Collider(Type type) : type(type) {}
	virtual ~Collider() {}

	Type type;
};

#endif /* COLLIDER_H_ */
