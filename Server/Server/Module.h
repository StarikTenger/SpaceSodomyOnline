#pragma once
class Module {
public:
	enum Type {
		HP_UP,
		ENERGY_UP,
		CASCADE,
		IMPULSE
	};

	int type = 0;
	double timeToCoolDown = 0;
	Module();
	Module(int _type);
};

