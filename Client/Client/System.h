#pragma once
#include <vector>
#include <fstream>
#include <string>
#include <SFML/Graphics.hpp>
#include "Vec2.h"
#include "Object.h"
#include "Cell.h"
#include "Player.h"
#include "Bonus.h"


class System{
public:
	int id = 0;
	int privilegies = 0;
	int target = 0;

	double bulletVel = 15;
	double time = 0;
	double hpPrev = 0;

	Object mainPlayer;

	std::string state;

	std::vector<Player> players;

	std::vector<Object> objects;
	std::vector<Bonus> bonuses;
	
	std::vector<std::vector<Cell> > field;
	

	System();
	System(std::string path);
	~System();
	int checkWall(Vec2 pos);
	bool checkAbility(Object shooter, Object target, double threshold);
	void unpack(std::string str);
};