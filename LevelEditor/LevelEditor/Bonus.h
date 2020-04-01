#pragma once
#include "Unit.h"

class Bonus :public Unit {
public:
	enum Type {
		NONE,
		HP,
		ENERGY,
		BERSERK,
		IMMORTAL,
		BOOST
	};
	Type type = NONE;
	Bonus();
	~Bonus();
};

