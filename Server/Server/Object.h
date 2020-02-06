#pragma once
#include "geometry.h"
#include "Engine.h"
#include "Gun.h"
#include "geometry.h"
#include "color.h"
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
	Color color;

	Vec2 pos; // position
	Vec2 vel; // velocity

	// Ship atributes (only for objects with SHIP type)
	Engine engine;
	Gun gun;

	double m = 1; // mass
	double r = 0.4; // radius 
	double dir = 0; // direction
	double w = 0; // angular velocity
	double hpMax = 5; // maximum hit points
	double hp = hpMax; // hit points
	double energyMax = 10;
	double energy = energyMax;
	double energyRecovery = 1;

	bool collision = 1;

	std::vector<bool> orders = {0,0,0,0,0,0,0,0,0,0}; // shows which commands are active

	Object();
	Object(int _id, Color _color, Vec2 _pos, double _m, double _dir);
};

