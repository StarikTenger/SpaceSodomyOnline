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
#include "ModuleInfo.h"
#include "Team.h"


class System {
public:

	double dt = 0.02;
	double bounce = 0.2;

	// bonuses
	std::vector<BonusInfo> bonusInfo = { 
		{},
		{ 6, 5 }, // HP
		{ 6, 5 }, // ENERGY
		{ 1, 40 }, // BERSERK
		{ 1, 40 }, // IMMORTAL
		{ 2, 30 }, // BOOST
		{ 1, 20 } // LASER
	};
	static std::map<std::string, int> bonusNames;

	// Modules - period, energy, stamina
	std::vector<ModuleInfo> moduleInfo = {
		{10, 3, 3}, // 0 hp up
		{6, 0, 3}, // 1 energy up
		{5, 1.5, 3}, // 2 cascade
		{5, 1.5, 3}, // 3 impulse
		{5, 1.5, 3}, // 4 rocket
		{4, 1.5, 3}, // 5 splash
		{8, 2, 3}, // 6 immortality
		{15, 3, 3}, // 7 blink
		{20, 3, 3}, // 8 invis
		{5, 2, 3}, // 9 mass
	};
	static std::map<std::string, int> moduleNames;

	Vec2 border = {50, 50};

	std::map<int, Player> players;
	std::map<int, Team> teams;
	int wallKills = 0;

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
	void shoot(Object& object, Vec2 shift, int type, double dir, int skip);
	void shoot(Object& object);
	void setExplosion(Object& object, Vec2 pos, Vec2 vel, double r, double power, double t, double dmg);
	void damage(Object& object, Object& target, double value);
	void explode(Object& object, Vec2 pos, double r, double angle, double power, double dmg, double backForce);
	int checkWall(Vec2 pos);
	bool checkAbility(Object shooter, Object target, double threshold);
	bool collision(Object& body, std::pair<Vec2, Vec2> wall);
	void collision();
};

