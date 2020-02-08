#pragma once
#include "Vec2.h"

class Bonus {
public:
	enum Type {
		HP,
		ENERGY
	};
	Vec2 pos;
	Type type = ENERGY;

	Bonus() {};
};

