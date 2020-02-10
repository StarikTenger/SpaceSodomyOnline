#pragma once
#include "geometry.h"

class Laser {
public:
	Vector2d base;
	Vector2d end;
	double direction = 0;
	Laser();
	~Laser();
};

