#include "Control.h"
#include "getMilliCount.h"
#include <iostream>
#include <fstream>
#include <sstream>

Control::Control() {
	//buffer = new char(1024);
	sys = System("level.lvl");
	sys.setPlayer({ 1, {48, 145, 255}, {3, 3}, 1, 0 });
	sys.setPlayer({ 2, {0, 255, 64},{46, 5}, 1, 0 });
	sys.setPlayer({ 3, {255, 38, 96},{43, 47}, 1, 0 });
	sys.setPlayer({ 4, {255, 234, 79},{3, 46}, 1, 0 });
	sys.setPlayer({ 5, {194, 41, 255},{23, 25}, 1, 0 });

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
		for (auto a : addresses) { 
			std::string message = sys.pack();
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
		ss >> id;

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
					}
				}
			}
		}

		
	}
}
