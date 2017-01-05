#include "Totem.h"
#include "selene.h"

Totem::Totem() : Enemy(150, 10, 5), dtTotal(0), fire(false)
{
}

Totem::~Totem()
{
}

void Totem::_init(b2World * world, const Vector2 &pos, float scale)
{
	//initialize Totem's sprite
	setResAnim(res::gm.getResAnim("totem"));
	setAnchor(Vector2(0.5f, 0.5f));
	originalPosition = pos;

	setUserData((void*)"enemy");


	//setup the body
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position = ge::convert(pos);

	body = world->CreateBody(&bodyDef);

	setScale(scale);

	b2PolygonShape box;
	b2Vec2 size = ge::convert(getSize() / 2 * scale);
	box.SetAsBox(size.x, size.y);
	body->CreateFixture(&box, 10.0f);
	//set collision categories
	b2Filter filter = body->GetFixtureList()->GetFilterData();
	filter.categoryBits = enemyCat;
	filter.maskBits = groundCat | weaponCat;
	body->GetFixtureList()->SetFilterData(filter);

	body->SetUserData(this);
	body->SetFixedRotation(true);
	vel = body->GetLinearVelocity();
}

void Totem::doUpdate(const UpdateState & us)
{
	dtTotal += us.dt;

	if (attached && currentState == alive) {
		if (dtTotal > 1200) {
			dtTotal = 1200;
		}

		if (HP <= 0 && currentState == alive) {
			die();
		}

		if (currentState == alive) {
			if (dtTotal >= 1200) {
				fire = true;
			}
		}
	}
	else if (currentState == dying) {
		if (getAlpha() == 15) {
			currentState = dead;
		}
		else {
			setAlpha(getAlpha() - 20);
		}
		dtTotal = 0;
	}
}
bool Totem::canFire()
{
	return fire;
}
void Totem::fireWeapon()
{
	fire = false;
	dtTotal = 0;
}

void Totem::animateState()
{

}
void Totem::die()
{
	currentState = dying;

	b2Fixture* fixtureList = body->GetFixtureList();
	b2Filter filter;

	while (fixtureList) {
		filter = fixtureList->GetFilterData();
		filter.maskBits = groundCat;
		fixtureList->SetFilterData(filter);
		fixtureList = fixtureList->GetNext();
	}

}

void Totem::startContact(b2Contact* contact)
{
	log::messageln("Totem started contact");

	void* fixtureUserDataA = contact->GetFixtureA()->GetUserData();
	void* fixtureUserDataB = contact->GetFixtureB()->GetUserData();

}

void Totem::endContact(b2Contact* contact)
{
	log::messageln("Totem ended contact");

	void* fixtureUserDataA = contact->GetFixtureA()->GetUserData();
	void* fixtureUserDataB = contact->GetFixtureB()->GetUserData();

}

