#pragma once
#include "Character.h"
#include "Rocket.h"
#include "MachineGun.h"

class Player : public Character
{
public:
	Player();
	void _init(b2World * world, const Vector2 & pos, float scale);

	void startContact(b2Contact* contact);
	void endContact(b2Contact* contact);

	void setPlayerPosition(const Vector2 & pos);
	int getDirection();
	int getWeapon();
	int getHP();
	void gg();
	void respawn();
	bool canFire();
	void fireWeapon();
	void changeWeapon();
	void op();

	enum Weapon { mg, rocket };

protected:
	virtual void doUpdate(const UpdateState& us);

private:
	enum Direction	{ right, left };
	enum WeaponRecharge { mgR = 50, rocketR = 600};

	Direction direction;
	Weapon weapon;

	uint8 doubleJump;
	uint8 jumpless;
	bool jumpPressed;
	void animateState();
	b2World* _world;
	bool stunned;
	bool fire;
	int stun_dt;
	int weapon_dt;

	int HP;

	void stun();



};
