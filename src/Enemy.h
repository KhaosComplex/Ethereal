#pragma once
#include "Character.h"

class Enemy : public Character {
public:
	Enemy(int initHP, int initDamage, int initRecharge) : Character(0), HP(initHP), Damage(initDamage), Recharge(initRecharge), currentState(alive) {};

	int attack() { return Damage; };

	void getHurt(int lostHP) { HP -= lostHP; };
	bool isDead() { return currentState == dead; };
 	void halt() { body->SetLinearVelocity(b2Vec2(0, 0)); body->SetTransform(ge::convert(originalPosition), body->GetAngle()); }
	const Vector2& getOriginalPosition() const { return originalPosition; }

protected:
	enum State { alive, dying, dead };
	State currentState;
	Vector2 originalPosition;

	int HP;
	const int Damage;
	const int Recharge;
	//Drop* drop;

	virtual void die() = 0;
};