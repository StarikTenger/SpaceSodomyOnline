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

int System::checkWall(Vec2 pos) {
	int x = (int)(pos.x / 1);
	int y = (int)(pos.y / 1);
	Vec2 rel = pos - Vec2(x + 0.5, y + 0.5);
	if ((x < 0 || y < 0 || x >= field.size() || y >= field[0].size() || field[x][y].type == 1) ||
		rel.y < -rel.x && field[x][y].type == CORNER_A || rel.y > -rel.x && field[x][y].type == CORNER_C ||
		rel.y < rel.x && field[x][y].type == CORNER_B || rel.y > rel.x && field[x][y].type == CORNER_D
		) {
		return 1;
	}
	return 0;
}

bool System::checkAbility(Object shooter, Object target, double threshold) {
	if (distance(shooter.pos, target.pos) < EPS) {
		return 0;
	}
	bool contact = 1;

	double stepSize = 0.5;
	double a = abs(dir(shooter.vel - target.vel + direction(shooter.dir) * 15)
		- dir(target.pos - shooter.pos));
	Vec2 step = direction(dir(shooter.vel - target.vel + direction(shooter.dir) * 15))* stepSize;
	for (int i = 0; i < distance(shooter.pos, target.pos) / stepSize; i++) {
		if (checkWall(shooter.pos + step * i)) {
			contact = 0;
		}
	}

	if (!contact) {
		return 0;
	}



	while (a >= 2 * M_PI) {
		a -= 2 * M_PI;
	}
	while (a <= 0) {
		a += 2 * M_PI;
	}
	if (a < threshold) {
		return 1;
	}
	return 0;
}

void System::unpack(std::string str) {
	objects = {};
	playerList = {};
	players = {};
	bonuses = {};

	std::stringstream ss;
	ss << str;
	std::string type;
	
	while (ss >> type) {
		// END
		if (type == "/")
			break;

		// Players
		if (type == "P") {
			playerList.push_back({});
			auto& player = playerList.back();

			int id = 0;
			
			ss >> id;
			ss >> player.name;
			ss >> player.color.r >> player.color.g >> player.color.b;
			player.color.a = 255;
			ss >> player.kills;
			ss >> player.deaths;

			players[id] = player;
			//players.insert({ id, player });
		}

		// Bonuses
		if (type == "e" || type == "h" || type == "b" || type == "i" || type == "o") {
			bonuses.push_back({});
			auto& bonus = bonuses.back();
			int x, y;
			ss >> x >> y;
			bonus.pos.x = x + 0.5;
			bonus.pos.y = y + 0.5;
			if (type == "e")
				bonus.type = Bonus::ENERGY;
			if (type == "h")
				bonus.type = Bonus::HP;
			if (type == "b")
				bonus.type = Bonus::BERSERK;
			if (type == "i")
				bonus.type = Bonus::IMMORTAL;
			if (type == "o")
				bonus.type = Bonus::BOOST;
		}

		// Objects
		if (type == "S" || type == "B") {
			objects.push_back({});
			auto& object = objects.back();

			// id
			ss >> object.id;
			// position
			ss >> object.pos.x >> object.pos.y;
			// direction
			ss >> object.dir;
			
			// linear velocity
			ss >> object.vel.x >> object.vel.y;
			// angular velocity
			ss >> object.w;

			// color
			//ss >> object.color.r >> object.color.g >> object.color.b;
			object.color = players[object.id].color;
			object.color.a = 255;

			if (type == "B")
				objects.back().type = Object::BULLET;

			if (type == "S") {
				// hp
				ss >> object.hp >> object.hpMax;
				// energy
				ss >> object.energy >> object.energyMax;
				// orders
				{
					int orders;
					ss >> orders;
					int i = 0; objects.back().orders.size();
					while (orders > 0) {
						if (orders % 2 == 1)
							objects.back().orders[i] = 1;
						orders /= 2;
						i++;
					}
				}
				// effects
				{
					int effects;
					ss >> effects;
					int i = 0; objects.back().orders.size();
					while (effects > 0) {
						if (effects % 2 == 1)
							objects.back().effects[i] = 1;
						effects /= 2;
						i++;
					}
				}
				// active
				int abl;
				ss >> abl;
				object.activeAbility = Bonus::Type(abl);
			}
		}
	}

	// Sort playerList
	std::sort(playerList.begin(), playerList.end());
	std::reverse(playerList.begin(), playerList.end());

	// Relation object to player
	for (auto& object : objects) {
		if (object.type == Object::SHIP) {
			players[object.id].object = &object;
			// Find main player
			if(object.id == id)
				mainPlayer = object;
		}
	}
}
