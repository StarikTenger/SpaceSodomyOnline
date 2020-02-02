#pragma once
#include <vector>
#include <fstream>
#include <string>
#include <SFML/Graphics.hpp>
#include "Vec2.h"
#include "Object.h"
#include "Cell.h"


class System{
public:
	int id = 0;

	std::string state;
	std::vector<Object> objects;
	std::vector<std::vector<Cell> > field;

	System();
	System(std::string path);
	~System();
	void unpack(std::string str);
};