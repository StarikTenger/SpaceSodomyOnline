#pragma once
#include <SFML/Graphics.hpp>
#include "geometry.h"

class Gamepad {
public:
	int button1 = 0;
	int button2 = 0;
	int button3 = 0;
	int button4 = 0;
	int leftUp = 0;
	int leftDown = 0;
	int rightUp = 0;
	int rightDown = 0;
	int button9 = 0;
	int button10 = 0;
	Vec2 pos;
	Gamepad();
	~Gamepad();
	int& get(int button);
};

