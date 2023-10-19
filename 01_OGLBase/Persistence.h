#pragma once

#include <sstream>
#include <iostream>
#include <string>

class Player;

class Persistence
{
private:
	Player* m_player;
public:
	Persistence();
	Persistence(Player*);

	void Save();
	bool Load();
};
