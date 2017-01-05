#pragma once
#include "oxygine-framework.h"
#include "Box2D/Box2D.h"
#include "ContactListener.h"
#include "Box2DDebugDraw.h"
#include "Room.h"
#include <list>
#include "WeaponProjectile.h"
#include "Dungeon.h"

#include "Enemy.h"
#include "Goblin.h"
#include "Totem.h"
#include "TurretBoss.h"
#include "TurretBossProjectile.h"
#include "TotemProjectile.h"
using namespace oxygine;

DECLARE_SMART(Player, spPlayer)
DECLARE_SMART(Goblin, spGoblin)
DECLARE_SMART(Totem, spTotem)
DECLARE_SMART(TurretBoss, spTurretBoss)
DECLARE_SMART(Game, spGame)
DECLARE_SMART(WeaponProjectile, spWeaponProjectile)
DECLARE_SMART(Enemy, spEnemy)

class Game : public Actor
{
public:
	Game();
	~Game();

	void init();

protected:
	b2World* _world;
	void doUpdate(const UpdateState& us);
    void Game::showDebug(Event* ev);
	void onEvent(Event* ev);
	void click(Event* event);

	spActor _ui;

	spPlayer player;


private:
	ContactListener* contactL;
	spBox2DDraw _debugDraw;
	int levelNum;
	Dungeon* dungeon;
	std::vector<spWeaponProjectile> projectileList;
	//Data from the level
	spSprite tileList[12][200];
	spEnemy enemyList[12][200];
	std::vector<spSprite> bgList;
	std::list<b2Body*> bodyList;
	spTurretBoss boss;
	bool keys[256];
	spTextField HP;
	int pixelOffset;
	const int pixelWidth;
	const int pixelHeight;
	int dt_total;
	int counterForDEBUG = 0;
	bool bossRoom;
	bool heldClick;
	bool isGG;
	int oldTileBeginning;
	int oldTileEnd;
	int oldEnemyBeginning;
	int oldEnemyEnd;
	int bgRenderPos;
	Vector2 mousePosition;

	void initGUI();
	void renderLevel(int offsetModifier);
	void unloadLevel();
	void gg();

};

