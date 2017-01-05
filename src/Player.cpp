#include "Player.h"
#include "Enemy.h"

Player::Player() : Character(0), doubleJump(2), direction(right), HP(100), stunned(false), stun_dt(0), weapon_dt(0), weapon(rocket), fire(true)
{

}

void Player::_init(b2World * world, const Vector2 &pos, float scale)
{
	//initialize player's sprite
	setResAnim(res::gm.getResAnim("playerWalkRight"));
	setAnchor(Vector2(0.5f, 0.5f));

	setUserData((void*)"player");

	//setup the body
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position = ge::convert(pos);

	body = world->CreateBody(&bodyDef);

	setScale(scale);

	b2CircleShape circle;
	b2Vec2 size = ge::convert(getSize() / 2 * scale);
	size.x = size.x - 10 / ge::SCALE;
	size.y = size.y - 6 / ge::SCALE;
	circle.m_radius = size.x / 2;
	circle.m_p = b2Vec2(0, size.y - 1 / ge::SCALE);
	body->CreateFixture(&circle, 1.0f);

	circle.m_p = b2Vec2(0, -5 / ge::SCALE);
	body->CreateFixture(&circle, 1.0f);

	b2PolygonShape box;
	size = ge::convert(getSize() / 2 * scale);
	size.x = size.x - 10 / ge::SCALE;
	size.y = size.y - 6 / ge::SCALE;
	box.SetAsBox(size.x / 2, size.y / 2, b2Vec2(0, .060), 0);
	body->CreateFixture(&box, 1.0f);

	//set collision categories
	b2Filter filter = body->GetFixtureList()->GetFilterData();
	filter.categoryBits = playerCat;
	filter.maskBits = enemyCat | groundCat;
	body->GetFixtureList()->SetFilterData(filter);

	//add foot sensor fixture
	box.SetAsBox(0.07, 0.07, b2Vec2(0, .3), 0);
	body->CreateFixture(&box, 1.0f);
	//set collision categories
	filter = body->GetFixtureList()->GetFilterData();
	filter.categoryBits = playerCat;
	filter.maskBits = enemyCat | groundCat;
	body->GetFixtureList()->SetFilterData(filter);
	body->SetUserData(this);
	body->GetFixtureList()->SetSensor(true);
	body->GetFixtureList()->SetUserData((void*)"foot");


	body->SetFixedRotation(true);
	vel = body->GetLinearVelocity();
	_world = world;

}

void Player::doUpdate(const UpdateState & us)
{
	if (attached) {
		const uint8* data = SDL_GetKeyboardState(NULL);
		bool moved = false;

		vel = body->GetLinearVelocity();

		float speed = static_cast<double>(state["playerSpeed"]) / ge::SCALE;

		dtTotal += us.dt;

		if (stunned) {
			stun_dt += us.dt;
			stun();
		}

		if (weapon_dt < 2000) {
			weapon_dt += us.dt;
		}

		if (!fire) {
			switch (weapon) {
			case mg:
				if (weapon_dt >= mgR) {
					fire = true;
				}
				break;
			case rocket:
				if (weapon_dt >= rocketR) {
					fire = true;
				}
				break;
			}
		}

		if (data[SDL_GetScancodeFromKey(SDLK_d)]) {
			if (getResAnim() != res::gm.getResAnim("playerWalkRight") && doubleJump == 2) {
				setResAnim(res::gm.getResAnim("playerWalkRight"), getAnimFrame().getColumn());
				if (!data[SDL_GetScancodeFromKey(SDLK_a)]) {
					setAnimFrame(getResAnim(), 0);
					dtTotal = 0;
					direction = right;
				}
			}
			else if (getResAnim() != res::gm.getResAnim("playerJumpRight") && doubleJump != 2) {
				setResAnim(res::gm.getResAnim("playerJumpRight"), getAnimFrame().getColumn());
				if (!data[SDL_GetScancodeFromKey(SDLK_a)]) {
					setAnimFrame(getResAnim(), 0);
					dtTotal = 0;
					direction = right;
				}
			}

			animateState();

			vel.x = speed;
			body->SetLinearVelocity(vel);

			moved = true;
		}
		if (data[SDL_GetScancodeFromKey(SDLK_a)]) {
			if (getResAnim() != res::gm.getResAnim("playerWalkLeft") && doubleJump == 2) {
				setResAnim(res::gm.getResAnim("playerWalkLeft"), getAnimFrame().getColumn());
				if (!data[SDL_GetScancodeFromKey(SDLK_d)]) {
					setAnimFrame(getResAnim(), 0);
					dtTotal = 0;
					direction = left;
				}
			}
			else if (getResAnim() != res::gm.getResAnim("playerJumpLeft") && doubleJump != 2) {
				setResAnim(res::gm.getResAnim("playerJumpLeft"), getAnimFrame().getColumn());
				if (!data[SDL_GetScancodeFromKey(SDLK_d)]) {
					setAnimFrame(getResAnim(), 0);
					dtTotal = 0;
					direction = left;
				}
			}
			if (!moved) {
				animateState();
			}

			vel.x = -speed;
			body->SetLinearVelocity(vel);

			moved = true;
		}
		if (data[SDL_GetScancodeFromKey(SDLK_SPACE)]) {
			if (getResAnim() != res::gm.getResAnim("playerJumpLeft") && getResAnim() == res::gm.getResAnim("playerWalkLeft") && !jumpPressed) {
				setResAnim(res::gm.getResAnim("playerJumpLeft"));
				setAnimFrame(getResAnim(), 0);
				dtTotal = 0;
			}
			else if (getResAnim() != res::gm.getResAnim("playerJumpRight") && getResAnim() == res::gm.getResAnim("playerWalkRight") && !jumpPressed) {
				setResAnim(res::gm.getResAnim("playerJumpRight"));
				setAnimFrame(getResAnim(), 0);
				dtTotal = 0;
			}
			else if (getResAnim() != res::gm.getResAnim("playerJumpLeft") && getResAnim() == res::gm.getResAnim("playerIdleLeft") && !jumpPressed) {
				setResAnim(res::gm.getResAnim("playerJumpLeft"));
				setAnimFrame(getResAnim(), 0);
				dtTotal = 0;
			}
			else if (getResAnim() != res::gm.getResAnim("playerJumpRight") && getResAnim() == res::gm.getResAnim("playerIdleRight") && !jumpPressed) {
				setResAnim(res::gm.getResAnim("playerJumpRight"));
				setAnimFrame(getResAnim(), 0);
				dtTotal = 0;
			}
			else if (getResAnim() == res::gm.getResAnim("playerJumpRight") && data[SDL_GetScancodeFromKey(SDLK_a)] && !data[SDL_GetScancodeFromKey(SDLK_d)]) {
				setResAnim(res::gm.getResAnim("playerJumpLeft"));
			}
			else if (getResAnim() == res::gm.getResAnim("playerJumpLeft") && data[SDL_GetScancodeFromKey(SDLK_d)] && !data[SDL_GetScancodeFromKey(SDLK_a)]) {
				setResAnim(res::gm.getResAnim("playerJumpRight"));
			}
			if (!moved) {
				animateState();
			}
			if (doubleJump != 0 && !jumpPressed) {
				vel.y = -700.0f / ge::SCALE;
				body->SetLinearVelocity(vel);
				moved = true;
				--doubleJump;
				jumpPressed = true;
				sfxPlayer.play("playerJump");
			}
		} if (!data[SDL_GetScancodeFromKey(SDLK_SPACE)]) {
			jumpPressed = false;
		}
		if (!moved) {
			if (getResAnim() == res::gm.getResAnim("playerWalkLeft") || getResAnim() == res::gm.getResAnim("playerJumpLeft") && doubleJump == 2) {
				setResAnim(res::gm.getResAnim("playerIdleLeft"));
				setAnimFrame(getResAnim(), 0);
				dtTotal = 0;
			}
			else if (getResAnim() == res::gm.getResAnim("playerWalkRight") || getResAnim() == res::gm.getResAnim("playerJumpRight") && doubleJump == 2) {
				setResAnim(res::gm.getResAnim("playerIdleRight"));
				setAnimFrame(getResAnim(), 0);
				dtTotal = 0;
			}
			if (!data[SDL_GetScancodeFromKey(SDLK_SPACE)]) {
				animateState();
			}

			vel.x = 0;
			body->SetLinearVelocity(vel);
		}
		else {
			if (doubleJump == 2) {
			}
		}

		if (vel.y == 0 && doubleJump < 2) {
			jumpless++;
		}

		if (jumpless == 5 && doubleJump < 2) {
			if (getResAnim() == res::gm.getResAnim("playerJumpLeft")) {
				setResAnim(res::gm.getResAnim("playerIdleLeft"));
				setAnimFrame(getResAnim(), 0);
				dtTotal = 0;
			}
			else if (getResAnim() == res::gm.getResAnim("playerJumpRight")) {
				setResAnim(res::gm.getResAnim("playerIdleRight"));
				setAnimFrame(getResAnim(), 0);
				dtTotal = 0;
			}
			jumpless = 0;
		}
	}
}

void Player::animateState()
{
	if (dtTotal / static_cast<int>(state["playerAnimRate"]) >= 1) {
		if (getAnimFrame().getColumn() == getResAnim()->getColumns() - 1) {
			setAnimFrame(getResAnim(), 0);
		}
		else {
			setAnimFrame(getResAnim(), getAnimFrame().getColumn() + 1);
		}
		dtTotal = 0;
	}

}

void Player::stun()
{
	if (stun_dt <= 250) {
		setAlpha(0);
	}
	else if (stun_dt > 250 && stun_dt <= 500) {
		setAlpha(255);
	}
	else if (stun_dt > 500 && stun_dt <= 750) {
		setAlpha(0);
	}
	else {
		setAlpha(255);

		b2Fixture* fixtureList = body->GetFixtureList();
		b2Filter filter;

		while (fixtureList) {
			filter = fixtureList->GetFilterData();
			filter.categoryBits = playerCat;
			filter.maskBits = enemyCat | groundCat;
			fixtureList->SetFilterData(filter);
			fixtureList = fixtureList->GetNext();
		}
		stun_dt = 0;
		stunned = false;
	}
}

void Player::startContact(b2Contact* contact)
{
	log::messageln("made contact");
	void* fixtureUserDataA = contact->GetFixtureA()->GetUserData();
	void* fixtureUserDataB = contact->GetFixtureB()->GetUserData();
	if (fixtureUserDataA == "foot" || fixtureUserDataB == "foot") {
		doubleJump = 2;
	} 

	Actor* actorA = (Actor*)contact->GetFixtureA()->GetBody()->GetUserData();
	Actor* actorB = (Actor*)contact->GetFixtureB()->GetBody()->GetUserData();

	if (actorA && actorB) {
		if (actorA->getUserData() == "enemy") {
			HP -= dynamic_cast<Enemy*>(actorA)->attack();
			stunned = true;

			b2Fixture* fixtureList = body->GetFixtureList();
			b2Filter filter;

			while (fixtureList) {
				filter = fixtureList->GetFilterData();
				filter.maskBits = groundCat;
				fixtureList->SetFilterData(filter);
				fixtureList = fixtureList->GetNext();
			}

			stun();
		}
		else if (actorB->getUserData() == "enemy") {
			HP -= dynamic_cast<Enemy*>(actorB)->attack();

			stunned = true;

			b2Fixture* fixtureList = body->GetFixtureList();
			b2Filter filter;

			while (fixtureList) {
				filter = fixtureList->GetFilterData();
				filter.maskBits = groundCat;
				fixtureList->SetFilterData(filter);
				fixtureList = fixtureList->GetNext();
			}

			stun();
		}
		else if (actorA->getUserData() == "projectile") {
			HP -= dynamic_cast<WeaponProjectile*>(actorA)->attack();
			stunned = true;

			b2Fixture* fixtureList = body->GetFixtureList();
			b2Filter filter;

			while (fixtureList) {
				filter = fixtureList->GetFilterData();
				filter.maskBits = groundCat;
				fixtureList->SetFilterData(filter);
				fixtureList = fixtureList->GetNext();
			}

			stun();
		}
		else if (actorB->getUserData() == "projectile") {
			HP -= dynamic_cast<WeaponProjectile*>(actorB)->attack();

			stunned = true;

			b2Fixture* fixtureList = body->GetFixtureList();
			b2Filter filter;

			while (fixtureList) {
				filter = fixtureList->GetFilterData();
				filter.maskBits = groundCat;
				fixtureList->SetFilterData(filter);
				fixtureList = fixtureList->GetNext();
			}

			stun();
		}
	}
}

void Player::endContact(b2Contact* contact)
{
	log::messageln("ended contact");
	log::messageln("DURRRRRRRRRRR@@@@@@@");
}

void Player::setPlayerPosition(const Vector2 & pos)
{
	body->SetTransform(ge::convert(pos), body->GetAngle());
}

int Player::getDirection()
{
	return direction;
}

int Player::getWeapon()
{
	return weapon;
}

int Player::getHP()
{
	return HP;
}

void Player::gg()
{
	stunned = false;
	setAlpha(255);

	b2Fixture* fixtureList = body->GetFixtureList();
	b2Filter filter;

	while (fixtureList) {
		filter = fixtureList->GetFilterData();
		filter.maskBits = 0;
		fixtureList->SetFilterData(filter);
		fixtureList = fixtureList->GetNext();
	}
}

void Player::respawn()
{
	body->SetTransform(ge::convert(Vector2(getPosition().x, 0)), body->GetAngle());

	b2Fixture* fixtureList = body->GetFixtureList();
	b2Filter filter;

	while (fixtureList) {
		filter = fixtureList->GetFilterData();
		filter.maskBits = enemyCat | groundCat;
		fixtureList->SetFilterData(filter);
		fixtureList = fixtureList->GetNext();
	}

	HP = 100;
}

bool Player::canFire()
{
	return fire;
}

void Player::fireWeapon()
{
	fire = false;
	weapon_dt = 0;
}

void Player::changeWeapon()
{
	if (weapon == rocket) {
		weapon = mg;
	}
	else if (weapon == mg) {
		weapon = rocket;
	}
}

void Player::op()
{
	HP = 100000;
}
