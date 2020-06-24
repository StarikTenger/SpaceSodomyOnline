#pragma once
#include "Color.h"
#include "Object.h"
#include "Module.h"
#include <string>

class Player {
public:
	enum Orders {
		MOVE_LEFT,
		MOVE_RIGHT,
		MOVE_FORWARD,
		MOVE_BACKWARD,
		TURN_LEFT,
		TURN_RIGHT,
		SHOOT,
		STABILIZE_ROTATION,
		ACTIVATE,
		MODULE_1,
		MODULE_2
	};
	std::vector<bool> orders = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0 }; // shows which commands are active

	// Ship atributes (only for objects with SHIP type)
	Engine engine;
	Gun gun;
	Bonus::Type activeAbility = Bonus::NONE;
	std::vector<double> effects = { 0,0,0,0,0,0,0,0 };
	std::vector<Module> modules = { {Module::Type::HP_UP}, {Module::Type::ENERGY_UP} };

	int alive = 1;
	int kills = 0;
	int deaths = 0;
	int team = 0;
	int progress = 0;
	int lastContact = 0;

	double afkTimer = 0;
	double timer = 0;

	std::string name = "???";
	
	Color color;

	Object* object = nullptr;

	Player() {};
};

