#pragma once
#include <vector>
#include <string>
#include "Cell.h"
#include "Color.h"
#include "Unit.h"
#include "Body.h"
#include "Bonus.h"
#include "Creature.h"
#include "Exit.h"
#include "LaserCarrier.h"
#include "Robot.h"
#include "Ship.h"
#include "Turret.h"
#include "RocketLauncher.h"
#include "Generator.h"




class System {
public:
	std::vector<std::vector<Cell> > field;
	std::vector<Unit*> units;
	double blockSize = 1;

	std::vector<int> colorsActive = { 0, 0, 0, 0, 0, 0, 0 };
	std::vector<Color> colorMatches = {
		Color(0, 151, 255),
		Color(50, 255, 81),
		Color(255, 238, 61),
		Color(200, 61, 255),
		Color(255, 61, 141),
		Color(255, 158, 61)
	};

	System();
	System(std::string path);
	~System();
	void save(std::string path);
	void fill(int x, int y, int allow);
};
