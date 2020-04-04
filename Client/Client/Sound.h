#pragma once
#include <string>
#include "Vec2.h"

class Sound {
public:
	std::string name;
	Vec2 pos;
	Sound() {};
	Sound(std::string _name, Vec2 _pos);
};

