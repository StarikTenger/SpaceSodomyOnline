#pragma once
#include <ctime>
#include <algorithm>
#include <fstream>
#include <deque>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include "keys.h"
#include "DrawSystem.h"
#include "random.h"
#include "System.h"
#include "geometry.h"
#include "Mouse.h"
#include "Gamepad.h"
#include "Replay.h"
#include "Audio.h"
#include "Sound.h"

enum controlMode {
	GAME, REPLAY
};

class Control{
public:
	// net stuff
	char buffer[2048];
	std::string address = "localhost";
	int port = 8001;
	sf::UdpSocket socket;
	int id = 666;
	std::string name = "";

	controlMode mode = GAME;
	Replay replay;

	System sys;
	System sysPrev;
	DrawSystem drawSys;
	Audio audio;
	Mouse mouse;
	Mouse mousePrev;
	Gamepad joystick;
	bool mouseVideoMode = 0;

	std::deque<Sound> sounds;

	std::vector<bool> keys = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	std::vector<bool> keysPrev = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	std::vector<std::vector<int> > keyMatches;

	//time
	int dt = 20;
	int timePrev = 0;
	bool stop = 0;
	int iteration = 0;


	Control();
	~Control();

	void events();	
	void setSounds();
	void step();
	Vec2 getCursorPos();
	std::string configParametersAdress = "parameters.conf";
	void loadConfig();
	void saveConfig();
};

