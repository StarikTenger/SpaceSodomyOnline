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
		LASER
	};
	Vec2 pos;
	Type type = NONE;

	Bonus() {};
};

