#include "Control.h"
#include "getMilliCount.h"
#include "configProcessing.h"
#include <iostream>
#include <fstream>
#include <sstream>

Control::Control() {

	// sys config
	sys = System("level.lvl");
	loadConfig("config.conf");

	// Socket config
	socket.setBlocking(0);
	socket.bind(port);
	std::cout << sizeof(buffer) << " --- \n";

	// Replay
	replay = std::ofstream("replay" + std::to_string(getMilliCount() / 1000) + ".rep");
}

Control::~Control() {

}

// Loading config
void Control::loadConfig(std::string path) {
	std::ifstream file(path);
	std::string command; // Current command
	while (file >> command) {
		if (command == "END") // End of file
			break;
		comment(command, file);

		if (command == "PORT") // Port
			file >> port;

		if (command == "TEAM") {
			int id;
			file >> id;
			sys.teams.insert({ id, {} });
			std::string command1;
			while (file >> command1) {
				if (command1 == "END")
					break;
				comment(command1, file);

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
				comment(command1, file);

				int type = System::bonusNames[command1];
				file >> sys.bonusInfo[type].limit;
				file >> sys.bonusInfo[type].countdownTime;
			}
		}
		if (command == "MODULEINFO") {
			std::string command1;
			while (file >> command1) {
				if (command1 == "END")
					break;
				comment(command1, file);

				int type = System::moduleNames[command1];
				file >> sys.moduleInfo[type].cooldownTime; // Period
				file >> sys.moduleInfo[type].energy; // Energy consumption
				file >> sys.moduleInfo[type].stamina; // Stamina consumption
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

		// Putting message to stringstream
		std::stringstream ss;
		ss << messages.front();
		messages.pop_front();

		// Getting id & name
		int id;
		std::string name;
		ss >> id;
		ss >> name;
	
		// Checking if ID not found
		if (sys.players.find(id) == sys.players.end())
			continue;

		// Giving player a name
		sys.players[id].name = name;

		// AFK management
		std::string orders;		
		if (ss >> orders && orders.size() && orders != "s") { // "s" stands for autostab which is not an action
			sys.players[id].afkTimer = 10;
		}

		// Current player
		auto& player = sys.players[id];

		// Setting all orders to 0
		for (int i = 0; i < player.orders.size(); i++)
			player.orders[i] = 0;

		// Getting commands from string
		std::stringstream orderStream;
		orderStream << orders;

		// Matching commands to orders (CAN BE DONE WITH MAP!!!)
		char command;
		while (orderStream >> command) {
			switch (command) {
			case 'L':
				player.orders[Player::MOVE_LEFT] = 1;
				break;
			case 'R':
				player.orders[Player::MOVE_RIGHT] = 1;
				break;
			case 'U':
				player.orders[Player::MOVE_FORWARD] = 1;
				break;
			case 'D':
				player.orders[Player::MOVE_BACKWARD] = 1;
				break;
			case 'l':
				player.orders[Player::TURN_LEFT] = 1;
				break;
			case 'r':
				player.orders[Player::TURN_RIGHT] = 1;
				break;
			case 'S':
				player.orders[Player::SHOOT] = 1;
				break;
			case 's':
				player.orders[Player::STABILIZE_ROTATION] = 1;
				break;
			case 'a':
				player.orders[Player::ACTIVATE] = 1;
				break;
			case '1':
				player.orders[Player::MODULE_1] = 1;
				break;
			case '2':
				player.orders[Player::MODULE_2] = 1;
				break;
			}
		}	

		// Modules
		for (auto& m : player.modules) {
			ss >> m.type;
		}
	}
}
