#include "GameElement.h"
#include "resources.h"
#include "MyButton.h"
#include "GameScene.h"
#include "Scene.h"

namespace ge {
	b2Vec2 convert(const Vector2& pos)
	{
		return b2Vec2(pos.x / SCALE, pos.y / SCALE);
	}

	Vector2 convert(const b2Vec2& pos)
	{
		return Vector2(pos.x * SCALE, pos.y * SCALE);
	}


}

StaticObj::StaticObj(b2World* world, const RectF& rc)
{

	setResAnim(res::gm.getResAnim("floor"));
	setSize(rc.getSize());
	setPosition(rc.getLeftTop());
	setAnchor(Vector2(0.5f, 0.5f));
	setVisible(false);

	b2BodyDef groundBodyDef;
	groundBodyDef.position = ge::convert(getPosition());

	b2Body* groundBody = world->CreateBody(&groundBodyDef);

	b2PolygonShape groundBox;
	b2Vec2 sz = ge::convert(getSize() / 2);
	groundBox.SetAsBox(sz.x, sz.y);
	groundBody->CreateFixture(&groundBox, 0.0f);
}