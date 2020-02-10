#pragma once
#include "geometry.h"

class Gun {
public:
	double cooldownTime = 0;
	double timeToCooldown = 0;
	double bulletVelocity = 0;
	int directions = 1;
	double divergenceAngle = M_PI*2;
	Gun();
	Gun(double _cooldownTime, double _bulletVelocity);
	~Gun();
};

