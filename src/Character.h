#pragma once
#include "ContactObject.h"
#include "SoundPlayer.h"
#include "SoundInstance.h"

extern SoundPlayer sfxPlayer;

class Character : public ContactObject
{
public:
	Character(int init) : frameCount(init), currentState(alive), attached(false), ContactObject() {};

	virtual void _init(b2World* world, const Vector2 & pos, float scale) = 0;
	bool isDead() { return currentState == dead; };

	b2Body* getBody() { return body; }
	void setAttached(bool newAttached) { attached = newAttached; }
	bool isAttached() { return attached; }

protected:
	enum State { alive, dying, dead };
	State currentState;
	virtual void doUpdate(const UpdateState& us) = 0;
	b2Body* body;
	b2Vec2 vel;
	uint8 frameCount;
	bool attached;
};
