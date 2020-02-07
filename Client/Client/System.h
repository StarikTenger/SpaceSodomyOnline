#pragma once
#include <vector>
#include <fstream>
#include <string>
#include <SFML/Graphics.hpp>
#include "Vec2.h"
#include "Object.h"
#include "Cell.h"
#include "Player.h"


class System{
public:
	int id = 0;
	int privilegies = 1;

	double bulletVel = 15;

	std::string state;
	std::vector<Object> objects;
	std::vector<std::vector<Cell> > field;
	std::vector<Player> players;

	System();
	System(std::string path);
	~System();
	int checkWall(Vec2 pos);
	void unpack(std::string str);
};