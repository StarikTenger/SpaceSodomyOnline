#pragma once
#include "Color.h"
#include "Object.h"
#include "Module.h"
#include <string>

class Player {
public:
	enum Orders { // names for command (this also matches them to their id)
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
	Engine engine; // engine parameters
	Gun gun; // gun parameters
	Bonus::Type activeAbility = Bonus::NONE; // bonus slot
	std::vector<double> effects = { 0,0,0,0,0,0,0,0,0,0 }; // time ot cooldown for every effect
	std::vector<Module> modules = { {Module::Type::HP_UP}, {Module::Type::ENERGY_UP} };

	int alive = 1; // 0 - dead, 1 - alive
	int kills = 0; // number of kills
	int deaths = 0; // number of deaths
	int team = 0; // team id
	int progress = 0; // kill streak
	int lastContact = 0; // id of hostile object, last contacted with
	int localTime = 0; // client time for ping

	double afkTimer = 0; // indicates type from last action
	double timer = 0; // time in system
	

	std::string name = "???";
	
	Color color;

	Object* object = nullptr; // reference to object, nullptr if it's dead (doesn't exist)

	Player() {};
};

