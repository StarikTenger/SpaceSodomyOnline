#pragma once
#include "Color.h"
#include "Object.h"
#include <string>

class Player {
public:
	std::string name = "a";
	int kills = 0;
	int deaths = 0;

	Object* object = nullptr;

	Color color;

	Player() {};
};

bool operator<(const Player& a, const Player& b);