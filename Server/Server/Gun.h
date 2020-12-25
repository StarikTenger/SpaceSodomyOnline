#pragma once

class Gun {
public:
	double cooldownTime = 0.5;
	double timeToCooldown = 0;
	double damage = 1;
	double vel = 15; // velocity of bullet
	double force = 0;
	double lifetime = 10; // countdown to explosion
	double energy = 0.3; // energy points required
	double stamina = 2.5; // stamina points required
};

