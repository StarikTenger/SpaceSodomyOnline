#include "Control.h"
#include "getMilliCount.h"
#include <iostream>
#include <fstream>
#include <sstream>

Control::Control() {

	// sys config
	sys = System("level.lvl");
	loadConfig("config.conf");

	// Socket config
	socket.setBlocking(0);
	socket.bind(8001);
	std::cout << sizeof(buffer) << " --- \n";

	// Replay
	replay = std::ofstream("replay" + std::to_string(getMilliCount() / 1000) + ".rep");
}

Control::~Control() {

}


void Control::loadConfig(std::string path) {
	std::ifstream file(path);
	std::string command;
	while (file >> command) {
		if (command == "END")
			break;
		if (command == "TEAM") {
			int id;
			file >> id;
			sys.teams.insert({ id, {} });
			std::string command1;
			while (file >> command1) {
				if (command1 == "END")
					break;
				if (command1 == "S") {
					Vec2 pos;
					file >> pos.x >> pos.y;
					sys.teams[id].spawnpoints.push_back(pos);
				}
				if (command1 == "COL") {
					Color col;
					file >> col.r >> col.g >> col.b;
					col.a = 255;
					sys.teams[id].color = col;
				}
			}
		}
		if (command == "PLAYER") {
			int id, team;
			file >> id >> team;
			sys.setPlayer({ id, team, {},{0, 0}, 1, 0 });
		}
		if (command == "BONUSINFO") {
			std::string command1;
			while (file >> command1) {
				if (command1 == "END")
					break;
				
				int type = System::bonusNames[command1];
				file >> sys.bonusInfo[type].limit;
				file >> sys.bonusInfo[type].countdownTime;
			}
		}
	}
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
		replay << message << "\n";
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
				
				// AFK management
				std::string orders;
				
				if (ss >> orders && orders.size() && orders != "s") {
					sys.players[id].afkTimer = 10;
				}

				// Getting commands from string
				std::stringstream orderStream;
				orderStream << orders;
				char command;
				while (orderStream >> command) {
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
