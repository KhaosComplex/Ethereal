#pragma once
#include "oxygine-framework.h"
using namespace oxygine;

namespace res
{
	//resources for the main menu
	extern Resources mm;
	//resources for the game
	extern Resources gm;
	//resources for the game sound
	extern Resources sound;
	void load();
	void free();
}