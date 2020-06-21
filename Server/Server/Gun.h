#pragma once
class Gun {
public:
	double cooldownTime = 0.5;
	double timeToCooldown = 0;
	double damage = 1;
	double vel = 15; // velocity of bullet
	double force = 0;
	double lifetime = 10; // countdown to explosion
	double consumption = 1; // energy points required
};

