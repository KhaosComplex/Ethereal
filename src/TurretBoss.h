#pragma once
#include "Enemy.h"

class TurretBoss : public Enemy
{
public:
	TurretBoss();
	~TurretBoss();

	void startContact(b2Contact* contact);
	void endContact(b2Contact* contact);

	void _init(b2World * world, const Vector2 &pos, float scale);
	void doUpdate(const UpdateState& us);
	bool canFire();
	void fireWeapon();
	void start();

private:
	int dtTotal;

	void animateState();
	void die();
	bool started;
	bool fire;
};