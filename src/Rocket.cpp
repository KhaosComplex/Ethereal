#include "Rocket.h"
#include "GameElement.h"
#include "selene.h"
#include "Enemy.h"

Rocket::Rocket(b2World * world, const Vector2 &spritePos, const Vector2 &endPos, float scale, int initID) : WeaponProjectile(75, initID) {
	setResAnim(res::gm.getResAnim("rocket_ammo"));
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
	velocity = b2Vec2(deltaX * mod * static_cast<double>(state["rocketSpeed"]), deltaY * mod * static_cast<double>(state["rocketSpeed"]));

	float bodyAngle = body->GetAngle();
	b2Vec2 toTarget = ge::convert(endPos) - body->GetPosition();
	float desiredAngle = atan2f(-toTarget.x, toTarget.y);
	body->SetTransform(body->GetPosition(), desiredAngle);

	
}

Rocket::~Rocket() {
	
}

void Rocket::doUpdate(const UpdateState& us) {
	dtTotal += us.dt;

	if (currentState == alive) {
		body->SetLinearVelocity(b2Vec2(velocity.x, velocity.y));
	}
	else if (currentState == dying) {
		if (dtTotal / static_cast<int>(state["playerAnimRate"]) >= 1) {
			if (getAnimFrame().getColumn() == getResAnim()->getColumns() - 1) {
				currentState = dead;
			}
			else {
				setAnimFrame(getResAnim(), getAnimFrame().getColumn() + 1);
			}
			dtTotal = 0;
		}
	}
}

int Rocket::attack()
{
	return 0;
}

void Rocket::startContact(b2Contact * contact)
{
	log::messageln("made contact - ROCKET");
	

	Actor* actorA = (Actor*)contact->GetFixtureA()->GetBody()->GetUserData();
	Actor* actorB = (Actor*)contact->GetFixtureB()->GetBody()->GetUserData();

	if (actorA && actorB) {
		if (actorA->getUserData() == "enemy") {
			dynamic_cast<Enemy*>(actorA)->getHurt(damage);
		}
		else if (actorB->getUserData() == "enemy") {
			dynamic_cast<Enemy*>(actorB)->getHurt(damage);
		}
	}

	destroy();
}

void Rocket::endContact(b2Contact * contact)
{
	log::messageln("ended contact - ROCKET");
}

void Rocket::destroy()
{
	setResAnim(res::gm.getResAnim("rocket_explosion"), 0);
	body->SetLinearVelocity(b2Vec2(0,0));
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
