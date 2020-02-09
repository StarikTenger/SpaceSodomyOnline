#pragma once
#include "Color.h"
#include <string>

class Player {
public:
	std::string name = "a";
	int kills = 0;
	int deaths = 0;

	Color color;

	Player() {};
};

bool operator<(const Player& a, const Player& b);