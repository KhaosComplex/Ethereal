#include "MyButton.h"
#include "SoundPlayer.h"
#include "SoundInstance.h"

extern SoundPlayer sfxPlayer;

MyButton::MyButton() : buttonSound(-1)
{
	//pressed button should be RED
	addEventListener(TouchEvent::TOUCH_DOWN, CLOSURE(this, &MyButton::onEvent));
	addEventListener(TouchEvent::TOUCH_UP, CLOSURE(this, &MyButton::onEvent));
	addEventListener(TouchEvent::CLICK, CLOSURE(this, &MyButton::onEvent));
}

void MyButton::onEvent(Event* ev)
{
	TouchEvent* event = static_cast<TouchEvent*>(ev);
	if (ev->type == TouchEvent::TOUCH_DOWN)
		setColor(Color::Aqua);

	if (ev->type == TouchEvent::TOUCH_UP)
		setColor(Color::White);

	if (ev->type == TouchEvent::CLICK)
	{
		//clicked button should scale up and down
		setScale(1.0f);
		addTween(Actor::TweenScale(1.1f), 300, 1, true);
	}
	if (buttonSound == -1) {
		buttonSound = sfxPlayer.getSoundsNum();
		spSoundInstance step = sfxPlayer.play("menuSelect");
		step->setDoneCallback(CLOSURE(this, &MyButton::finishButtonSound));
	}
}

void MyButton::finishButtonSound(Event* ev) {
	buttonSound = -1;
}