#pragma once
#include "Vec2.h"

class Bonus {
public:
	enum Type {
		NONE,
		HP,
		ENERGY,
		BERSERK,
		IMMORTAL,
		BOOST,
		LASER,
		INVISIBILITY,
		MASS
	};
	Vec2 pos;
	Type type = ENERGY;

	Bonus() {};
};

