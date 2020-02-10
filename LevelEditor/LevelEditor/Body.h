#pragma once
#include "Vector2d.h"


class Body{
public:
	Vector2d pos;
	Vector2d posPrev;
	Vector2d vel;
	Vector2d velPrev;
	double w = 0;
	double direction = 0;
	double m = 1;
	double r = 0.3;
	double wetFrictionK = 1;
	bool collision = 0;

	Body();
	Body(Vector2d _pos, Vector2d _vel, double _m);
	~Body();
};

