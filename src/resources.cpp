#include "resources.h"

namespace res
{
	Resources mm;
	Resources gm;
	Resources sound;

	void load()
	{
		mm.loadXML("mainmenu.xml");
		gm.loadXML("game.xml");
		sound.loadXML("sound.xml");
	}

	void free()
	{
		mm.free();
		gm.free();
		sound.free();
	}
}