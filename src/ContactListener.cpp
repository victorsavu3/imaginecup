#include "ContactListener.h"

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

