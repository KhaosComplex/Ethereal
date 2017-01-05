#pragma once
#include "oxygine-framework.h"
using namespace oxygine;
using namespace std;

DECLARE_SMART(MyButton, spMyButton);
class MyButton : public Sprite
{
public:
	MyButton();

private:
	void onEvent(Event*);
	void finishButtonSound(Event* ev);

	spTextField _text;
	int buttonSound;
};