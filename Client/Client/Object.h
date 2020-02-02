#pragma once
#include "geometry.h"

class Object {
public:
	enum Type {
		SHIP,
		BULLET,
		EXPLOSION
	};

	int id = 0;
	Type type = SHIP;
	Vec2 pos;
	double r = 0.4;
	double dir;
	double hp;

	Object() {};
};

