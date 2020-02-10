#pragma once
#include "Creature.h"
#include "Laser.h"
#include <vector>

class LaserCarrier : public Creature {
public:
	std::vector<Laser> lasers;
	//int directions = 1;
	LaserCarrier();
	LaserCarrier(int n);
	~LaserCarrier();
};

