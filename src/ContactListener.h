#ifndef CONTACTLISTENTER_H_
#define CONTACTLISTENTER_H_

#include <Box2D/Box2D.h>

class ContactListener : public b2ContactListener {
public:
	void BeginContact(b2Contact* contact);
	void EndContact(b2Contact* contact);
};

#endif /* CONTACTLISTENTER_H_ */
