#include "TurretBossProjectile.h"
#include "GameElement.h"
#include "selene.h"
#include "Enemy.h"

TurretBossProjectile::TurretBossProjectile(b2World * world, const Vector2 &spritePos, const Vector2 &endPos, float scale, int initID) : WeaponProjectile(20, initID) {
	setResAnim(res::gm.getResAnim("turret_ammo"));
	setAnchor(Vector2(0.5f, 0.5f));
	setPosition(spritePos);
	setScale(scale);
	setPriority(4);

	setUserData((void*)"projectile");

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
	filter.categoryBits = enemyCat;
	filter.maskBits = playerCat;
	body->GetFixtureList()->SetFilterData(filter);

	float distance = (150.0f / ge::SCALE);
	float deltaY = endPos.y - spritePos.y;
	float deltaX = endPos.x - spritePos.x;
	float hyp = std::pow(std::pow(deltaX, 2) + std::pow(deltaY, 2), .5);
	float mod = distance / hyp;
	velocity = b2Vec2(deltaX * mod * static_cast<double>(state["turretBossProjectileSpeed"]), deltaY * mod * static_cast<double>(state["turretBossProjectileSpeed"]));

	float bodyAngle = body->GetAngle();
	b2Vec2 toTarget = ge::convert(endPos) - body->GetPosition();
	float desiredAngle = atan2f(toTarget.x, toTarget.y);
	body->SetTransform(body->GetPosition(), -desiredAngle);
}

TurretBossProjectile::~TurretBossProjectile() {

}

void TurretBossProjectile::doUpdate(const UpdateState& us) {
	dtTotal += us.dt;

	if (currentState == alive) {
		body->SetLinearVelocity(b2Vec2(velocity.x / 3, velocity.y / 3));
	}
	else if (currentState == dying) {
		
	}
}

int TurretBossProjectile::attack()
{
	return damage;
}

void TurretBossProjectile::startContact(b2Contact * contact)
{
	log::messageln("made contact - TurretBossProjectile");

	destroy();
}

void TurretBossProjectile::endContact(b2Contact * contact)
{
	log::messageln("ended contact - TurretBossProjectile");
}

void TurretBossProjectile::destroy()
{
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
