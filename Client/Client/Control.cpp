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
	drawSys.cam.dir = -M_PI / 2;

	sys = System("level.lvl");
	
	loadConfig();

	sys.id = id;

	socket.setBlocking(0);

	if (name == "REPLAY")
		mode = REPLAY;

	// Replay
	if (mode == REPLAY) {
		dt = 40;
		replay = Replay("replay.rep");
	}
}

Control::~Control() {

}

Vec2 Control::getCursorPos() {
	return geom::rotate((mouse.pos - drawSys.cam.border/2) / drawSys.cam.scale, drawSys.cam.dir);
}

void Control::loadConfig() {
	std::ifstream config("config.conf");
	config >> address;
	config >> port;
	config >> id;
	config >> name;
	for(int i = 0; i < 2; i++)
		config >> sys.modules[i];
	config.close();
}

void Control::saveConfig() {
	
}

void Control::step() {
	int timeMs = getMilliCount();

	// Timer action
	if (timeMs - timePrev > dt) {
		timePrev = timeMs;
		sys.time = timeMs * 0.001;

		drawSys.cam.border = { drawSys.w, drawSys.h };

		drawSys.cam.dir = -M_PI / 2;

		if (!mouseVideoMode) {
			if (mouse.state) {
				drawSys.cam.pos += (mousePrev.pos - mouse.pos) / drawSys.cam.scale;
			}
			double dS = pow(1.1, mouse.delta);
			drawSys.cam.scale *= dS;
			drawSys.cam.pos += (drawSys.cam.border / 2 - mouse.pos) / drawSys.cam.scale * (1 - dS); // it works
		}

		// Events
		events();
		drawSys.mouse = mouse;

		// Zoom
		if (keys[ZOOM_OUT])
			drawSys.cam.scale /= pow(drawSys.cam.scaleVel, 1.0 / (double)dt);
		if (keys[ZOOM_IN])
			drawSys.cam.scale *= pow(drawSys.cam.scaleVel, 1.0 / (double)dt);

		if (mode == GAME){
			// Stab
			if (keys[STABILIZE_ROTATION_AUTO] && !keysPrev[STABILIZE_ROTATION_AUTO]) {
				stabilize = !stabilize;
			}

			// Send
			std::string message = "";
			message += std::to_string(id) + " ";
			message += name + " #";

			// Mouse
			if (mouse.state) {
				double ang = geom::dir(mouse.pos - drawSys.cam.border / 2);
				if(sin(ang) > 0 && random::floatRandom(0, 1, 3) < sin(ang))
					message += "D";
				if (sin(ang) < 0 && random::floatRandom(-1, 0, 3) > sin(ang))
					message += "U";
				if (cos(ang) > 0 && random::floatRandom(0, 1, 3) < cos(ang))
					message += "R";
				if (cos(ang) < 0 && random::floatRandom(-1, 0, 3) > cos(ang))
					message += "L";
				
			}

			if (keys[MOVE_LEFT])
				message += "L";
			if (keys[MOVE_RIGHT])
				message += "R";
			if (keys[MOVE_FORWARD] || keys[MOVE_FORWARD1])
				message += "U";
			if (keys[MOVE_BACKWARD] || keys[MOVE_BACKWARD1])
				message += "D";
			if (keys[TURN_RIGHT])
				message += "r";
			if (keys[TURN_LEFT])
				message += "l";
			if (stabilize || keys[STABILIZE_ROTATION])
				message += "s";
			if (keys[ACTIVATE])
				message += "a";
			if (keys[MODULE_1])
				message += "1";
			if (keys[MODULE_2])
				message += "2";
			if (keys[SHOOT])
				message += "S";

			// modulesCooldown
			message += " " + std::to_string(sys.modules[0]) + " " + std::to_string(sys.modules[1]);
			socket.send(message.c_str(), message.size() + 1, address, port);


			sys.state = std::string(buffer);
		} 
		else {
			
			if (keys[LEFT] && !keysPrev[LEFT])
				replay.speedDown();
			if (keys[RIGHT] && !keysPrev[RIGHT])
				replay.speedUp();
			if (keys[SPACE] && !keysPrev[SPACE])
				replay.play = !replay.play;
			if (keys[S] && !keysPrev[S])
				replay.smartMode = !replay.smartMode;
			if (keys[H] && !keysPrev[H])
				replay.hud = !replay.hud;
			if (keys[UP] && !keysPrev[UP]) {
				replay.focusId++;
				if (replay.focusId > sys.players.size()) {
					replay.focusId = 0;
				}
				std::cout << replay.focusId << "\n";
			}
			//std::cout << sys.players.size() << " s\n";
			if (keys[DOWN] && !keysPrev[DOWN]) {
				replay.focusId--;
				if (replay.focusId < 0) {
					replay.focusId = sys.players.size();
				}
				std::cout << replay.focusId << "\n";
			}

			// Set sys.id
			{
				sys.id = 0;
				int i = 0;
				for (const auto& player : sys.players) {
					if (i == replay.focusId)
						break;
					i++;
					sys.id = player.first;
				}
			}

			// Mouse
			if (mouse.state && !mousePrev.state && mouse.pos.y > drawSys.h - 10)
				mouseVideoMode = 1;
			if(mouseVideoMode && !mouse.state && mousePrev.state)
				mouseVideoMode = 0;
			if (mouseVideoMode)
				replay.frame = (int)(replay.frames.size() * mouse.pos.x / drawSys.w);

			replay.step();
			sys.state = replay.frames[replay.frame];
		}

		// Copy of system
		sysPrev = sys;

		sys.unpack(sys.state);

		drawSys.mode = mode;
		drawSys.system = &sys;
		drawSys.replay = &replay;
		drawSys.draw();
		drawSys.window->display();

		setSounds();

	}

	// Getting messages from server
	for (int i = 0; i < 5; i++) {
		std::size_t received = 0;
		sf::IpAddress sender;
		unsigned short port = 0;
		socket.receive(buffer, sizeof(buffer), received, sender, port);
	}
}
