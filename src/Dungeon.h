#pragma once
#include "Room.h"
#include <array>

class Dungeon
{
public:
	Dungeon();
	~Dungeon();
	void generateDungeon();
	void loadDungeonRoom();
	void reverseLoadDungeonRoom();
	void loadFullDungeon();
	Room* getCurrentRoom();
	std::vector<Room*> getFullDung();
	std::vector<Room*>::iterator getFullDungBegin();
	std::vector<Room*>::iterator getFullDungEnd();
private:
	std::array<int, 10> dung;
	std::vector<int> usedRooms;
	std::vector<Room*> fullDung;
	int currentPosition;
	int overallPosition;
	Room* currentRoom;

	int terminal;
	int chest;

	bool started;

};