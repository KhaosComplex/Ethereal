#pragma once
#include "Scene.h"

DECLARE_SMART(AboutMenuScene, spAboutMenuScene);
class AboutMenuScene : public Scene
{
public:
	static spAboutMenuScene instance;

	AboutMenuScene();

private:
	void onEvent(Event* ev);

	spActor _text;
};