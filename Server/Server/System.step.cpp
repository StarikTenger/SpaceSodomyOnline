#include "geometry.h"
#include "System.h"
#include "random.h"
#include <iostream>
#include <math.h>

void System::step() {
	// Counters
	for (auto& b : bonusInfo) {
		b.countdown -= dt;
	}
	for (auto& object : objects) {
		if (object.type == Object::SHIP) {
			double k = 1;
			if (object.effects[Bonus::BERSERK] > 0) {
				k = 3;
				object.energy = object.energyMax;
			}
			object.gun.timeToCooldown -= dt * k;
			object.energy += object.energyRecovery * dt;
			if (object.energy > object.energyMax)
				object.energy = object.energyMax;


			for (auto& effect : object.effects)
				effect -= dt;
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
			double k = 1;
			if (object.effects[Bonus::BOOST] > 0)
				k = 5;

			if (object.orders[Object::MOVE_FORWARD]) {
				object.vel += geom::rotate(Vec2(object.engine.linearForce, 0), object.dir) * dt * k;
				object.energy -= object.engine.consumptionLinear * dt;
			}

			if (object.orders[Object::MOVE_RIGHT]) {
				object.vel += geom::rotate(Vec2(object.engine.linearForce, 0), object.dir + M_PI * 0.5) * dt * k;
				object.energy -= object.engine.consumptionLinear * dt;
			}

			if (object.orders[Object::MOVE_BACKWARD]) {
				object.vel += geom::rotate(Vec2(object.engine.linearForce, 0), object.dir + M_PI) * dt * k;
				object.energy -= object.engine.consumptionLinear * dt;
			}

			if (object.orders[Object::MOVE_LEFT]) {
				object.vel += geom::rotate(Vec2(object.engine.linearForce, 0), object.dir + M_PI * 1.5) * dt * k;
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
		
		// activate
		if (object.orders[Object::ACTIVATE]) {
			switch (object.activeAbility) {
			case Bonus::BERSERK:
				object.effects[Bonus::BERSERK] = 10;
				break;
			case Bonus::IMMORTAL:
				object.effects[Bonus::IMMORTAL] = 10;
				break;
			case Bonus::BOOST:
				object.effects[Bonus::BOOST] = 5;
				break;
			}
			object.activeAbility = Bonus::NONE;
		}
	}

	// Collison (&damage)
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
				Vec2 pos = Vec2(x + 0.5, y + 0.5);
				if (teams.find(player.second.team) != teams.end() && teams[player.second.team].spawnpoints.size())
					pos = teams[player.second.team].spawnpoints[random::intRandom(0, teams[player.second.team].spawnpoints.size() - 1)];
				else if (field[x][y].type)
					continue;

				player.second.deaths++;
				setPlayer({ player.first, player.second.team, player.second.color, pos, 1, 0 });
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
				object.activeAbility = Bonus::BERSERK;
			}
			if (bonus.type == Bonus::IMMORTAL) {
				bonus.type = Bonus::NONE;
				object.activeAbility = Bonus::IMMORTAL;
			}
			if (bonus.type == Bonus::BOOST) {
				bonus.type = Bonus::NONE;
				object.activeAbility = Bonus::BOOST;
			}
		}
	}

	// Bonuses calculating
	for (auto& b : bonusInfo) {
		b.number = 0;
	}
	for (auto& bonus : bonuses) {
		bonusInfo[bonus.type].number ++;
	}

	// Bonus countdown freezing
	for (auto& b : bonusInfo) 
		if (b.number >= b.limit)
			b.countdown = b.countdownTime;
	// Bonuses spawn
	{
		int r = random::intRandom(1, bonusInfo.size() - 1);
		Vec2 pos;
		pos = bonusInfo[r].positions[random::intRandom(0, bonusInfo[r].positions.size() - 1)];

		int x = (int)pos.x;
		int y = (int)pos.y;
		bool repeat = 0;
		for (const auto& bonus : bonuses) {
			if ((int)bonus.pos.x == x && (int)bonus.pos.y == y)
				repeat = 1;
		}
		if (!field[x][y].type && bonusInfo[r].number < bonusInfo[r].limit && bonusInfo[r].countdown < 0 && !repeat) {
			bonuses.push_back({});
			bonuses.back().pos = { x + 0.5, y + 0.5 };

			bonuses.back().type = static_cast<Bonus::Type>(r);
			bonusInfo[r].countdown = bonusInfo[r].countdownTime;
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