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
		if (command == "FORCEFIELD") {
			for (int x = 0; x < width; x++) {
				for (int y = 0; y < height; y++) {
					int dir;
					file >> dir;
					if(dir)
						field[y][x].forceField = geom::direction((dir - 1) * M_PI/4) * 16;
				}
			}
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
			if (type == "laser")
				bonusInfo[Bonus::LASER].positions.push_back(pos);
		}
	}
}

System::~System() {
}

std::map<std::string, int> System::bonusNames = {
		{"HP", Bonus::HP},
		{"ENERGY", Bonus::BERSERK},
		{"BERSERK", Bonus::BERSERK},
		{"IMMORTAL", Bonus::IMMORTAL},
		{"BOOST", Bonus::BOOST},
		{"LASER", Bonus::LASER}
};

void System::setPlayer(Object object) {

	objects.push_back(object);
	if (players.find(object.id) == players.end())
		players[object.id] = Player();
	
	players[object.id].team = object.team;
	if (teams.find(object.team) == teams.end()) {
		teams[object.team] = Team();
		teams[object.team].color = fromHSV(random::intRandom(0, 360), 1, 1);
	}
	object.color = players[object.id].color = teams[object.team].color;
	
	auto& player = objects.back();
	while (1) {
		int x = random::intRandom(1, field.size() - 1);
		int y = random::intRandom(1, field.size() - 1);
		player.pos = Vec2(x + 0.5, y + 0.5);
		if (teams.find(player.team) != teams.end() && teams[player.team].spawnpoints.size())
			player.pos = teams[player.team].spawnpoints[random::intRandom(0, teams[player.team].spawnpoints.size() - 1)] + Vec2(0.5, 0.5);
		else if (field[x][y].type)
			continue;

		break;
	}
}

std::string System::pack() {
	std::string packet = "";

	// Players
	for (const auto& player : players) {
		if (player.second.afkTimer < 0)
			continue;
		packet += "P ";
		packet += to_string(player.first) + " ";
		packet += player.second.name + " ";
		packet += to_string(player.second.color.r) + " ";
		packet += to_string(player.second.color.g) + " ";
		packet += to_string(player.second.color.b) + " ";
		packet += to_string(player.second.kills) + " ";
		packet += to_string(player.second.deaths) + " ";
	}
	// wall player
	{
		packet += "P ";
		packet += "0 ";
		packet += " WALL ";
		packet += "255 255 255 ";
		packet += to_string(wallKills) + " ";
		packet += to_string(0) + " ";
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
		case Bonus::LASER:
			packet += "l ";
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
		// dir
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

void System::damage(Object& object, Object& target, double value) {
	if (object.team == target.team || target.effects[Bonus::IMMORTAL] > 0)
		return;
	target.hp -= value;
	if (target.hp < EPS && target.type == Object::SHIP) {
		players[object.id].kills++;
		players[target.id].deaths++;

		if (players[object.id].object) {
			players[object.id].object->energy += 5;
			players[object.id].object->energyRecovery *= 1.1;
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

	Vec2 step = direction(shooter.dir) * stepSize;
	for (int i = 0; i < distance(shooter.pos, target.pos) / stepSize; i++) {
		if (checkWall(shooter.pos + step * i)) {
			contact = 0;
		}
	}

	if (!contact) {
		return 0;
	}


	double a = abs(shooter.dir - dir(target.pos - shooter.pos));

	while (a >= 2 * M_PI) {
		a -= 2 * M_PI;
	}
	while (a <= 0) {
		a += 2 * M_PI;
	}

	if (distance(target.pos, shooter.pos, shooter.pos + step) < target.r && a < threshold)
		return 1;
	return 0;
}
