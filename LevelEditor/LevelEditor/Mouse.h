#pragma once
#include "Vector2d.h"

class Mouse{
public:
	Vector2d pos;
	Vector2d posPrev;
	double delta = 0;
	bool state = 0;
	bool statePrev = 0;
	Mouse();
	~Mouse();
};

