#include "Game.h"
#include "Player.h"
#include "resources.h"
#include "GameElement.h"
#include "MyButton.h"
#include "GameScene.h"
#include "SoundPlayer.h"
#include "SoundInstance.h"
#include "selene.h"

extern SoundPlayer sfxPlayer;
extern SoundPlayer musicPlayer;

extern sel::State state;


Game::Game() : _world(0), levelNum(0), dungeon(new Dungeon), keys{ false }, bossRoom(false), dt_total(0), pixelOffset(0), pixelWidth(1280), pixelHeight(720), oldTileBeginning(0), oldTileEnd(20), oldEnemyBeginning(0), oldEnemyEnd(0), bgRenderPos(0), heldClick(false), isGG(false)
{

}

Game::~Game()
{
	delete contactL;
	delete dungeon;
	delete _world;
	boss = nullptr;
	projectileList.clear();
	bgList.clear();
	bodyList.clear();
	for (int i = 0; i < 200; ++i) {
		for (int f = 0; f < 12; ++f) {
			tileList[f][i] = nullptr;
			enemyList[f][i] = nullptr;
		}
	}
}

void Game::init()
{
	state.Load("../data/scripts/constants.lua");

	//set the size of the world to the size of the stage
	setSize(Vector2(pixelWidth * 10, 720));

	//Create a new world with the gravity set to 20
	_world = new b2World(b2Vec2(0, 17.5));

	contactL = new ContactListener;
	_world->SetContactListener(contactL);

	dungeon->generateDungeon();
	dungeon->loadFullDungeon();

	//create the player
	player = new Player;
	player->_init(_world, Vector2(getStage()->getWidth() / 2 - 100, getStage()->getHeight() / 2), static_cast<double>(state["playerScale"]));

	initGUI();

	for (int i = 0; i < 10; ++i) {
		dungeon->loadDungeonRoom();
		renderLevel(pixelOffset);
		pixelOffset++;
	}
	bgList.at(0)->attachTo(this);

	for (int i = oldTileBeginning; i < 20; ++i) {
		for (int f = 0; f < 12; ++f) {
			if (tileList[f][i]) {
				tileList[f][i]->attachTo(this);
			}
		}
	}

	for (int i = oldEnemyBeginning; i < 30; ++i) {
		for (int f = 0; f < 12; ++f) {
			if (enemyList[f][i]) {
				enemyList[f][i]->attachTo(this);
				enemyList[f][i]->setAttached(true);
			}
		}
	}


	player->setPriority(3);
	player->setAttached(true);
	addChild(player);

	addEventListener(TouchEvent::TOUCH_DOWN, CLOSURE(this, &Game::click));
	addEventListener(TouchEvent::TOUCH_UP, CLOSURE(this, &Game::click));
	addEventListener(TouchEvent::MOVE, CLOSURE(this, &Game::click));

	musicPlayer.play("gameTrack", PlayOptions().loop());
}

void Game::doUpdate(const UpdateState& us)
{
	dt_total += us.dt;

	HP->setText("HP: " + std::to_string(player->getHP()));
	if (player->getHP() <= 0 && !isGG) {
		gg();
		isGG = true;
	}

	if (heldClick) {
		if (player->canFire()) {
			if (player->getWeapon() == Player::rocket) {
				Rocket* rock = new Rocket(_world, Vector2(player->getX() + ((mousePosition).x - player->getX() > 0 ? 30 : -30), player->getY()), mousePosition, static_cast<double>(state["rocketScale"]), projectileList.size());
				rock->attachTo(this);
				projectileList.push_back(rock);

				sfxPlayer.play("rocketFire");

				player->fireWeapon();
			}
			else if (player->getWeapon() == Player::mg) {
				MachineGun* mg = new MachineGun(_world, Vector2(player->getX() + ((mousePosition).x - player->getX() > 0 ? 30 : -30), player->getY()), mousePosition, 0.30f, projectileList.size());
				mg->attachTo(this);
				projectileList.push_back(mg);

				sfxPlayer.play("rocketFire");

				player->fireWeapon();
			}
		}
	}

	if (boss->canFire()) {
		TurretBossProjectile* tb = new TurretBossProjectile(_world, Vector2(boss->getX() + -30, boss->getY()), player->getPosition(), 3.0f, projectileList.size());
		tb->attachTo(this);
		projectileList.push_back(tb);

		boss->fireWeapon();
	}

	if (player->getX() >= (getWidth() / 20) && player->getX() <= (getWidth() / 20) * 19 + 2 && !bossRoom) {
		int x = (player->getX() - (getWidth() / 20));
		if (x <= 3) {
			x = 0;
		}
		else if (x >= (getWidth() - getWidth() / 10 - 5)) {
			x = (getWidth() - getWidth() / 10);
			bossRoom = true;
			boss->start();
		}
		this->setPosition(Vector2(-x, getY()));
		_ui->setPosition(Vector2(x, getY()));
	}

	int visibleTileXB = ceil(-(getPosition().x) / dungeon->getCurrentRoom()->currentMap->GetTileWidth());
	int visibleTileXE = ceil((-(getPosition().x) + getWidth() / 10) / dungeon->getCurrentRoom()->currentMap->GetTileWidth());

	int visibleEnemyXB = ceil((-(getPosition().x) - getWidth() / 20) / dungeon->getCurrentRoom()->currentMap->GetTileWidth());
	int visibleEnemyXE = ceil((-(getPosition().x) + getWidth() / 10 + getWidth() / 20) / dungeon->getCurrentRoom()->currentMap->GetTileWidth());
	if (visibleEnemyXB < 0) {
		visibleEnemyXB = 0;
	}
	if (visibleEnemyXE > getWidth() / dungeon->getCurrentRoom()->currentMap->GetTileWidth()) {
		visibleEnemyXE = getWidth() / dungeon->getCurrentRoom()->currentMap->GetTileWidth();
	}

	if (visibleTileXB != oldTileBeginning) {
		if (visibleTileXB > oldTileBeginning) {
			//bg
			if (-(getPosition().x) > bgRenderPos) {
				if (bgRenderPos != 0) {
					bgList.at((bgRenderPos / (getWidth() / 10)) - 1)->detach();
				}
				bgList.at((bgRenderPos / (getWidth() / 10)) + 1)->attachTo(this);
				bgRenderPos += getWidth() / 10;
			}

			//Tiles
			if (oldTileBeginning != 0) {
				for (int i = oldTileBeginning - 1; i < visibleTileXB - 1; ++i) {
					for (int f = 0; f < 12; ++f) {
						if (tileList[f][i]) {
							tileList[f][i]->detach();
						}
					}
				}
			}


			for (int i = oldTileEnd; i < visibleTileXE; ++i) {
				for (int f = 0; f < 12; ++f) {
					if (tileList[f][i]) {
						tileList[f][i]->attachTo(this);
					}
				}
			}

			//Enemies
			if (oldEnemyBeginning != 0) {
				for (int i = oldEnemyBeginning - 1; i < visibleEnemyXB - 1; ++i) {
					for (int f = 0; f < 12; ++f) {
						if (enemyList[f][i]) {
							if (!enemyList[f][i]->isDead()) {
								enemyList[f][i]->detach();
								enemyList[f][i]->setAttached(false);
								enemyList[f][i]->halt();
							}
						}
					}
				}
			}

			for (int i = oldEnemyEnd; i < visibleEnemyXE; ++i) {
				for (int f = 0; f < 12; ++f) {
					if (enemyList[f][i]) {
						if (!enemyList[f][i]->isDead()) {
							enemyList[f][i]->attachTo(this);
							enemyList[f][i]->setAttached(true);
						}
					}
				}
			}
		}
		else {
			//bg
			if (-(getPosition().x) < bgRenderPos) {
				if (bgRenderPos != getWidth() - getWidth() / 10) {
					bgList.at((bgRenderPos / (getWidth() / 10)) + 1)->detach();
				}
				bgList.at((bgRenderPos / (getWidth() / 10)) - 1)->attachTo(this);
				bgRenderPos -= getWidth() / 10;
			}

			//tiles
			for (int i = visibleTileXE; i < oldTileEnd; ++i) {
				for (int f = 0; f < 12; ++f) {
					if (tileList[f][i]) {
						tileList[f][i]->detach();
					}
				}
			}
			if (visibleTileXB != 0) {
				for (int i = visibleTileXB - 1; i < oldTileBeginning - 1; ++i) {
					for (int f = 0; f < 12; ++f) {
						if (tileList[f][i]) {
							tileList[f][i]->attachTo(this);
						}
					}
				}
			}

			//Enemies
			for (int i = visibleEnemyXE - 1; i < oldEnemyEnd - 1; ++i) {
				for (int f = 0; f < 12; ++f) {
					if (enemyList[f][i]) {
						if (!enemyList[f][i]->isDead()) {
							enemyList[f][i]->detach();
							enemyList[f][i]->setAttached(false);
							enemyList[f][i]->halt();
						}
					}
				}
			}

			if (visibleEnemyXB != 0) {
				for (int i = visibleEnemyXB; i < oldEnemyBeginning; ++i) {
					for (int f = 0; f < 12; ++f) {
						if (enemyList[f][i]) {
							if (!enemyList[f][i]->isDead()) {
								enemyList[f][i]->attachTo(this);
								enemyList[f][i]->setAttached(true);
							}
						}
					}
				}
			}
		}
	}

	oldTileBeginning = visibleTileXB;
	oldTileEnd = visibleTileXE;

	oldEnemyBeginning = visibleEnemyXB;
	oldEnemyEnd = visibleEnemyXE;

	const uint8* data = SDL_GetKeyboardState(NULL);
	if (data[SDL_GetScancodeFromKey(SDLK_e)]) {
		if (!keys[SDLK_e]) {
			player->changeWeapon();
			keys[SDLK_e] = true;
		}
	}
	else {
		keys[SDLK_e] = false;
	}
	if (data[SDL_GetScancodeFromKey(SDLK_p)]) {
		if (!keys[SDLK_p]) {
			player->op();
		}
	}
	else {
		keys[SDLK_p] = false;
	}

	//step our world, use delta time to make sure consistency across framerates.
	if (dt_total >= 17) {
		_world->Step(ge::FIXED_TIMESTEP, 6, 2);
		dt_total = 0;
	}

	int projectileCount = 0;

	//update each body position on display
	b2Body* body = _world->GetBodyList();
	while (body)
	{
		Actor* actor = (Actor*)body->GetUserData();
		b2Body* next = body->GetNext();
		if (actor)
		{
			const b2Vec2& pos = body->GetPosition();
			if (dynamic_cast<Enemy*>(actor) != nullptr) {
				dynamic_cast<Enemy*>(actor)->setPosition(ge::convert(pos));
				dynamic_cast<Enemy*>(actor)->setRotation(body->GetAngle());
			}
			else {
				actor->setPosition(ge::convert(pos));
				actor->setRotation(body->GetAngle());
			}


			if (actor->getUserData() == (void*)"player") {
				if (actor->getY() > getHeight() + 50 || actor->getX() > getWidth() + 50 || actor->getX() < -50)
				{
					player->detach();
					_world->DestroyBody(body);
					player->setAttached(false);
				}
			}
			else if (dynamic_cast<WeaponProjectile*>(actor) != nullptr)
			{
				projectileCount++;
				if (dynamic_cast<WeaponProjectile*>(actor)->isDead())
				{
					projectileList.at(dynamic_cast<WeaponProjectile*>(actor)->getID()) = nullptr;
					_world->DestroyBody(body);
					actor->detach();
				}
				else if (actor->getY() > getHeight() + 50 || actor->getY() < -300 || actor->getX() > getWidth() + 50 || actor->getX() < -50)
				{
					_world->DestroyBody(body);
					actor->detach();
				}
			}
			else if (dynamic_cast<Enemy*>(actor) != nullptr)
			{
				if (dynamic_cast<Enemy*>(actor)->isDead())
				{
					_world->DestroyBody(body);
					actor->detach();
					dynamic_cast<Enemy*>(actor)->setAttached(false);
				}
				else if (dynamic_cast<Totem*>(actor) != nullptr) {
					if (dynamic_cast<Totem*>(actor)->canFire() && actor->getX() <= player->getX() + 800 && actor->getX() >= player->getX() - 800) {
						TotemProjectile* tp = new TotemProjectile(_world, Vector2(actor->getX() + -30, actor->getY()), player->getPosition(), 0.25f, projectileList.size());
						tp->attachTo(this);
						projectileList.push_back(tp);

						dynamic_cast<Totem*>(actor)->fireWeapon();
					}
				}
				if (!dynamic_cast<Enemy*>(actor)->isAttached() && !dynamic_cast<Enemy*>(actor)->isDead() && actor->getPosition().x >= -getPosition().x && actor->getPosition().x <= -getPosition().x+getWidth()/10) {
					actor->attachTo(this);
					dynamic_cast<Enemy*>(actor)->setAttached(true);
				} 
			}
			else {
				//remove fallen bodies
				if (actor->getY() > getHeight() + 50 || actor->getY() < -300 || actor->getX() > getWidth() + 50 || actor->getX() < -50)
				{
					body->SetUserData(0);
					_world->DestroyBody(body);

					actor->detach();
				}
			}
		}

		body = next;
	}
	if (projectileCount == 0) {
		projectileList.clear();
	}
}

void Game::showDebug(Event* event)
{
	TouchEvent* te = safeCast<TouchEvent*>(event);
	te->stopsImmediatePropagation = true;
	if (_debugDraw)
	{
		_debugDraw->detach();
		_debugDraw = 0;
		return;
	}

	_debugDraw = new Box2DDraw;
	_debugDraw->SetFlags(b2Draw::e_shapeBit | b2Draw::e_jointBit | b2Draw::e_pairBit | b2Draw::e_centerOfMassBit);
	_debugDraw->attachTo(this);
	_debugDraw->setWorld(ge::SCALE, _world);
	_debugDraw->setPriority(6);
}

void Game::onEvent(Event* ev) {
	string id = ev->currentTarget->getName();
	if (id == "back")
	{
		//back to main menu
		GameScene::instance->changeGameScene();

	}

}

void Game::click(Event* event)
{
	//log::messageln("CLICKED!");
	TouchEvent* te = safeCast<TouchEvent*>(event);

	if (te->type == TouchEvent::TOUCH_DOWN) {
		heldClick = true;
	}
	else if (te->type == TouchEvent::TOUCH_UP) {
		heldClick = false;
	}
	else {
		mousePosition = te->localPosition;
	}
}

void Game::initGUI()
{

	//create separate layer for elements virtual joystick and other UI in future
	_ui = new Actor;
	_ui->attachTo(this);
	//it would be higher than other actors with default priority = 0
	_ui->setPriority(4);

	//create back button
	spSprite btn = initActor(new MyButton,
		arg_name = "back",
		arg_resAnim = res::mm.getResAnim("menu_back"),
		arg_anchor = Vector2(0.5f, 0.5f),
		arg_pos = Vector2(getWidth() / 10 - 100, 50),
		arg_attachTo = _ui);

	//handle click to button
	btn->addEventListener(TouchEvent::CLICK, CLOSURE(this, &Game::onEvent));

	//create show debug button
	btn = new Button;
	btn->setPosition(Vector2(getWidth() / 10 - btn->getWidth() / 2 - 300, btn->getHeight() / 2));
	btn->attachTo(_ui);
	btn->addEventListener(TouchEvent::CLICK, CLOSURE(this, &Game::showDebug));

	HP = new TextField;
	HP->attachTo(_ui);
	HP->setSize(Vector2(300, 0));
	HP->setPosition(getWidth() / 20, 50);
	HP->setText("HP: " + std::to_string(player->getHP()));

	TextStyle style;
	style.font = res::gm.getResFont("main")->getFont();
	style.color = Color::White;
	HP->setStyle(style);

}

void Game::renderLevel(int offsetModifier) {
	dungeon->getCurrentRoom()->bg->setPriority(0);
	dungeon->getCurrentRoom()->bg->setUserData((void*) "bg");
	if (dungeon->getCurrentRoom()->bg->getPosition().x != (pixelWidth * offsetModifier)) {
		dungeon->getCurrentRoom()->bg->setPosition(Vector2(dungeon->getCurrentRoom()->bg->getPosition().x + (pixelWidth * offsetModifier), dungeon->getCurrentRoom()->bg->getPosition().y));
	}
	bgList.push_back(dungeon->getCurrentRoom()->bg);

	for (int tileLayerN = 0; tileLayerN != dungeon->getCurrentRoom()->currentMap->GetNumTileLayers(); ++tileLayerN) {
		int tileCounter = 0;
		const Tmx::TileLayer *tileLayer = dungeon->getCurrentRoom()->currentMap->GetTileLayer(tileLayerN);
		if (tileLayer->GetName() == "GameElements") {
			for (int y = 0; y < tileLayer->GetHeight(); ++y)
			{
				for (int x = 0; x < tileLayer->GetWidth(); ++x)
				{
					int counter = 0;
					int gid = tileLayer->GetTileGid(x, y);
					if (gid != 0) {
						for (auto gidIT = dungeon->getCurrentRoom()->gidNums.begin(); gidIT != dungeon->getCurrentRoom()->gidNums.end(); gidIT++) {
							int gidCur = *gidIT;
							if (gid <= gidCur || counter == dungeon->getCurrentRoom()->gidNums.size() - 1) {
								int realGid = gid - gidCur;
								if (realGid >= 0) {
									if (realGid == 2) {
										spGoblin gob = new Goblin();
										int posCol = tileCounter / dungeon->getCurrentRoom()->currentMap->GetWidth();
										int posRow = tileCounter % dungeon->getCurrentRoom()->currentMap->GetWidth();
										Vector2 pos = Vector2(posRow * dungeon->getCurrentRoom()->currentMap->GetTileWidth() + dungeon->getCurrentRoom()->currentMap->GetTileWidth() / 2 + (pixelWidth * offsetModifier), posCol * dungeon->getCurrentRoom()->currentMap->GetTileHeight() + dungeon->getCurrentRoom()->currentMap->GetTileHeight());
										gob->_init(_world, pos, static_cast<double>(state["goblinScale"]));
										gob->setPriority(2);
										enemyList[(int)(pos.y / dungeon->getCurrentRoom()->currentMap->GetTileHeight())][(int)(pos.x / dungeon->getCurrentRoom()->currentMap->GetTileWidth())] = gob;
									}
									else if (realGid == 1) {
										spTotem tot = new Totem();
										int posCol = tileCounter / dungeon->getCurrentRoom()->currentMap->GetWidth();
										int posRow = tileCounter % dungeon->getCurrentRoom()->currentMap->GetWidth();
										Vector2 pos = Vector2(posRow * dungeon->getCurrentRoom()->currentMap->GetTileWidth() + dungeon->getCurrentRoom()->currentMap->GetTileWidth() / 2 + (pixelWidth * offsetModifier), posCol * dungeon->getCurrentRoom()->currentMap->GetTileHeight() + dungeon->getCurrentRoom()->currentMap->GetTileHeight());
										tot->_init(_world, pos, 0.20f);
										tot->setPriority(2);
										enemyList[(int)(pos.y / dungeon->getCurrentRoom()->currentMap->GetTileHeight())][(int)(pos.x / dungeon->getCurrentRoom()->currentMap->GetTileWidth())] = tot;
									}
									else if (realGid == 0) {
										spTurretBoss tBoss = new TurretBoss();
										int posCol = tileCounter / dungeon->getCurrentRoom()->currentMap->GetWidth();
										int posRow = tileCounter % dungeon->getCurrentRoom()->currentMap->GetWidth();
										Vector2 pos = Vector2(posRow * dungeon->getCurrentRoom()->currentMap->GetTileWidth() + dungeon->getCurrentRoom()->currentMap->GetTileWidth() / 2 + (pixelWidth * offsetModifier), posCol * dungeon->getCurrentRoom()->currentMap->GetTileHeight() + dungeon->getCurrentRoom()->currentMap->GetTileHeight());
										tBoss->_init(_world, pos, 1.0f);
										tBoss->setPriority(2);
										boss = tBoss;
										tBoss->attachTo(this);
									}
								}
								gidIT = --dungeon->getCurrentRoom()->gidNums.end();
							}
							else {
								counter++;
							}
						}
					}
					tileCounter++;
				}
			}
		}
		else if (tileLayer->GetName() == "bg") {
		}
		else {
			for (int y = 0; y < tileLayer->GetHeight(); ++y)
			{
				for (int x = 0; x < tileLayer->GetWidth(); ++x)
				{
					int counter = 0;
					int gid = tileLayer->GetTileGid(x, y);
					if (gid != 0) {
						for (auto gidIT = dungeon->getCurrentRoom()->gidNums.begin(); gidIT != dungeon->getCurrentRoom()->gidNums.end(); gidIT++) {
							int gidCur = *gidIT;
							if (gid >= gidCur || counter == dungeon->getCurrentRoom()->gidNums.size() - 1) {
								int realGid = gid - gidCur;
								if (realGid >= 0) {
									spSprite newTile = new Sprite;
									spSprite tileSP = dungeon->getCurrentRoom()->tileSpriteList.at(counter);
									int posCol = tileCounter / dungeon->getCurrentRoom()->currentMap->GetWidth();
									int posRow = tileCounter % dungeon->getCurrentRoom()->currentMap->GetWidth();
									newTile->setResAnim(tileSP->getResAnim(), realGid, 0);
									newTile->setPosition(Vector2(posRow * dungeon->getCurrentRoom()->currentMap->GetTileWidth() + (pixelWidth * offsetModifier), posCol * dungeon->getCurrentRoom()->currentMap->GetTileHeight()));
									newTile->setPriority(1);
									tileList[(int)(newTile->getPosition().y / dungeon->getCurrentRoom()->currentMap->GetTileHeight())][(int)(newTile->getPosition().x / dungeon->getCurrentRoom()->currentMap->GetTileWidth())] = newTile;
								}
								gidIT = --dungeon->getCurrentRoom()->gidNums.end();
							}
							else {
								counter++;
							}
						}
					}
					tileCounter++;
				}
			}
		}
	}
	for (int objN = 0; objN != dungeon->getCurrentRoom()->currentMap->GetNumObjectGroups(); ++objN) {
		const Tmx::ObjectGroup *objectGroup = dungeon->getCurrentRoom()->currentMap->GetObjectGroup(objN);
		for (int oGN = 0; oGN != objectGroup->GetNumObjects(); ++oGN) {
			const Tmx::Object *object = objectGroup->GetObject(oGN);

			b2BodyDef groundBodyDef;
			groundBodyDef.position = ge::convert(Vector2(object->GetX() + object->GetWidth() / 2 + (pixelWidth * offsetModifier), object->GetY() + object->GetHeight() / 2));

			b2Body* groundBody = _world->CreateBody(&groundBodyDef);

			b2PolygonShape groundBox;
			b2Vec2 sz = ge::convert(Vector2(object->GetWidth() / 2, object->GetHeight() / 2));
			groundBox.SetAsBox(sz.x, sz.y);
			groundBody->CreateFixture(&groundBox, 0.0f);
			groundBody->GetFixtureList()->SetFriction(0.0f);
			b2Filter filter = groundBody->GetFixtureList()->GetFilterData();
			filter.categoryBits = 0x0008;
			filter.maskBits = 0x0001 | 0x0002 | 0x0004;
			groundBody->GetFixtureList()->SetFilterData(filter);

			bodyList.push_back(_world->GetBodyList());

		}
	}
}

void Game::unloadLevel()
{

}

void Game::gg()
{
	player->gg();
}
