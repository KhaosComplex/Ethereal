#include "MainMenuScene.h"
#include "HelpMenuScene.h"
#include "AboutMenuScene.h"
#include "GameScene.h"
#include "Sprite.h"
#include "initActor.h"
#include "MyButton.h"
#include "resources.h"
#include "GameElement.h"

spMainMenuScene MainMenuScene::instance;

MainMenuScene::MainMenuScene()
{
	//create background
	spSprite bg = new Sprite;
	bg->setResAnim(res::mm.getResAnim("menu_bg_logoless"));
	bg->attachTo(_view);

	//create logo
	spSprite logo = initActor(new Sprite,
		arg_resAnim = res::mm.getResAnim("logo"),
		arg_attachTo = _view,
		arg_pos = Vector2(110, 50));


	/*
	spSprite btn = new MyButton;
	btn->setResAnim(res::ui.getResAnim("play")),
	btn->setAnchor(Vector2(0.5f, 0.5f));
	btn->setPosition(_view->getSize()/2);
	btn->attachTo(_view);
	*/

	//create start button
	spSprite btn = initActor(new MyButton,
		arg_name = "start",
		arg_resAnim = res::mm.getResAnim("menu_start"),
		arg_anchor = Vector2(0.5f, 0.5f),
		arg_pos = Vector2((float)getStage()->getWidth()/2 - 100, (float)getStage()->getHeight() / 2 + 100),
		arg_attachTo = _view);

	//handle click to button
	btn->addEventListener(TouchEvent::CLICK, CLOSURE(this, &MainMenuScene::onEvent));

	//create the exit button
	btn = initActor(new MyButton,
		arg_name = "exit",
		arg_resAnim = res::mm.getResAnim("menu_exit"),
		arg_anchor = Vector2(0.5f, 0.5f),
		arg_pos = Vector2((float)getStage()->getWidth() / 2 + 100, (float)getStage()->getHeight() / 2 + 100),
		arg_attachTo = _view);

	btn->addEventListener(TouchEvent::CLICK, CLOSURE(this, &MainMenuScene::onEvent));

	//create the about button
	btn = initActor(new MyButton,
		arg_name = "about",
		arg_resAnim = res::mm.getResAnim("menu_about"),
		arg_anchor = Vector2(0.5f, 0.5f),
		arg_pos = Vector2((float)getStage()->getWidth() / 2 + 300, (float)getStage()->getHeight() / 2 + 100),
		arg_attachTo = _view);

	btn->addEventListener(TouchEvent::CLICK, CLOSURE(this, &MainMenuScene::onEvent));

	//create the help button
	btn = initActor(new MyButton,
		arg_name = "help",
		arg_resAnim = res::mm.getResAnim("menu_help"),
		arg_anchor = Vector2(0.5f, 0.5f),
		arg_pos = Vector2((float)getStage()->getWidth() / 2 - 300, (float)getStage()->getHeight() / 2 + 100),
		arg_attachTo = _view);

	btn->addEventListener(TouchEvent::CLICK, CLOSURE(this, &MainMenuScene::onEvent));
}

void MainMenuScene::onEvent(Event* ev)
{
	string id = ev->currentTarget->getName();
	if (id == "exit")
	{
		//request quit
		core::requestQuit();
	}

	if (id == "start")
	{
		//clicked to play button
		//change scene NEW FEATURE
		GameScene::instance->init();
		changeScene(GameScene::instance);
	}

	if (id == "help")
	{
		changeScene(HelpMenuScene::instance);
	}

	if (id == "about")
	{
		changeScene(AboutMenuScene::instance);
	}
}