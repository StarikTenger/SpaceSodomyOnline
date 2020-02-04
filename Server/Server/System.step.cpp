#include "geometry.h"
#include "System.h"
#include "random.h"
#include <iostream>
#include <math.h>

void System::step() {
	// Cooldown
	for (auto& object : objects) {
		if (object.type == Object::SHIP)
			object.gun.timeToCooldown -= dt;
		if (object.type == Object::BULLET)
			object.hp -= dt;
	}

	// Orders
	for (auto& object : objects) {
		if (object.type != Object::SHIP) // Orders are for ships only
			continue;
		if (object.orders[Object::STABILIZE_ROTATION] && 
			!object.orders[Object::TURN_LEFT] && 
			!object.orders[Object::TURN_RIGHT])
			if (object.w > EPS)
				object.orders[Object::TURN_LEFT] = 1;
			else if (object.w < -EPS)
				object.orders[Object::TURN_RIGHT] = 1;

		if (object.orders[Object::MOVE_FORWARD])
			object.vel += geom::rotate(Vec2(object.engine.linearForce, 0), object.dir) * dt;
		if (object.orders[Object::MOVE_RIGHT])
			object.vel += geom::rotate(Vec2(object.engine.linearForce, 0), object.dir + M_PI * 0.5) * dt;
		if (object.orders[Object::MOVE_BACKWARD])
			object.vel += geom::rotate(Vec2(object.engine.linearForce, 0), object.dir + M_PI) * dt;
		if (object.orders[Object::MOVE_LEFT])
			object.vel += geom::rotate(Vec2(object.engine.linearForce, 0), object.dir + M_PI * 1.5) * dt;

		if (object.orders[Object::TURN_RIGHT])
			object.w += object.engine.angularForce * dt;
		if (object.orders[Object::TURN_LEFT])
			object.w -= object.engine.angularForce * dt;

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

	// Delete
	for (int i = 0; i < objects.size(); i++) {
		if (objects[i].hp < EPS) {
			objects.erase(objects.begin() + i);
			i--;
		}
	}

	// Movement
	for (auto& object : objects) {
		object.pos += object.vel * dt;
		object.dir += object.w * dt;
	}
}