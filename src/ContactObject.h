#pragma once

#include "oxygine-framework.h"
#include "Box2D\Box2D.h"
#include "Resources.h"
#include "GameElement.h"
#include "selene.h"
using namespace oxygine;

extern sel::State state;

class ContactObject : public Sprite
{
public:
	virtual void startContact(b2Contact* contact) = 0;
	virtual void endContact(b2Contact* contact) = 0;
protected:
	ContactObject() : dtTotal(0) {};

	enum CollisionCat { playerCat = 0x0001, enemyCat = 0x0002, weaponCat = 0x0004, groundCat = 0x0008 };
	int dtTotal;
};