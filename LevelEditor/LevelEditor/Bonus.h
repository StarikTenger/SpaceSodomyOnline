#pragma once
#include "Unit.h"

class Bonus :public Unit {
public:
	enum Type {
		NONE,
		HP,
		ENERGY,
		BERSERK,
		IMMORTAL
	};
	Type type = NONE;
	Bonus();
	~Bonus();
};

