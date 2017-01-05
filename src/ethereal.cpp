#include "oxygine-framework.h"
#include "resources.h"
#include "MainMenuScene.h"
#include "HelpMenuScene.h"
#include "AboutMenuScene.h"
#include "GameScene.h"
#include "selene.h"
#include "SoundPlayer.h"
#include "SoundSystem.h"

using namespace oxygine;
using namespace sel;

SoundPlayer sfxPlayer;
SoundPlayer musicPlayer;

State state{ true };

void ethereal_preinit()
{
}

void ethereal_init()
{
	//initialize our sound system with 16 channels
	SoundSystem::create()->init(16);

	//initialize SoundPlayer
	SoundPlayer::initialize();

	//load the resources
	res::load();

	//create all the scenes
	MainMenuScene::instance = new MainMenuScene;
	HelpMenuScene::instance = new HelpMenuScene;
	AboutMenuScene::instance = new AboutMenuScene;
	GameScene::instance = new GameScene;

	//show main menu
	MainMenuScene::instance->show();

	sfxPlayer.setResources(&res::sound);
	sfxPlayer.setVolume(0.5f);
	musicPlayer.setResources(&res::sound);
	musicPlayer.setVolume(0.3f);
}

void ethereal_update()
{
	SoundSystem::get()->update();
	sfxPlayer.update();
	musicPlayer.update();
}

void ethereal_destroy()
{
	sfxPlayer.stop();
	musicPlayer.stop();

	MainMenuScene::instance = 0;
	GameScene::instance = 0;
	HelpMenuScene::instance = 0;
	AboutMenuScene::instance = 0;

	res::free();

	SoundPlayer::free();
	SoundSystem::free();
}