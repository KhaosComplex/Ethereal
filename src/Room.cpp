#include "Room.h"

Room::Room() : currentMap(new Map)
{

}

Room::~Room()
{
	for (int iter = 0; iter != tileSpriteList.size(); ++iter) {
		delete tileSpriteList.at(iter)->getResAnim();
	}
	tileSpriteList.erase(tileSpriteList.begin(), tileSpriteList.end());
	gidNums.erase(gidNums.begin(), gidNums.end());
	delete currentMap;
}

void Room::initLevel(std::string filename, std::string filepath) {
	//Load the current map from the TMX file
	currentMap->ParseFile(filename);

	for (int iter = 0; iter != currentMap->GetNumTilesets(); iter++) {
		ResAnim* resAnim = new ResAnim;
		const Tileset *tileset = currentMap->GetTileset(iter);
		
		resAnim->init(filepath + tileset->GetImage()->GetSource(),
			tileset->GetImage()->GetWidth() / tileset->GetTileWidth(),
			tileset->GetImage()->GetHeight() / tileset->GetTileHeight(),
			1.0f);
		spSprite tile = new Sprite;
		tile->setResAnim(resAnim);
		
		tileSpriteList.push_back(tile);

		gidNums.push_back(tileset->GetFirstGid());
	}

	bg = new Sprite;
	ResAnim* resAnim = new ResAnim;
	resAnim->init("../data/rooms/regular" + currentMap->GetImageLayer(0)->GetImage()->GetSource());
	bg->setResAnim(resAnim);
	tileSpriteList.push_back(bg);

}