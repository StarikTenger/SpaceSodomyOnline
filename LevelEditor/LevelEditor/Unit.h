#pragma once
#include "Geometry.h"
#include "Body.h"
#include <string>


class Unit {
public:
	Body body;
	std::string team = "good";

	double maxHp = 100;
	double hp = maxHp;
	bool collision = 0;
	bool alive = 1;

	Unit();
	virtual ~Unit();
};