#pragma once
#include <vector>
#include <map>
#include "Shooter.h"
#include "Mover.h"
#include "Engine.h"
#include "Gun.h"
#include "Ship.h"
#include "Orderlist.h"

class Robot : public Ship {
public:
	double fear = 0;
	int characteristic = 0;
	Robot();
	~Robot();
};

