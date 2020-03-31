#pragma once
#include <vector>
#include <fstream>
#include "Vec2.h"
#include <string>
#include <SFML/Graphics.hpp>
#include "Object.h"
#include "Cell.h"
#include "Player.h"
#include "Bonus.h"
#include "BonusInfo.h"
#include "Team.h"


class System {
public:

	double dt = 0.02;
	double bounce = 0.2;

	std::vector<BonusInfo> bonusInfo = { 
		{},
		{ 6, 5 },
		{ 6, 5 },
		{ 1, 40 },
		{ 1, 40 }
	};



	Vec2 border = {50, 50};

	std::map<int, Player> players;
	std::map<int, Team> teams;

	std::vector<Object> objects;
	std::vector<Object> objectsToAdd;
	std::vector<Bonus> bonuses;

	std::vector<std::vector<Cell> > field;

	System();
	System(std::string path);
	~System();

	std::string pack();
	void step();
	void setPlayer(Object object);
	void shoot(Object& object);
	bool collision(Object& body, std::pair<Vec2, Vec2> wall);
	void collision();
};