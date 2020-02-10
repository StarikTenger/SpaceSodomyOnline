#pragma once
#include "Shooter.h"
#include "Gun.h"

class RocketLauncher : public Creature, public Shooter{
public:
	RocketLauncher();
	~RocketLauncher();
};

