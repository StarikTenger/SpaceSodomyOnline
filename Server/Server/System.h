#pragma once
#include <vector>
#include <fstream>
#include "Vec2.h"
#include <string>
#include <SFML/Graphics.hpp>
#include "Object.h"
#include "Cell.h"


class System {
public:
	double dt = 0.02;
	double bounce = 0.2;

	Vec2 border = {50, 50};

	std::vector<Object> objects;
	std::vector<std::vector<Cell> > field;

	System();
	System(std::string path);
	~System();

	std::string pack();
	void step();
	bool collision(Object& body, std::pair<Vec2, Vec2> wall);
	void collision();
};