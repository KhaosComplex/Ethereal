#include "TurretBoss.h"
#include "selene.h"

TurretBoss::TurretBoss() : Enemy(5000, 10, 5), dtTotal(0), started(false), fire(false)
{
}

TurretBoss::~TurretBoss()
{
}

void TurretBoss::_init(b2World * world, const Vector2 &pos, float scale)
{
	//initialize TurretBoss's sprite
	setResAnim(res::gm.getResAnim("turret"));
	setAnchor(Vector2(0.5f, 0.5f));
	originalPosition = pos;

	setUserData((void*)"boss");


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
	filter.maskBits =  groundCat | weaponCat;
	body->GetFixtureList()->SetFilterData(filter);

	body->SetUserData(this);
	body->SetFixedRotation(true);
	vel = body->GetLinearVelocity();
}

void TurretBoss::doUpdate(const UpdateState & us)
{
	if (attached && currentState == alive) {
		if (dtTotal < 2400){
			dtTotal += us.dt;
		}

		if (!started) {
			HP = 5000;
		}

		if (HP <= 0 && currentState == alive) {
			die();
		}

		if (started && currentState == alive) {
			if (dtTotal >= 2400) {
				fire = true;
			}
		}
	}
}
bool TurretBoss::canFire()
{
	return fire;
}
void TurretBoss::fireWeapon()
{
	fire = false;
	dtTotal = 0;
}
void TurretBoss::start()
{
	started = true;
}
void TurretBoss::animateState()
{
	
}
void TurretBoss::die()
{
	currentState = dying;

	b2Fixture* fixtureList = body->GetFixtureList();
	b2Filter filter;

	while (fixtureList) {
		filter = fixtureList->GetFilterData();
		filter.maskBits = 0;
		fixtureList->SetFilterData(filter);
		fixtureList = fixtureList->GetNext();
	}

}

void TurretBoss::startContact(b2Contact* contact)
{
	log::messageln("TurretBoss started contact");

	void* fixtureUserDataA = contact->GetFixtureA()->GetUserData();
	void* fixtureUserDataB = contact->GetFixtureB()->GetUserData();

}

void TurretBoss::endContact(b2Contact* contact)
{
	log::messageln("TurretBoss ended contact");

	void* fixtureUserDataA = contact->GetFixtureA()->GetUserData();
	void* fixtureUserDataB = contact->GetFixtureB()->GetUserData();

}

