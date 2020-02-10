#pragma once
#include "Shooter.h"
#include "Gun.h"

class Turret : public Creature, public Shooter {
public:
	double anger = 0;
	double angerThreshold = 0.5;
	double activeRadius = 10;
	Turret();
	Turret(Gun _gun, double w);
	~Turret();
};

