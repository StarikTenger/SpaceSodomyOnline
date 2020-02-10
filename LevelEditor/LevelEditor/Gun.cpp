#include "Gun.h"



Gun::Gun()
{
}

Gun::Gun(double _cooldownTime, double _bulletVelocity) {
	cooldownTime = _cooldownTime;
	timeToCooldown = _cooldownTime;
	bulletVelocity = _bulletVelocity;
}


Gun::~Gun()
{
}
