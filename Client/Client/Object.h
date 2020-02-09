#pragma once
#include "geometry.h"
#include "color.h"

class Object {
public:
	enum Type {
		SHIP,
		BULLET,
		EXPLOSION
	};
	enum Orders {
		MOVE_LEFT,
		MOVE_RIGHT,
		MOVE_FORWARD,
		MOVE_BACKWARD,
		TURN_LEFT,
		TURN_RIGHT,
		SHOOT,
		STABILIZE_ROTATION
	};

	int id = 0;
	Type type = SHIP;
	Color color;

	bool berserk = 0;
	bool immortal = 0;

	Vec2 pos;
	Vec2 vel;
	double r = 0.4;
	double dir;
	double w;
	double hp;
	double hpMax;
	double energyMax;
	double energy;

	std::vector<bool> orders = { 0,0,0,0,0,0,0,0,0,0 };

	Object() {};
};

