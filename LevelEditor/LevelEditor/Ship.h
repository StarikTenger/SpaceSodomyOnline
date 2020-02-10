#pragma once
#include <vector>
#include <map>
#include "Shooter.h"
#include "Mover.h"
#include "Engine.h"
#include "Gun.h"
#include "Orderlist.h"

class Ship : public Creature, public Shooter, public Mover{
public:
	//Engine engine;

	Ship();
	Ship(Engine _engine, Gun _gun);
	~Ship();
};

