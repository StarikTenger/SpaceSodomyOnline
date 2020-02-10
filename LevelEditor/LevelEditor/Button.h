#pragma once
#include <string>
#include "geometry.h"

class Button {
public:
	int type = 0;
	Vector2d pos;
	Vector2d box;
	std::string text;
	double value;
	double step = 0.1;
	Vector2d bound;

	Button();
	~Button();
};

