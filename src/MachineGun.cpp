#include "MachineGun.h"
#include "GameElement.h"
#include "selene.h"
#include "Enemy.h"

MachineGun::MachineGun(b2World * world, const Vector2 &spritePos, const Vector2 &endPos, float scale, int initID) : WeaponProjectile(10, initID) {
	setResAnim(res::gm.getResAnim("mg_ammo"));
	setAnchor(Vector2(0.5f, 0.5f));
	setPosition(spritePos);
	setScale(scale);
	setPriority(1);

	//setup the body
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.bullet = true;
	bodyDef.position = ge::convert(spritePos);

	body = world->CreateBody(&bodyDef);
	b2PolygonShape box;
	b2Vec2 size = ge::convert(getSize() / 2 * scale);
	box.SetAsBox(size.x, size.y);
	body->CreateFixture(&box, 1.0f);
	body->GetFixtureList()->SetUserData((void*)"projectile");
	body->GetFixtureList()->SetSensor(true);
	body->SetUserData(this);
	body->SetGravityScale(0);
	b2Filter filter = body->GetFixtureList()->GetFilterData();
	filter.categoryBits = weaponCat;
	filter.maskBits = enemyCat | groundCat;
	body->GetFixtureList()->SetFilterData(filter);

	float distance = (100.0f / ge::SCALE);
	float deltaY = endPos.y - spritePos.y;
	float deltaX = endPos.x - spritePos.x;
	float hyp = std::pow(std::pow(deltaX, 2) + std::pow(deltaY, 2), .5);
	float mod = distance / hyp;
	velocity = b2Vec2(deltaX * mod * static_cast<double>(state["machinegunSpeed"]), deltaY * mod * static_cast<double>(state["machinegunSpeed"]));

	float bodyAngle = body->GetAngle();
	b2Vec2 toTarget = ge::convert(endPos) - body->GetPosition();
	float desiredAngle = atan2f(-toTarget.x + 30, -toTarget.y + 30);
	body->SetTransform(body->GetPosition(), desiredAngle);


}

MachineGun::~MachineGun() {

}

void MachineGun::doUpdate(const UpdateState& us) {
	dtTotal += us.dt;

	if (currentState == alive) {
		body->SetLinearVelocity(b2Vec2(velocity.x, velocity.y));
	}
	else if (currentState == dying) {
		if (getAlpha() == 15) {
			currentState = dead;
		}
		else {
			setAlpha(getAlpha() -20);
		}
		dtTotal = 0;
	}

}

int MachineGun::attack()
{
	return 0;
}

void MachineGun::startContact(b2Contact * contact)
{
	log::messageln("made contact - MachineGun");


	Actor* actorA = (Actor*)contact->GetFixtureA()->GetBody()->GetUserData();
	Actor* actorB = (Actor*)contact->GetFixtureB()->GetBody()->GetUserData();

	if (actorA && actorB) {
		if (actorA->getUserData() == "enemy" || actorA->getUserData() == "boss") {
			dynamic_cast<Enemy*>(actorA)->getHurt(damage);
		}
		else if (actorB->getUserData() == "enemy" || actorB->getUserData() == "boss") {
			dynamic_cast<Enemy*>(actorB)->getHurt(damage);
		}
	}

	destroy();
}

void MachineGun::endContact(b2Contact * contact)
{
	log::messageln("ended contact - MachineGun");
}

void MachineGun::destroy()
{
	body->SetLinearVelocity(b2Vec2(0, 0));
	currentState = dying;
	setPriority(4);

	b2Fixture* fixtureList = body->GetFixtureList();
	b2Filter filter;

	while (fixtureList) {
		filter = fixtureList->GetFilterData();
		filter.maskBits = 0;
		fixtureList->SetFilterData(filter);
		fixtureList = fixtureList->GetNext();
	}
}
