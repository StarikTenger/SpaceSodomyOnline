#pragma once
#include "geometry.h"
#include "Engine.h"
#include "Gun.h"
#include "Effects.h"
#include "geometry.h"
#include "Color.h"
#include "Bonus.h"
#include <vector>



class Object {
public:
	enum Type {
		SHIP,
		BULLET,
		ROCKET,
		EXPLOSION,
		MASS
	};


	int id = 666;
	int team = 0;
	int type = SHIP;
	Color color;

	Vec2 pos; // position
	Vec2 vel; // velocity
	Vec2 deltaVel; // deltaVel

	// Projectile (bullet, rocket, etc) atributes
	double damage = 1;
	double force = 0;


	// Explosion atributes
	double expansionVel = 1;

	double m = 1; // mass
	double mConst = m; // mass
	double r = 0.4; // radius 
	double dir = 0; // dir
	double w = 0; // angular velocity
	double hpMax = 3; // maximum hit points
	double hp = hpMax; // hit points
	double energyMax = 10;
	double energy = energyMax;
	double energyRecovery = 0;
	double staminaMax = 10;
	double stamina = energyMax;
	double staminaRecovery = 2;

	

	bool collision = 1;

	Object();
	Object(int _id, int _team, Color _color, Vec2 _pos, double _m, double _dir);

	void init();
};

