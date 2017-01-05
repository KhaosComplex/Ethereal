#pragma once
#include "oxygine-framework.h"
#include "Box2D/Box2D.h"

using namespace oxygine;
//Game Elements
DECLARE_SMART(StaticObj, spStatic);

namespace ge
{
	const float SCALE = 100.0f;
	const float FIXED_TIMESTEP = 1.0f / 60.0f;
	b2Vec2 convert(const Vector2& pos);
	Vector2 convert(const b2Vec2& pos);

	enum soundType {
		jump
	};
}

class StaticObj : public Box9Sprite
{
public:
	StaticObj(b2World* world, const RectF& rc);
};


