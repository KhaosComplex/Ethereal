#include "HelpMenuScene.h"
#include "MainMenuScene.h"
#include "Sprite.h"
#include "initActor.h"
#include "MyButton.h"
#include "resources.h"

spHelpMenuScene HelpMenuScene::instance;

HelpMenuScene::HelpMenuScene()
{
	//create background
	spSprite bg = new Sprite;
	bg->setResAnim(res::mm.getResAnim("menu_bg_logoless"));
	bg->attachTo(_view);

	//create back button
	spSprite btn = initActor(new MyButton,
		arg_name = "back",
		arg_resAnim = res::mm.getResAnim("menu_back"),
		arg_anchor = Vector2(0.5f, 0.5f),
		arg_pos = Vector2((float)getStage()->getWidth() / 2 - 100, (float)getStage()->getHeight() / 2 + 100),
		arg_attachTo = _view);

	//handle click to button
	btn->addEventListener(TouchEvent::CLICK, CLOSURE(this, &HelpMenuScene::onEvent));

	//align it to top right
	btn->setX(_view->getWidth() - btn->getWidth() / 2);
	btn->setY(btn->getHeight() / 2);

	//create holder for button and text
	_text = new Actor;
	_text->attachTo(_view);

	//initialize TextStyle for TextField
	//TextStyle is plain struct with "setting" for Text
	TextStyle style;
	style.font = res::mm.getResFont("main")->getFont();
	//vertical align
	style.vAlign = TextStyle::VALIGN_BOTTOM;
	//horizontal align
	style.hAlign = TextStyle::HALIGN_CENTER;

	spTextField about = initActor(new TextField,
		arg_style = style,
		//colored text by "html" tags
		arg_htmlText = "Controls:"
		"\nAD for movement \(left-right\)."
		"\nSpacebar to jump."
		"\nPoint with the mouse to aim, Click to fire."
		"\n\nGet to the end without losing all your health!"
		"\n\nIf you're having trouble, press \"p\" to gain infinite life!"
		"\n\nGood Luck!",
		arg_x = _view->getWidth() / 2,
		arg_y = _view->getHeight() / 2 + 65,
		arg_attachTo = _text,
		arg_alpha = 128);

	//animate with infinity loops "Paused!" text
	about->addTween(Actor::TweenAlpha(255), 1200, -1, true);
	about->addTween(Actor::TweenScale(1.1f), 1200, -1, true);
}

void HelpMenuScene::onEvent(Event* ev)
{
	string id = ev->currentTarget->getName();
	if (id == "back")
	{
		//back to main menu
		changeScene(MainMenuScene::instance);
	}
}