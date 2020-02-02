#pragma once
#include "Vec2.h"

class Mouse{
public:
	Vec2 pos;
	bool state = 0;
	bool statePrev = 0;
	Mouse();
	~Mouse();
};

