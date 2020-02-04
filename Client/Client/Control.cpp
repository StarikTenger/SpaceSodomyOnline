#include "Control.h"
#include "getMilliCount.h"
#include <iostream>
#include <fstream>
#include <sstream>

Control::Control() {
	for (int i = 0; i < 100; i++) {
		keyMatches.push_back({});
	}
	std::ifstream keys("keys.conf");
	while (keys) {
		std::string key;
		keys >> key;
		while (keys) {
			std::string key1;
			keys >> key1;
			if (key1 == "END")
				break;
			keyMatches[getKey(key1)].push_back(getKey(key));

		}
	}

	sys = System("level.lvl");
	
	std::ifstream config("config.conf");
	config >> address;
	config >> port;
	config >> id;
	config.close();

	sys.id = id;

	socket.setBlocking(0);
}

Control::~Control() {

}

Vec2 Control::getCursorPos() {
	return geom::rotate((mouse.pos - drawSys.cam.border/2) / drawSys.cam.scale, drawSys.cam.dir);
}

void Control::loadConfig() {
	
}


void Control::saveConfig() {
	
}

void Control::step() {
	int timeMs = getMilliCount();
	if (timeMs - timePrev > dt) {
		timePrev = timeMs;
		
		events();
		drawSys.mouse = mouse;			

		if (keys[ZOOM_OUT])
			drawSys.cam.scale /= pow(drawSys.cam.scaleVel, 1.0 / (double)dt);
		if (keys[ZOOM_IN])
			drawSys.cam.scale *= pow(drawSys.cam.scaleVel, 1.0 / (double)dt);

		// Send
		std::string message = "";
		message += std::to_string(id) + " ";
		if (keys[MOVE_LEFT])
			message += "L";
		if (keys[MOVE_RIGHT])
			message += "R";
		if (keys[MOVE_FORWARD]) 
			message += "U";
		if (keys[MOVE_BACKWARD])
			message += "D";
		if (keys[TURN_RIGHT])
			message += "r";
		if (keys[TURN_LEFT])
			message += "l";
		if (keys[STABILIZE_ROTATION])
			message += "s";
		if (keys[SHOOT])
			message += "S";
		socket.send(message.c_str(), message.size() + 1, address, port);

		// Receive
		/*std::size_t received = 0;
		sf::IpAddress sender;
		unsigned short port = 0;
		socket.receive(buffer, sizeof(buffer), received, sender, port);*/
		//std::cout << buffer << "\n";
		sys.state = std::string(buffer);
		sys.unpack(sys.state);

		drawSys.system = &sys;
		drawSys.draw();
		drawSys.window->display();

	}
	std::size_t received = 0;
	sf::IpAddress sender;
	unsigned short port = 0;
	socket.receive(buffer, sizeof(buffer), received, sender, port);
}
