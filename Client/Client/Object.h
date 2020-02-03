#pragma once
#include "geometry.h"

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
	Vec2 pos;
	double r = 0.4;
	double dir;
	double hp;

	std::vector<bool> orders = { 0,0,0,0,0,0,0,0,0,0 };

	Object() {};
};

