#include "System.h"
#include "geometry.h"
#include "random.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace geom;
using namespace random;
using namespace std;

System::System(){
	
}

System::~System() {
}

System::System(string path) {
	ifstream file(path);
	int width, height;
	file >> width >> height;
	for (int x = 0; x < width; x++) {
		field.push_back(vector<Cell>(height));
	}
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			Cell c;
			std::string type;
			file >> type;
			if (type == "0") {
				c.type = 0;
			}
			else if (type == "0") {
				c.type = EMPTY;
			}
			else if (type == "1") {
				c.type = WALL;
			}
			else if (type == "2") {
				c.type = WALL;
				c.spikes = 1;
			}
			else if (type == "A" || type == "a") {
				c.type = CORNER_A;
			}
			else if (type == "B" || type == "b") {
				c.type = CORNER_B;
			}
			else if (type == "C" || type == "c") {
				c.type = CORNER_C;
			}
			else if (type == "D" || type == "d") {
				c.type = CORNER_D;
			}
			field[y][x] = c;
		}
	}
}

void System::unpack(std::string str) {
	objects = {};
	std::stringstream ss;
	ss << str;
	std::string type;
	while (ss >> type) {
		if (type == "S") {
			int id;
			ss >> id;
			double x, y, dir;
			ss >> x >> y >> dir;
			objects.push_back({});
			objects.back().id = id;
			objects.back().pos = { x,y };
			objects.back().dir = dir;
		}
	}
}
