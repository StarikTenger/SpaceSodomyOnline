#include "Turret.h"

Turret::Turret() {}
Turret::Turret(Gun _gun, double w) {
	collision = 1;
	gun = _gun;
	body.w = w;
	team = "enemy";
}
Turret::~Turret()
{}
