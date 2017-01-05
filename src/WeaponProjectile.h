#pragma once
#include "ContactObject.h"


class WeaponProjectile : public ContactObject
{
public:
	virtual int attack() = 0;

	bool isDead() { return currentState == dead; };
	int getID() { return id; }

protected:
	WeaponProjectile(int initDamage, int initID) : damage(initDamage), id(initID), currentState(alive), ContactObject() {};

	virtual void doUpdate(const UpdateState& us) = 0;

	enum State { alive, dying, dead };
	State currentState;

	virtual void destroy() = 0;

	int damage;
	int id;
};