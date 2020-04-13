#include "System.h"
#include "geometry.h"

#include <iostream>
#include <fstream>

void System::save(std::string path) {
	std::ofstream file(path);
	file << field.size() << " " << field[0].size() << "\n";
	for (int y = 0; y < field[0].size(); y++) {
		for (int x = 0; x < field.size(); x++) {
			if (field[x][y].spikes) {
				file << "2";
			}
			else {
				switch (field[x][y].type) {
				case WALL:
					file << "1";
					break;
				case CORNER_A:
					file << "a";
					break;
				case CORNER_B:
					file << "b";
					break;
				case CORNER_C:
					file << "c";
					break;
				case CORNER_D:
					file << "d";
					break;
				default:
					file << "0";
					break;
				}
			}
			file << " ";
		}
		file << "\n";
	}
	file << "FORCEFIELD \n";
	for (int y = 0; y < field[0].size(); y++) {
		for (int x = 0; x < field.size(); x++) {
			file << field[x][y].forceField << " ";
		}
		file << "\n";
	}
	for (Unit* unit : units) {
		{//EXIT
			Exit* exit;
			if (exit = dynamic_cast<Exit*>(unit)) {
				file << "EXIT " << (int)exit->body.pos.x << " " << (int)exit->body.pos.y << " \n";
				continue;
			}
		}
		{//BONUS
			Bonus* bonus;
			if (bonus = dynamic_cast<Bonus*>(unit)) {
				std::string type = "energy";
				if(bonus->type == Bonus::ENERGY)
					type = "energy";
				if (bonus->type == Bonus::HP)
					type = "hp";
				if (bonus->type == Bonus::BERSERK)
					type = "berserk";
				if (bonus->type == Bonus::IMMORTAL)
					type = "immortal";
				if (bonus->type == Bonus::BOOST)
					type = "boost";
				if (bonus->type == Bonus::LASER)
					type = "laser";
				file << "BONUS " << (int)bonus->body.pos.x << " " << (int)bonus->body.pos.y << " " << type <<  "\n";
				continue;
			}
		}
		
		{//TURRET
			Turret* turret;
			if (turret = dynamic_cast<Turret*>(unit)) {
				file << "TURRET ";
				file << "CD " << turret->gun.cooldownTime << " ";
				file << "BV " << turret->gun.bulletVelocity << " ";
			}
		}
		{//ROCKET_LAUNCHER
			RocketLauncher* rocketLauncher;
			if (rocketLauncher = dynamic_cast<RocketLauncher*>(unit)) {
				file << "ROCKET_LAUNCHER ";
				file << "CD " << rocketLauncher->gun.cooldownTime << " ";
				file << "BV " << rocketLauncher->gun.bulletVelocity << " ";
				file << "DIRECTIONS " << (int)rocketLauncher->gun.directions << " ";
			}
		}
		{//LASER_CARRIER
			LaserCarrier* laserCarrier;
			if (laserCarrier = dynamic_cast<LaserCarrier*>(unit)) {
				file << "LASER_CARRIER ";
				file << "DIRECTIONS " << (int)laserCarrier->lasers.size() << " ";
			}
		}
		{//ROBOT
			Robot* robot;
			if (robot = dynamic_cast<Robot*>(unit)) {
				file << "ROBOT ";
				file << "CD " << robot->gun.cooldownTime << " ";
				file << "BV " << robot->gun.bulletVelocity << " ";
				file << "MF " << robot->engine.mainForce << " ";
				file << "SHIELDS " << 0 << " ";
			}
		}
		{//GENERATOR
			Generator* generator;
			if (generator = dynamic_cast<Generator*>(unit)) {
				file << "GENERATOR ";
				file << "COLOR " << generator->color << " ";
			}
		}


		file << "POS " << (int)unit->body.pos.x << " " << (int)unit->body.pos.y << " ";
		file << "DIR " << unit->body.direction << " ";
		file << "W " << unit->body.w << " ";
		file << "END\n";
	}
	file << "END";
	file.close();
}