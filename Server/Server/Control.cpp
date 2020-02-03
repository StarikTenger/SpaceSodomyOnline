#include "Control.h"
#include "getMilliCount.h"
#include <iostream>
#include <fstream>
#include <sstream>

Control::Control() {
	//buffer = new char(1024);
	sys = System("level.lvl");
	sys.objects.push_back({ {3, 4}, 1, 0 });
	sys.objects.push_back({ {4, 4}, 1, 0 });
	sys.objects.back().id = 10;

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
		std::stringstream ss;
		ss << buffer;
		int id;
		ss >> id;

		for (auto& object : sys.objects) {
			if (object.id == id) {
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
					case 's':
						object.orders[Object::STABILIZE_ROTATION] = 1;
						break;
					}
				}
			}
		}


		received = 0;
	}
}
