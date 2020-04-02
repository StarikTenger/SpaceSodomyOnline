#pragma once
#include "Color.h"
#include <string>

class Player {
public:
	int alive = 1;
	int kills = 0;
	int deaths = 0;
	int team = 0;

	double timer = 0;

	std::string name = "???";

	Color color;

	Player() {};
};

