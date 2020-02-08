#include "geometry.h"
#include "System.h"
#include "random.h"
#include <iostream>
#include <math.h>

void System::step() {
	// Counters
	for (auto& object : objects) {
		if (object.type == Object::SHIP) {
			object.gun.timeToCooldown -= dt;
			object.energy += object.energyRecovery * dt;
			if (object.energy > object.energyMax)
				object.energy = object.energyMax;
		}
		if (object.type == Object::BULLET)
			object.hp -= dt;
	}

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

	// Bonuses collect
	for (auto& object : objects) {
		if (object.type == Object::SHIP) {
			for (auto& bonus : bonuses) {
				if (geom::distance(object.pos, bonus.pos) < object.r * 2) {
					if (bonus.type == Bonus::ENERGY) {
						bonus.type = Bonus::NONE;
						object.energy += 5;
					}
				}
			}
		}
	}

	// Bonusese set
	if (bonuses.size() < bonusLimit) {
		int x = random::intRandom(1, field.size() - 1);
		int y = random::intRandom(1, field.size() - 1);
		if (!field[x][y].type) {
			bonuses.push_back({});
			bonuses.back().pos = {x + 0.5, y + 0.5};
		}
	}
			

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

	// Delete bonuses
	for (int i = 0; i < bonuses.size(); i++) {
		if (bonuses[i].type == Bonus::NONE) {
			bonuses.erase(bonuses.begin() + i);
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
}