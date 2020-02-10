#include "Control.h"
#include "getMilliCount.h"
#include <iostream>
#include <fstream>
#include <sstream>

Control::Control() {
	sys = System("level.lvl");
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
	drawSys.system = &sys;
}

Control::~Control() {

}

Vector2d Control::getCursorPos() {
	return drawSys.getCursorPos();
}


void Control::step() {
	int timeMs = getMilliCount();
	if (timeMs - timePrev > dt) {
		timePrev = timeMs;
				
		events();
		drawSys.mouse = mouse;
		drawSys.menu = &menu;
		
		
		if (saved) {
			drawSys.window->setTitle("Level Editor");
		}
		else {
			drawSys.window->setTitle("Level Editor*");
		}		
		actions();
		drawSys.draw();
		drawSys.window->display();
		
	}
}
