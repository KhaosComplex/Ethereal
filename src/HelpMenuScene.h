#pragma once
#include "Scene.h"

DECLARE_SMART(HelpMenuScene, spHelpMenuScene);
class HelpMenuScene : public Scene
{
public:
	static spHelpMenuScene instance;

	HelpMenuScene();

private:
	void onEvent(Event* ev);

	spActor _text;
};