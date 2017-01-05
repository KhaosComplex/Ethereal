#include "Goblin.h"
#include "selene.h"

Goblin::Goblin() : Enemy(150, 10, 5), dtTotal(0), footState(floor), direction(right), changedDirection(false), firstPass(0)
{
}

Goblin::~Goblin()
{
}

void Goblin::_init(b2World * world, const Vector2 &pos, float scale)
{
	//initialize Goblin's sprite
	setResAnim(res::gm.getResAnim("goblinWalkRight"));
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
	box.SetAsBox(size.x, size.y, b2Vec2(0, .060), 0);
	body->CreateFixture(&box, 10.0f);
	//set collision categories
	b2Filter filter = body->GetFixtureList()->GetFilterData();
	filter.categoryBits = enemyCat;
	filter.maskBits = playerCat | groundCat | weaponCat;
	body->GetFixtureList()->SetFilterData(filter);
	//add footRight sensor fixture
	box.SetAsBox(0.05, 0.07, b2Vec2(.28, .35), 0);
	body->CreateFixture(&box, 10.0f);
	body->GetFixtureList()->SetSensor(true);
	body->GetFixtureList()->SetUserData((void*)"footRight");
	filter = body->GetFixtureList()->GetFilterData();
	filter.categoryBits = enemyCat;
	filter.maskBits = groundCat;
	body->GetFixtureList()->SetFilterData(filter);
	//add footLeft sensor fixture
	box.SetAsBox(0.05, 0.07, b2Vec2(-.28, .35), 0);
	body->CreateFixture(&box, 10.0f);
	body->GetFixtureList()->SetSensor(true);
	body->GetFixtureList()->SetUserData((void*)"footLeft");
	filter = body->GetFixtureList()->GetFilterData();
	filter.categoryBits = enemyCat;
	filter.maskBits = groundCat;
	body->GetFixtureList()->SetFilterData(filter);

	body->SetUserData(this);
	body->SetFixedRotation(true);
	vel = body->GetLinearVelocity();
}

void Goblin::doUpdate(const UpdateState & us)
{
	if (attached) {
		dtTotal += us.dt;

		float speed = static_cast<double>(state["goblinSpeed"]) / ge::SCALE * us.dt;

		if (HP <= 0 && currentState == alive) {
			die();
		}

		if (getPosition().y > 720) {
			currentState = dead;
		}

		if (currentState == alive) {
			if (direction == right) {
				if (getResAnim() != res::gm.getResAnim("goblinWalkRight")) {
					setResAnim(res::gm.getResAnim("goblinWalkRight"), getAnimFrame().getColumn());
				}
				vel.x = speed;
				body->SetLinearVelocity(vel);

			}
			else if (direction == left) {
				if (getResAnim() != res::gm.getResAnim("goblinWalkLeft")) {
					setResAnim(res::gm.getResAnim("goblinWalkLeft"), getAnimFrame().getColumn());
				}

				vel.x = -speed;
				body->SetLinearVelocity(vel);

			}
		}
		else if (currentState == dying) {

		}
		animateState();
		changedDirection = false;
	}
}
void Goblin::animateState()
{
	if (dtTotal / static_cast<int>(state["goblinAnimRate"]) >= 1)
	{
		if (currentState == alive)
		{
			if (getAnimFrame().getColumn() == getResAnim()->getColumns() - 1)
			{
				setAnimFrame(getResAnim(), 0);
			}
			else {
				setAnimFrame(getResAnim(), getAnimFrame().getColumn() + 1);
			}
		}
		else if (currentState == dying)
		{
			if (getAnimFrame().getColumn() == getResAnim()->getColumns() - 1)
			{
				currentState = dead;
			}
			else
			{
				setAnimFrame(getResAnim(), getAnimFrame().getColumn() + 1);
			}
		}
		dtTotal = 0;
	}
}
void Goblin::die()
{
	if (getResAnim() == res::gm.getResAnim("goblinWalkRight"))
	{
		setResAnim(res::gm.getResAnim("goblinDieRight"), 0);
	}
	else
	{
		setResAnim(res::gm.getResAnim("goblinDieLeft"), 0);
	}

	body->SetLinearVelocity(b2Vec2(0, 0));
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
void Goblin::reverseDirection()
{
	if (!changedDirection) {
		if (direction == right) {
			direction = left;
		}
		else {
			direction = right;
		}
		changedDirection = true;
	}
}

void Goblin::startContact(b2Contact* contact)
{
	log::messageln("Goblin started contact");

	void* fixtureUserDataA = contact->GetFixtureA()->GetUserData();
	void* fixtureUserDataB = contact->GetFixtureB()->GetUserData();

	if (firstPass != 2) {
		if (fixtureUserDataA == "footRight" || fixtureUserDataB == "footRight" || fixtureUserDataA == "footLeft" || fixtureUserDataB == "footLeft") {
			firstPass++;
		}
	}
	else {
		if (fixtureUserDataA == "footRight" || fixtureUserDataB == "footRight" || fixtureUserDataA == "footLeft" || fixtureUserDataB == "footLeft") {
			if (footState == floor) {
				reverseDirection();
				footState = wall;
			}
			else {
				footState = floor;
			}
		}
	}


}

void Goblin::endContact(b2Contact* contact)
{
	log::messageln("Goblin ended contact");

	void* fixtureUserDataA = contact->GetFixtureA()->GetUserData();
	void* fixtureUserDataB = contact->GetFixtureB()->GetUserData();

	if (fixtureUserDataA == "footRight" || fixtureUserDataB == "footRight" || fixtureUserDataA == "footLeft" || fixtureUserDataB == "footLeft") {
		if (footState == floor) {
			reverseDirection();
			footState = nothing;
		}
		else {
			footState = floor;
		}
	}
}

