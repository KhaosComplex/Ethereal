#pragma once
#include "WeaponProjectile.h"


class Rocket : public WeaponProjectile {
public:
	Rocket(b2World * world, const Vector2 & pos, const Vector2 &endPos, float scale, int initID);
	~Rocket();
	void doUpdate(const UpdateState& us);
	int attack();

	void startContact(b2Contact* contact);
	void endContact(b2Contact* contact);
private:
	b2Body* body;
	b2Vec2 velocity;

	void destroy();


};