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
	config >> name;
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
		sys.time = timeMs * 0.001;

		drawSys.cam.border = { drawSys.w, drawSys.h};

		drawSys.cam.dir = -M_PI/2;
		if (mouse.state) {
			drawSys.cam.pos += (mousePrev.pos - mouse.pos) / drawSys.cam.scale;
		}
		double dS = pow(1.1, mouse.delta);
		drawSys.cam.scale *= dS;
		drawSys.cam.pos += (drawSys.cam.border / 2 - mouse.pos) / drawSys.cam.scale * (1 - dS); // it works

		events();
		drawSys.mouse = mouse;			

		if (keys[ZOOM_OUT])
			drawSys.cam.scale /= pow(drawSys.cam.scaleVel, 1.0 / (double)dt);
		if (keys[ZOOM_IN])
			drawSys.cam.scale *= pow(drawSys.cam.scaleVel, 1.0 / (double)dt);

		// Shoot cheat
		sys.target = 0;
		for (const auto& object : sys.objects) {
			if (sys.checkAbility(sys.mainPlayer, object, 0.06))
				sys.target = 1;
		}


		// Send
		std::string message = "";
		message += std::to_string(id) + " ";
		message += name + " ";
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
		if (keys[ACTIVATE])
			message += "a";
		if (keys[SHOOT])
			message += "S";
		socket.send(message.c_str(), message.size() + 1, address, port);

		sys.state = std::string(buffer);
		sys.unpack(sys.state);

		drawSys.system = &sys;
		drawSys.draw();
		drawSys.window->display();

	}
	for (int i = 0; i < 5; i++) {
		std::size_t received = 0;
		sf::IpAddress sender;
		unsigned short port = 0;
		socket.receive(buffer, sizeof(buffer), received, sender, port);
	}
}
