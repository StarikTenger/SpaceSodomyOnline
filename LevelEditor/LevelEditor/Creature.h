#pragma once
#include "Unit.h"
#include "Orderlist.h"

class Creature : public Unit {
public:
	Orderlist orders;
	int shields = 0;
	double immortality = 0;
	double hp = 1;
	Creature();
	~Creature();
};

