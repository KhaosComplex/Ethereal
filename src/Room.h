#pragma once
#include "Tmx.h"
#include "oxygine-framework.h"



using namespace oxygine;
using namespace Tmx;

class Room
{
public:
	Room();
	~Room();

	void initLevel(std::string filename, std::string filepath);

	Map* currentMap;
	spSprite bg;
	std::vector<spSprite> tileSpriteList;
	std::vector<int> gidNums;
};