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

System::System() {

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
		if (command == "BONUS") {
			Vec2 pos;
			std::string type;
			file >> pos.x >> pos.y;
			file >> type;
			if (type == "energy")
				bonusInfo[Bonus::ENERGY].positions.push_back(pos);
			if (type == "hp")
				bonusInfo[Bonus::HP].positions.push_back(pos);
			if (type == "berserk")
				bonusInfo[Bonus::BERSERK].positions.push_back(pos);
			if (type == "immortal")
				bonusInfo[Bonus::IMMORTAL].positions.push_back(pos);
			if (type == "boost")
				bonusInfo[Bonus::BOOST].positions.push_back(pos);
		}
	}
}

System::~System() {
}

void System::setPlayer(Object object) {
	objects.push_back(object);
	if (players.find(object.id) == players.end())
		players[object.id] = Player();
	players[object.id].team = object.team;
	players[object.id].color = object.color;
}

std::string System::pack() {
	std::string packet = "";

	// Players
	for (const auto& player : players) {
		packet += "P ";
		packet += to_string(player.first) + " ";
		packet += player.second.name + " ";
		packet += to_string(player.second.color.r) + " ";
		packet += to_string(player.second.color.g) + " ";
		packet += to_string(player.second.color.b) + " ";
		packet += to_string(player.second.kills) + " ";
		packet += to_string(player.second.deaths) + " ";
	}

	// Bonuses
	for (const auto& bonus : bonuses) {
		if (bonus.type == Bonus::NONE)
			continue;

		switch (bonus.type) {
		case Bonus::ENERGY:
			packet += "e ";
			break;
		case Bonus::HP:
			packet += "h ";
			break;
		case Bonus::BERSERK:
			packet += "b ";
			break;
		case Bonus::IMMORTAL:
			packet += "i ";
			break;
		case Bonus::BOOST:
			packet += "o ";
			break;
		}

		packet += std::to_string((int)bonus.pos.x) + " ";
		packet += std::to_string((int)bonus.pos.y) + " ";
	}

	// Objects
	for (const auto& object : objects) {
		std::string str = "";
		// id
		str += to_string(object.id) + " ";
		// position
		str += to_string(object.pos.x, 3) + " ";
		str += to_string(object.pos.y, 3) + " ";
		// direction
		str += to_string(object.dir, 4) + " ";
		// linear velocity
		str += to_string(object.vel.x, 2) + " ";
		str += to_string(object.vel.y, 2) + " ";
		// angular velocity
		str += to_string(object.w, 3) + " ";

		if (object.type == Object::SHIP) {
			str = "S " + str;

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

			// packing effects
			int effects = 0;
			for (int i = 0; i < object.effects.size(); i++) {
				if (object.effects[i] > 0)
					effects += pow(2, i);
			}
			//std::cout << effects << "\n";
			str += to_string(effects) + " ";

			// actives
			str += to_string(object.activeAbility) + " ";

		}
		if (object.type == Object::BULLET) {
			str = "B " + str;
		}

		packet += str;
	}

	packet += "/";

	return packet;
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
	bullet.team = object.team;
	bullet.color = object.color;
	bullet.r = 0.4;
	bullet.dir = object.dir;
	bullet.pos = object.pos;
	bullet.damage = object.gun.damage;
	bullet.vel = object.vel + geom::direction(object.dir) * object.gun.vel;
	bullet.hp = object.gun.lifetime;
	objectsToAdd.push_back(bullet);
}
