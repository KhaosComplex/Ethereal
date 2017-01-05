#include "ContactListener.h"
#include "ContactObject.h"
#include "Player.h"
#include "Goblin.h"

ContactListener::ContactListener() {}

ContactListener::~ContactListener() {}

void ContactListener::BeginContact(b2Contact* contact)
{
	int numFootContacts  = 0;
	void* fixtureUserData = contact->GetFixtureA()->GetUserData();
	if (fixtureUserData == "foot")
		numFootContacts++;
	//check if fixture B was the foot sensor
	fixtureUserData = contact->GetFixtureB()->GetUserData();
	if (fixtureUserData == "foot")
		numFootContacts++;

	ContactObject* A = static_cast<ContactObject*>
		(contact->GetFixtureA()->GetBody()->GetUserData());
	ContactObject* B = static_cast<ContactObject*>
		(contact->GetFixtureB()->GetBody()->GetUserData());

	if (dynamic_cast<Player*>(A) != nullptr) {
		A->startContact(contact);
	}
	else if (dynamic_cast<Player*>(B) != nullptr) {
		B->startContact(contact);
	}
	else {
		if (A != nullptr) {
			A->startContact(contact);
		}
		if (B != nullptr) {
			B->startContact(contact);
		}
	}
}

void ContactListener::EndContact(b2Contact* contact)
{
	ContactObject* A = static_cast<ContactObject*>
		(contact->GetFixtureA()->GetBody()->GetUserData());
	ContactObject* B = static_cast<ContactObject*>
		(contact->GetFixtureB()->GetBody()->GetUserData());

	if (dynamic_cast<Player*>(A) != nullptr) {
		A->endContact(contact);
	}
	else if (B != nullptr) {
		B->endContact(contact);
	}
	else if (A != nullptr) {
		A->endContact(contact);
	}
}