#pragma once
class Module {
public:
	enum Type {
		HP_UP,
		ENERGY_UP,
		CASCADE,
		IMPULSE,
		ROCKET,
		SPLASH,
		IMMORTALITY,
		BLINK,
		INVISIBILITY,
		MASS,
		HOOK
	};

	int type = 0;
	double timeToCooldown = 0;
	Module();
	Module(int _type);
};

