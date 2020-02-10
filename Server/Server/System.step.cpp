#include "geometry.h"
#include "System.h"
#include "random.h"
#include <iostream>
#include <math.h>

void System::step() {
	// Counters
	bonusEnergy.countdown -= dt;
	bonusHp.countdown -= dt;
	bonusBerserk.countdown -= dt;
	bonusImmortal.countdown -= dt;
	for (auto& object : objects) {
		if (object.type == Object::SHIP) {
			double k = 1;
			if (object.effects.berserk > 0) {
				k = 3;
				object.energy = object.energyMax;
			}
			object.gun.timeToCooldown -= dt * k;
			object.energy += object.energyRecovery * dt;
			if (object.energy > object.energyMax)
				object.energy = object.energyMax;

			object.effects.berserk -= dt;
			object.effects.immortal -= dt;
		}
		if (object.type == Object::BULLET)
			object.hp -= dt;
	}

	//OBJECTS//////////////////////////////////////////////////////////////////////////

	// Orders
	for (auto& object : objects) {
		if (object.type != Object::SHIP) // Orders are for ships only
			continue;

		// stabilize
		if (object.orders[Object::STABILIZE_ROTATION] && 
			!object.orders[Object::TURN_LEFT] && 
			!object.orders[Object::TURN_RIGHT])
			if (object.w > EPS)
				object.orders[Object::TURN_LEFT] = 1;
			else if (object.w < -EPS)
				object.orders[Object::TURN_RIGHT] = 1;

		if (object.energy > EPS) {
			// linear
			if (object.orders[Object::MOVE_FORWARD]) {
				object.vel += geom::rotate(Vec2(object.engine.linearForce, 0), object.dir) * dt;
				object.energy -= object.engine.consumptionLinear * dt;
			}

			if (object.orders[Object::MOVE_RIGHT]) {
				object.vel += geom::rotate(Vec2(object.engine.linearForce, 0), object.dir + M_PI * 0.5) * dt;
				object.energy -= object.engine.consumptionLinear * dt;
			}

			if (object.orders[Object::MOVE_BACKWARD]) {
				object.vel += geom::rotate(Vec2(object.engine.linearForce, 0), object.dir + M_PI) * dt;
				object.energy -= object.engine.consumptionLinear * dt;
			}

			if (object.orders[Object::MOVE_LEFT]) {
				object.vel += geom::rotate(Vec2(object.engine.linearForce, 0), object.dir + M_PI * 1.5) * dt;
				object.energy -= object.engine.consumptionLinear * dt;
			}

			// angular
			if (object.orders[Object::TURN_RIGHT]) {
				object.w += object.engine.angularForce * dt;
				object.energy -= object.engine.consumptionAngular * dt;
			}

			if (object.orders[Object::TURN_LEFT]) {
				object.w -= object.engine.angularForce * dt;
				object.energy -= object.engine.consumptionAngular * dt;
			}
		}
		else {
			object.orders[Object::MOVE_FORWARD] = 0;
			object.orders[Object::MOVE_RIGHT] = 0;
			object.orders[Object::MOVE_BACKWARD] = 0;
			object.orders[Object::MOVE_LEFT] = 0;
			object.orders[Object::TURN_RIGHT] = 0;
			object.orders[Object::TURN_LEFT] = 0;
		}

		// shoot
		if (object.orders[Object::SHOOT])
			shoot(object);
		
	}

	// Collison
	collision();

	// Add new objects
	for (auto& object : objectsToAdd) {
		objects.push_back(object);
	}
	objectsToAdd = {};

	// Delete objects
	for (int i = 0; i < objects.size(); i++) {
		if (objects[i].hp < EPS) {
			if (objects[i].type == Object::SHIP) {
				players[objects[i].id].alive = 0;
			}
			objects.erase(objects.begin() + i);
			i--;
		}
	}

	// Respawn
	for (auto& player : players) {
		if (!player.second.alive) {
			while (1) {
				int x = random::intRandom(1, field.size() - 1);
				int y = random::intRandom(1, field.size() - 1);
				if (field[x][y].type)
					continue;
				player.second.deaths++;
				setPlayer({ player.first, player.second.color, Vec2(x + 0.5, y + 0.5), 1, 0 });
				player.second.alive = 1;
				break;
			}
		}
	}

	// Movement
	for (auto& object : objects) {
		object.pos += object.vel * dt;
		object.dir += object.w * dt;
	}

	//BONUSES//////////////////////////////////////////////////////////////////////////

	// Bonuses collect
	for (auto& object : objects) {
		if (object.type != Object::SHIP)
			continue;

		for (auto& bonus : bonuses) {
			if (geom::distance(object.pos, bonus.pos) >= object.r * 2)
				continue;

			if (bonus.type == Bonus::ENERGY) {
				bonus.type = Bonus::NONE;
				object.energy += 5;
			}
			if (bonus.type == Bonus::HP && object.hp < object.hpMax - EPS) {
				bonus.type = Bonus::NONE;
				object.hp += 1;
			}
			if (bonus.type == Bonus::BERSERK) {
				bonus.type = Bonus::NONE;
				object.effects.berserk = 10;
			}
			if (bonus.type == Bonus::IMMORTAL) {
				bonus.type = Bonus::NONE;
				object.effects.immortal = 10;
			}
		}
	}

	// Bonuses calculating
	bonusEnergy.number = 0;
	bonusHp.number = 0;
	bonusBerserk.number = 0;
	bonusImmortal.number = 0;
	for (auto& bonus : bonuses) {
		switch (bonus.type) {
		case Bonus::ENERGY:
			bonusEnergy.number++;
			break;
		case Bonus::HP:
			bonusHp.number++;
			break;
		case Bonus::BERSERK:
			bonusBerserk.number++;
			break;
		case Bonus::IMMORTAL:
			bonusImmortal.number++;
			break;
		}
	}

	// Bonus countdown freezing
	if (bonusEnergy.number >= bonusEnergy.limit)
		bonusEnergy.countdown = bonusEnergy.countdownTime;
	if (bonusHp.number >= bonusHp.limit)
		bonusHp.countdown = bonusHp.countdownTime;
	if (bonusBerserk.number >= bonusBerserk.limit)
		bonusBerserk.countdown = bonusBerserk.countdownTime;
	if (bonusImmortal.number >= bonusImmortal.limit)
		bonusImmortal.countdown = bonusImmortal.countdownTime;

	// Bonuses spawn
	{
		int r = random::intRandom(0, 3);
		Vec2 pos;
		if (r == 0) {
			pos = bonusEnergy.positions[random::intRandom(0, bonusEnergy.positions.size()-1)];
		}
		if (r == 1) {
			pos = bonusHp.positions[random::intRandom(0, bonusHp.positions.size() - 1)];
		}
		if (r == 2) {
			pos = bonusBerserk.positions[random::intRandom(0, bonusBerserk.positions.size() - 1)];
		}
		if (r == 3) {
			pos = bonusImmortal.positions[random::intRandom(0, bonusImmortal.positions.size() - 1)];
		}

		int x = (int)pos.x;
		int y = (int)pos.y;
		if (!field[x][y].type) {
			bonuses.push_back({});
			bonuses.back().pos = {x + 0.5, y + 0.5};

			if (r == 0 && bonusEnergy.number < bonusEnergy.limit && bonusEnergy.countdown < 0) {
				bonuses.back().type = Bonus::ENERGY;
				bonusEnergy.countdown = bonusEnergy.countdownTime;
			}
			if (r == 1 && bonusHp.number < bonusHp.limit && bonusHp.countdown < 0) {
				bonuses.back().type = Bonus::HP;
				bonusHp.countdown = bonusHp.countdownTime;
			}
			if (r == 2 && bonusBerserk.number < bonusBerserk.limit && bonusBerserk.countdown < 0) {
				bonuses.back().type = Bonus::BERSERK;
				bonusBerserk.countdown = bonusBerserk.countdownTime;
			}
			if (r == 3 && bonusImmortal.number < bonusImmortal.limit && bonusImmortal.countdown < 0) {
				bonuses.back().type = Bonus::IMMORTAL;
				bonusImmortal.countdown = bonusImmortal.countdownTime;
			}
		}
	}


			
	// Delete bonuses
	for (int i = 0; i < bonuses.size(); i++) {
		if (bonuses[i].type == Bonus::NONE) {
			bonuses.erase(bonuses.begin() + i);
			i--;
		}
	}
}