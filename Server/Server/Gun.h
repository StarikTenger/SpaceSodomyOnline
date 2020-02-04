#pragma once
class Gun {
public:
	double cooldownTime = 1;
	double timeToCooldown = 0;
	double vel = 10; // velocity of bullet
	double lifetime = 10; // countdown to explosion
};

