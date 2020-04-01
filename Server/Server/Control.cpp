#include "Control.h"
#include "getMilliCount.h"
#include <iostream>
#include <fstream>
#include <sstream>

Control::Control() {
	sys = System("level.lvl");
	// teams
	sys.teams.insert({ 1, {} });
	sys.teams.insert({ 2, {} });
	sys.teams[2].spawnpoints = { {2, 2}, {47, 47} };
	sys.teams[1].spawnpoints = { 
		{20, 16}, 
		{22, 36},
		{3, 37},
		{47, 11}
	};

	// players
	sys.setPlayer({ 1, 1,   {146, 235, 52}, {23, 23}, 1, 0 });
	sys.setPlayer({ 2, 1,  {0, 135, 47}, {27, 27}, 1, 0 });
	sys.setPlayer({ 4, 1, {0, 255, 208},{20, 16}, 1, 0 });


	sys.setPlayer({ 3, 2, {255, 20, 20},{47, 47}, 1, 0 });
	sys.objects.back().gun.cooldownTime = 0.3;
	sys.objects.back().gun.consumption = 0.5;
	sys.objects.back().gun.vel = 22;
	sys.objects.back().engine.linearForce = 5;

	//sys.setPlayer({ 5, 2, {0, 255, 64},{47, 47}, 1, 0 });
	//sys.setPlayer({ 3, 2, {255, 38, 96},{43, 47}, 1, 0 });
	//sys.setPlayer({ 4, 2, {255, 234, 79},{3, 44}, 1, 0 });
	//sys.setPlayer({ 5, 2, {194, 41, 255},{23, 25}, 1, 0 });

	socket.setBlocking(0);
	socket.bind(8001);
	std::cout << sizeof(buffer) << " --- \n";

}

Control::~Control() {

}


void Control::loadConfig() {
	
}


void Control::saveConfig() {
	
}

void Control::step() {
	receive();
	int timeMs = getMilliCount();
	if (timeMs - timePrev > dt) {
		timePrev = timeMs;
		
		checkMessages();

		for (int i = 0; i < 1; i++) {
			sys.step();
		}
		// Sending
		std::string message = sys.pack();
		for (auto a : addresses) { 		
			socket.send(message.c_str(), message.size() + 1, a.first, a.second);
		}
	}
}

void Control::receive() {
	// Receiving
	socket.receive(buffer, sizeof(buffer), received, sender, port);

	if (received) {
		// Check if address is new
		bool repeat = 0;
		for (auto a : addresses)
			if (a.first == sender && a.second == port)
				repeat = 1;

		if (!repeat) {
			addresses.push_back({ sender, port });
		}

		// Applying received info
		
		messages.push_back(std::string(buffer));
		received = 0;
	}
}

void Control::checkMessages() {
	while(messages.size()) {
		std::stringstream ss;
		ss << messages.front();
		messages.pop_front();

		int id;
		std::string name;
		ss >> id;
		ss >> name;
	
		if (sys.players.find(id) == sys.players.end())
			continue;

		sys.players[id].name = name;

		for (auto& object : sys.objects) {
			if (object.type == Object::SHIP && object.id == id) {
				// Setting all orders to 0
				for (int i = 0; i < object.orders.size(); i++)
					object.orders[i] = 0;
				// Getting commands from string
				char command;
				while (ss >> command) {
					switch (command) {
					case 'L':
						object.orders[Object::MOVE_LEFT] = 1;
						break;
					case 'R':
						object.orders[Object::MOVE_RIGHT] = 1;
						break;
					case 'U':
						object.orders[Object::MOVE_FORWARD] = 1;
						break;
					case 'D':
						object.orders[Object::MOVE_BACKWARD] = 1;
						break;
					case 'l':
						object.orders[Object::TURN_LEFT] = 1;
						break;
					case 'r':
						object.orders[Object::TURN_RIGHT] = 1;
						break;
					case 'S':
						object.orders[Object::SHOOT] = 1;
						break;
					case 's':
						object.orders[Object::STABILIZE_ROTATION] = 1;
						break;
					case 'a':
						object.orders[Object::ACTIVATE] = 1;
						break;
					}
				}
			}
		}	
	}
}
