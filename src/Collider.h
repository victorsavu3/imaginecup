#ifndef COLLIDER_H_
#define COLLIDER_H_

class Collider {
public:
	typedef enum {
		Player,
		Portal,
		Environment
	} Type;

	Collider(Type type) : type(type) {}

	Type type;
};

#endif /* COLLIDER_H_ */
