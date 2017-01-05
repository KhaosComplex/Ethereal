#pragma once
#include "Enemy.h"

class Goblin : public Enemy
{
public:
	Goblin();
	~Goblin();

	void startContact(b2Contact* contact);
	void endContact(b2Contact* contact);

	void _init(b2World * world, const Vector2 &pos, float scale);
	void doUpdate(const UpdateState& us);
	void reverseDirection();

private:
	enum Direction { right, left };
	enum footState { nothing, floor, wall };
	int dtTotal;
	int firstPass;
	bool changedDirection;

	Direction direction;
	footState footState;
	void animateState();
	void die();
};