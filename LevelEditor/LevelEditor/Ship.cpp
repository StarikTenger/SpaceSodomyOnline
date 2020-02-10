#include "Ship.h"



Ship::Ship()
{
	collision = 1;
}

Ship::Ship(Engine _engine, Gun _gun) {
	collision = 1;
	engine = _engine;
	gun = _gun;
}

Ship::~Ship()
{
}
