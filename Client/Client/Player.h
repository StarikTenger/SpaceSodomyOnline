#pragma once
#include "Color.h"

class Player {
public:
	int id = 0;
	int kills = 0;
	int deaths = 0;

	Color color;

	Player() {};
};