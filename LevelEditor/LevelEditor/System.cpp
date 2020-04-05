#include "System.h"
#include "geometry.h"

#include <iostream>
#include <fstream>


using namespace std;
System::System() {
	for (int x = 0; x < 50; x++) {
		field.push_back({});
		for (int y = 0; y < 50; y++) {
			field[x].push_back({});
			field[x].back().type = EMPTY;
		}
	}
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
	while (file) {
		string command;
		file >> command;
		if (command == "END") {
			break;
		}
		if (command == "SPAWNPOINT") {
			std::cout << "spawn\n";
			Ship* ship = new Ship(
				Engine(4, 4, 4),
				Gun(0.5, 15));
			file >> ship->body.pos.x >> ship->body.pos.y;
			ship->body.pos.x += 0.5 * blockSize;
			ship->body.pos.y += 0.5 * blockSize;
			ship->body.r = 0.24;
			ship->shields = 1;
			units.push_back(ship);
		}
		if (command == "BONUS") {
			Bonus* bonus = new Bonus();
			file >> bonus->body.pos.x >> bonus->body.pos.y;
			bonus->body.pos.x += 0.5 * blockSize;
			bonus->body.pos.y += 0.5 * blockSize;
			std::string type;
			file >> type;
			if (type == "energy")
				bonus->type = Bonus::ENERGY;
			if (type == "hp")
				bonus->type = Bonus::HP;
			if (type == "berserk")
				bonus->type = Bonus::BERSERK;
			if (type == "immortal")
				bonus->type = Bonus::IMMORTAL;
			if (type == "boost")
				bonus->type = Bonus::BOOST;
			if (type == "laser")
				bonus->type = Bonus::LASER;
			units.push_back(bonus);
		}
	}
}

System::~System()
{
}
