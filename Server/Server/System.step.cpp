#include "geometry.h"
#include "System.h"
#include "random.h"
#include <iostream>
#include <math.h>

void System::step() {
	//COUNTERS/////////////////////////////////////////////////////////////////////////

	// For respawning bonuses
	for (auto& b : bonusInfo) {
		b.countdown -= dt;
	}
	// Objects' counters
	for (auto& object : objects) {
		if (object.type == Object::SHIP) {
			auto& player = players[object.id];

			double k = 1; // cooldown speed koeff
			// boosts for berserk mode
			if (player.effects[Bonus::BERSERK] > 0) {
				k = 3;
				object.energy = object.energyMax;
				object.stamina = object.staminaMax;
			}

			// gun
			player.gun.timeToCooldown -= dt * k;

			// energy
			object.energy += object.energyRecovery * dt;
			if (object.energy > object.energyMax)
				object.energy = object.energyMax;

			// stamina
			object.stamina += object.staminaRecovery * dt;
			if (object.stamina > object.staminaMax)
				object.stamina = object.staminaMax;
			
			// effects
			for (auto& effect : player.effects) {
				effect -= dt;
				if (effect < 0)
					effect = 0;
			}

			// modules
			for (auto& module : player.modules)
				module.timeToCooldown -= dt;
		}
		if (object.type == Object::BULLET)
			object.hp -= dt;
		if (object.type == Object::EXPLOSION) {
			object.hp -= dt;
			object.r += object.expansionVel * dt;
		}

	}

	//OBJECTS//////////////////////////////////////////////////////////////////////////
	
	// Add mass
	for (auto& object : objects) {
		if (object.type != Object::SHIP)
			continue;
		object.mConst = object.m;
		object.m += players[object.id].effects[Bonus::MASS] / 5;
	}

	// Matching to players
	for (auto& object : objects) {
		if (object.type != Object::SHIP)
			continue;
		players[object.id].object = &object;
	}

	// Orders
	for (auto& object : objects) {
		if (object.type != Object::SHIP) // Orders are for ships only
			continue;

		auto& player = players[object.id];

		// stabilize
		if (player.orders[Player::STABILIZE_ROTATION] &&
			!player.orders[Player::TURN_LEFT] &&
			!player.orders[Player::TURN_RIGHT])
			if (object.w > EPS * 5)
				player.orders[Player::TURN_LEFT] = 1;
			else if (object.w < -EPS * 5)
				player.orders[Player::TURN_RIGHT] = 1;

		if (object.energy > EPS || 1) { // USELESS CHECK???
			// linear
			double k = 1;
			if (player.effects[Bonus::BOOST] > 0)
				k = 5;

			if (player.orders[Player::MOVE_FORWARD]) {
				object.deltaVel += geom::rotate(Vec2(player.engine.linearForce, 0), object.dir) * dt * k / object.m;
				object.energy -= player.engine.consumptionLinear * dt;
			}

			if (player.orders[Player::MOVE_RIGHT]) {
				object.deltaVel += geom::rotate(Vec2(player.engine.linearForce, 0), object.dir + M_PI * 0.5) * dt * k / object.m;
				object.energy -= player.engine.consumptionLinear * dt;
			}

			if (player.orders[Player::MOVE_BACKWARD]) {
				object.deltaVel += geom::rotate(Vec2(player.engine.linearForce, 0), object.dir + M_PI) * dt * k / object.m;
				object.energy -= player.engine.consumptionLinear * dt;
			}
			
			if (player.orders[Player::MOVE_LEFT]) {
				object.deltaVel += geom::rotate(Vec2(player.engine.linearForce, 0), object.dir + M_PI * 1.5) * dt * k / object.m;
				object.energy -= player.engine.consumptionLinear * dt;
			}

			// angular
			if (player.orders[Player::TURN_RIGHT]) {
				object.w += player.engine.angularForce * dt / object.m;
				object.energy -= player.engine.consumptionAngular * dt;
			}

			if (player.orders[Player::TURN_LEFT]) {
				object.w -= player.engine.angularForce * dt / object.m;
				object.energy -= player.engine.consumptionAngular * dt;
			}
		} 
		else {
			player.orders[Player::MOVE_FORWARD] = 0;
			player.orders[Player::MOVE_RIGHT] = 0;
			player.orders[Player::MOVE_BACKWARD] = 0;
			player.orders[Player::MOVE_LEFT] = 0;
			player.orders[Player::TURN_RIGHT] = 0;
			player.orders[Player::TURN_LEFT] = 0;
		}

		// Shoot
		if (player.orders[Player::SHOOT])
			shoot(object);
		
		// Activate BONUSES
		if (player.orders[Player::ACTIVATE]) {
			switch (player.activeAbility) {
			case Bonus::BERSERK:
				player.effects[Bonus::BERSERK] = 5;
				break;
			case Bonus::IMMORTAL:
				player.effects[Bonus::IMMORTAL] = 5;
				break;
			case Bonus::BOOST:
				player.effects[Bonus::BOOST] = 5;
				break;
			case Bonus::LASER:
				player.effects[Bonus::LASER] = 0.5;
				break;
			}
			player.activeAbility = Bonus::NONE;
		}

		// Using MODULES
		for (int i = Player::MODULE_1; i <= Player::MODULE_2; i++) {
			if (!player.orders[i]) // Order is inactive
				continue;

			// Current module id
			int moduleId = i - Player::MODULE_1; 

			if (player.modules[moduleId].timeToCooldown > 0) // Module is on cooldown
				break;

			// module info
			auto info = moduleInfo[player.modules[moduleId].type];

			if (info.energy > object.energy || info.stamina > object.stamina)
				break;

			// If success=0 => activation failed 
			int success = 1;

			// Check for type of module
			switch (player.modules[moduleId].type) {
			case Module::HP_UP: {
				player.object->hp += 1;
				if (player.object->hp > player.object->hpMax)
					player.object->hp = player.object->hpMax;
				break;
			}

			case Module::ENERGY_UP: {
				player.object->energy += 5;
				break;
			}

			case Module::CASCADE: {
				auto gunVelprev = player.gun.vel;
				player.gun.vel /= 2;
				for (int i = -2; i <= 2; i++) {
					shoot(object, { 0, 0 }, Object::BULLET, i * 0.1, 1);
				}
				player.gun.vel = gunVelprev;
				break;
			}

			case Module::IMPULSE: {
				object.deltaVel += geom::direction(object.dir) * 15;
				break;
			}

			case Module::ROCKET: {
				auto gunVelPrev = player.gun.vel;
				auto gunDamagePrev = player.gun.damage;
				player.gun.vel = 0;
				player.gun.force = 15;
				player.gun.damage = 2;
				shoot(object, { 0.1 , 0 }, Object::ROCKET, 0, 1);
				player.gun.vel = gunVelPrev;
				player.gun.damage = gunDamagePrev;
				player.gun.force = 0;
				break;
			}

			case Module::SPLASH: {
				explode(object, object.pos, 6, M_PI * 2, 13, 0, 1);
				break;
			}

			case Module::IMMORTALITY: {
				player.effects[Bonus::IMMORTAL] = 1.0;
				break;
			}

			case Module::BLINK: {
				auto pos = object.pos + geom::direction(object.dir) * 5;
				if (!checkWall(pos) && field[(int)pos.x][(int)pos.y].allowed)
					object.pos = pos;
				else
					success = 0;
				break;
			}
			case Module::INVISIBILITY: {
				player.effects[Bonus::INVISIBILITY] = 5.0;
				break;
			}
			case Module::MASS: {
				auto gunVelprev = player.gun.vel;
				player.gun.vel = 6;
				shoot(object, { 0.1 , 0 }, Object::MASS, 0, 1);
				shoot(object, { 0.1 , 0.1 }, Object::MASS, 0.02, 1);
				shoot(object, { 0.1 , -0.1 }, Object::MASS, -0.02, 1);
				player.gun.vel = gunVelprev;
				break;
			}
			}		

			// Set timeToCooldown
			if (success) {
				player.modules[moduleId].timeToCooldown = moduleInfo[player.modules[moduleId].type].cooldownTime;
				object.energy -= info.energy;
				object.stamina -= info.stamina;
			}
		}
	}

	// Bullet & Rocket force
	for (auto& object : objects) {
		if (object.type != Object::BULLET && object.type != Object::ROCKET)
			continue;
		object.deltaVel += geom::rotate(Vec2(object.force, 0), object.dir) * dt;
	}

	// Rocket triggering & explosions
	for (auto& object : objects) {
		if (object.type != Object::ROCKET)
			continue;

		// Target-following
		if (0) // Not yet
			for (auto& target : objects) {
				if (geom::distance(object.pos, target.pos) < 6 && object.team != target.team && target.type == Object::SHIP)
					object.dir = geom::dir(target.pos - object.pos);
			}

		// Trigger
		for (auto& target : objects) {
			double triggerRadius = 2;
			if (target.type != Object::SHIP)
				triggerRadius = target.r;

			if (geom::distance(object.pos, target.pos) < triggerRadius && object.team != target.team)
				object.hp = 0;
		}

		// Explode
		if (object.hp < EPS) {
			setExplosion(object, object.pos, object.deltaVel * 0, 2, 5, 0.1, object.damage);
		}
	}

	// Mass sticking
	for (auto& object : objects) {
		if (object.type != Object::MASS)
			continue;

		// Sticking
		for (auto& target : objects) {
			if (geom::distance(object.pos, target.pos) < object.r + target.r && object.team != target.team && target.type == Object::SHIP) {
				players[target.id].effects[Bonus::MASS] += object.m * 5;
				if(target.m > 0)
					target.deltaVel +=  (object.deltaVel - target.deltaVel) * object.m / target.m;
				object.hp = 0;
				damage(object, target, 0);
			}
		}
	}

	// Forcefield
	for (auto& object : objects) {
		int x = (int)object.pos.x;
		int y = (int)object.pos.y;
		if(x >= 0 && y >=0 && x < field.size() && y < field[x].size())
			object.deltaVel += field[x][y].forceField * dt; // object.m;
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
				players[objects[i].id].progress = 0;
				players[objects[i].id].timer = 3;
			}
			objects.erase(objects.begin() + i);
			i--;
		}
	}

	// Respawn
	for (auto& player : players) {
		player.second.timer -= dt;
		if (!player.second.alive) {
			while (1) {
				if (player.second.timer > 0 || player.second.afkTimer < 0)
					break;

				int x = random::intRandom(1, field.size() - 1);
				int y = random::intRandom(1, field.size() - 1);
				Vec2 pos = Vec2(x + 0.5, y + 0.5);
				if (teams.find(player.second.team) != teams.end() && teams[player.second.team].spawnpoints.size())
					pos = teams[player.second.team].spawnpoints[random::intRandom(0, teams[player.second.team].spawnpoints.size() - 1)];
				else if (field[x][y].type)
					continue;

				
				setPlayer({ player.first, player.second.team, player.second.color, pos, 1, 0 });
				objects.back().dir = random::floatRandom(0, 2 * M_PI, 2);
				player.second.alive = 1;
				break;
			}
		}
	}

	// DeltaVel
	for (auto& object : objects) {
		object.vel += object.deltaVel;
		object.deltaVel = {};
	}

	// Collison (&damage)
	collision();

	// Movement
	for (auto& object : objects) {
		object.pos += object.vel * dt;
		object.dir += object.w * dt;
	}

	// Restore mass
	for (auto& object : objects) {
		if (object.type != Object::SHIP)
			continue;
		object.m = object.mConst;
	}

	//BONUSES//////////////////////////////////////////////////////////////////////////

	// Bonuses collect
	for (auto& object : objects) {
		if (object.type != Object::SHIP)
			continue;

		auto& player = players[object.id];

		for (auto& bonus : bonuses) {
			if (geom::distance(object.pos, bonus.pos) >= object.r * 2)
				continue;

			if (bonus.type == Bonus::ENERGY) {
				bonus.type = Bonus::NONE;
				object.energy += 5;
			}
			else if (bonus.type == Bonus::HP) {
				if (object.hp < object.hpMax - EPS) {
					bonus.type = Bonus::NONE;
					object.hp += 1;
				}
			}
			else if (bonus.type != Bonus::NONE) {
				player.activeAbility = bonus.type;
				bonus.type = Bonus::NONE;
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

	// Bonus countdown freezing (if count is more than limit => countdown freezes)
	for (auto& b : bonusInfo) 
		if (b.number >= b.limit)
			b.countdown = b.countdownTime;

	// Bonuses spawn
	for (int i = 0; i < 1; i++) {
		int r = random::intRandom(1, bonusInfo.size() - 1);

		if (r >= bonusInfo.size() || !bonusInfo[r].positions.size()) // No bonuses of this type
			continue;

		Vec2 pos;
		pos = bonusInfo[r].positions[random::intRandom(0, bonusInfo[r].positions.size() - 1)];

		int x = (int)pos.x;
		int y = (int)pos.y;
		bool repeat = 0;
		for (const auto& bonus : bonuses) {
			if ((int)bonus.pos.x == x && (int)bonus.pos.y == y)
				repeat = 1;
		}
		if ( (x >0 && y > 0 && x < field.size() && y < field[0].size()) && 
			(!field[x][y].type && bonusInfo[r].number < bonusInfo[r].limit && bonusInfo[r].countdown < 0 && !repeat)) {
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
	
	//PLAYERS//////////////////////////////////////////////////////////////////////////

	for (auto& player : players) {
		player.second.afkTimer -= dt;
	}
}