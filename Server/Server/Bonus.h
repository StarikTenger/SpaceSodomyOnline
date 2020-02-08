#pragma once
#include "Vec2.h"

class Bonus {
public:
	enum Type {
		NONE,
		HP,
		ENERGY
	};
	Vec2 pos;
	Type type = ENERGY;

	Bonus() {};
};

