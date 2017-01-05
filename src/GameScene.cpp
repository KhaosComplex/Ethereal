#include "GameScene.h"
#include "MainMenuScene.h"
#include "MyButton.h"
#include "resources.h"
#include "GameElement.h"
#include "SoundPlayer.h"
#include "SoundInstance.h"

extern SoundPlayer sfxPlayer;
extern SoundPlayer musicPlayer;

spGameScene GameScene::instance;

GameScene::GameScene()
{
}

void GameScene::changeGameScene() {
	changeScene(MainMenuScene::instance);
	_game->detach();
	sfxPlayer.stop();
	musicPlayer.stop();
}

void GameScene::init() {
	_game = new Game;
	_game->init();
	_game->attachTo(_view);

	//Game Actor would have own Clock.
	//clock is internal time of each Actor
	//by default own clock has only Stage
	//clock could be paused and all children of this Actor would be paused to
	_game->setClock(new Clock);
}