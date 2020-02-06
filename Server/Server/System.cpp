#include "System.h"
#include "geometry.h"
#include "random.h"

#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <sstream>

using namespace geom;
using namespace random;
using namespace std;


template <typename T>
std::string to_string(const T a_value, int n) {
	std::ostringstream out;
	out.precision(n);
	out << std::fixed << a_value;
	return out.str();
}

System::System(){
	objects.push_back({ {0, 0}, 1, 0 });
	objects.push_back({ {200, 100}, 1, 0 });
	objects.back().id = 10;
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

System::~System() {
}

std::string System::pack() {
	std::string str = "";
	for (const auto& object : objects) {
		if (object.type == Object::SHIP) {
			str += "S ";
			// id
			str += to_string(object.id) + " ";
			// position
			str += to_string(object.pos.x, 3) + " ";
			str += to_string(object.pos.y, 3) + " ";
			// direction
			str += to_string(object.dir, 4) + " ";
			// color
			str += to_string((int)object.color.r) + " ";
			str += to_string((int)object.color.g) + " ";
			str += to_string((int)object.color.b) + " ";
			// hp
			str += to_string(object.hp, 1) + " ";
			str += to_string(object.hpMax, 1) + " ";
			// energy
			str += to_string(object.energy, 1) + " ";
			str += to_string(object.energyMax, 1) + " ";
			// packing orders
			int orders = 0;
			for (int i = 0; i < object.orders.size(); i++) {
				if (object.orders[i])
					orders += pow(2, i);
			}
			str += to_string(orders) + " ";
		}
		if (object.type == Object::BULLET) {
			str += "B ";
			// id
			str += to_string(object.id) + " ";
			// position
			str += to_string(object.pos.x, 3) + " ";
			str += to_string(object.pos.y, 3) + " ";
			// direction
			str += to_string(object.dir, 4) + " ";
			// color
			str += to_string((int)object.color.r) + " ";
			str += to_string((int)object.color.g) + " ";
			str += to_string((int)object.color.b) + " ";
		}
	}
	return str;
}

void System::shoot(Object& object) {
	if (object.gun.timeToCooldown > 0 || object.energy < object.gun.consumption)
		return;
	std::cout << object.gun.timeToCooldown << "\n";
	object.gun.timeToCooldown = object.gun.cooldownTime;
	object.energy -= object.gun.consumption;

	Object bullet;
	bullet.type = Object::BULLET;
	bullet.id = object.id;
	bullet.color = object.color;
	bullet.r = 0.2;
	bullet.dir = object.dir;
	bullet.pos = object.pos;
	bullet.vel = object.vel + geom::direction(object.dir) * object.gun.vel;
	bullet.hp = object.gun.lifetime;
	objectsToAdd.push_back(bullet);
}
