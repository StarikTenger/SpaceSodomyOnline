#pragma once
#include "geometry.h"
#include "Engine.h"
#include "Gun.h"
#include "geometry.h"
#include <vector>



class Object {
public:
	enum Type {
		SHIP,
		BULLET,
		EXPLOSION
	};

	enum Orders {
		MOVE_LEFT,
		MOVE_RIGHT,
		MOVE_FORWARD,
		MOVE_BACKWARD,
		TURN_LEFT,
		TURN_RIGHT,
		SHOOT,
		STABILIZE_ROTATION
	};

	int id = 666;
	Type type = SHIP;

	Vec2 pos; // position
	Vec2 vel; // velocity

	// Ship atributes (only for objects with SHIP type)
	Engine engine;
	Gun gun;

	double m = 0; // mass
	double r = 0.4; // radius 
	double dir = 0; // direction
	double w = 0; // angular velocity
	double hp = 1; // hit points

	bool collision = 1;

	std::vector<bool> orders = {0,0,0,0,0,0,0,0,0,0}; // shows which commands are active

	Object();
	Object(Vec2 _pos, double _m, double _dir);
};

