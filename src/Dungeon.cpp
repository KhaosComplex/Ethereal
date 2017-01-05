#include "Dungeon.h"
#include <random>
#include <algorithm>

Dungeon::Dungeon() : currentPosition(0), overallPosition(0), terminal(0), chest(0), started(false)
{
}

Dungeon::~Dungeon()
{
	for (int iter = 0; iter != fullDung.size(); ++iter) {
		delete fullDung.at(iter);
	}
	fullDung.erase(fullDung.begin(), fullDung.end());
}

void Dungeon::generateDungeon()
{
	std::random_device rand_dev;
	std::mt19937 generator(rand_dev());

	std::uniform_int_distribution<int> terminalDist(2, 6);
	terminal = terminalDist(generator);

	std::uniform_int_distribution<int> chestDist(1, 8);
	chest = chestDist(generator);

	while (chest == terminal) {
		chest = chestDist(generator);
	}

	std::uniform_int_distribution<int> terminalRooms(1, 10);
	dung[terminal] = terminalRooms(generator);
	usedRooms.push_back(dung[terminal]);

	std::uniform_int_distribution<int> chestRooms(1, 3);
	dung[chest] = chestRooms(generator);
	usedRooms.push_back(dung[chest]);

	std::uniform_int_distribution<int> bossRooms(1, 1);
	dung[9] = bossRooms(generator);
	usedRooms.push_back(dung[9]);

	std::uniform_int_distribution<int> roomDist(1, 10);
	int temp;
	for (int i = 0; i < 10; ++i) {
		if (i != terminal && i != chest && i != 9) {
			temp = roomDist(generator);
			while (std::find(usedRooms.begin(), usedRooms.end(), temp) != usedRooms.end()) {
				temp = roomDist(generator);
			}
			usedRooms.push_back(temp);
			dung[i] = temp;
		}
	}
}

void Dungeon::loadDungeonRoom()
{
	currentRoom = fullDung.at(currentPosition);
	currentPosition++;
	overallPosition++;
}

void Dungeon::reverseLoadDungeonRoom()
{
	currentPosition -= 2;
	overallPosition -= 2;

	currentRoom = fullDung.at(currentPosition-1);
	currentPosition++;
	overallPosition++;
	
}

void Dungeon::loadFullDungeon()
{
	int curPos = 0;
	for (int i = 0; i < 10; ++i) {
		Room* room = new Room;

		if (curPos == 0) {
			room->initLevel("../data/rooms/start/start_1.tmx", "../data/rooms/start/");
		}
		else if (curPos == terminal) {
			room->initLevel("../data/rooms/terminal/terminal_" + std::to_string(dung[curPos]) + ".tmx", "../data/rooms/terminal/");
		}
		else if (curPos == chest) {
			room->initLevel("../data/rooms/chest/chest_" + std::to_string(dung[curPos]) + ".tmx", "../data/rooms/chest/");
		}
		else if (curPos == 9) {
			room->initLevel("../data/rooms/boss/boss_" + std::to_string(dung[curPos]) + ".tmx", "../data/rooms/boss/");
		}
		else {
			room->initLevel("../data/rooms/regular/regular_" + std::to_string(dung[curPos]) + ".tmx", "../data/rooms/regular/");
		}

		fullDung.push_back(room);
		curPos++;
	}
}



Room * Dungeon::getCurrentRoom()
{
	return currentRoom;
}

std::vector<Room*> Dungeon::getFullDung()
{
	return fullDung;
}

std::vector<Room*>::iterator Dungeon::getFullDungBegin()
{
	return fullDung.begin();
}

std::vector<Room*>::iterator Dungeon::getFullDungEnd()
{
	return fullDung.end();
}

