#pragma once
#include <ctime>
#include <algorithm>
#include <fstream>
#include <deque>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include "keys.h"
#include "random.h"
#include "System.h"
#include "geometry.h"
#include "Mouse.h"
#include "Gamepad.h"

enum controlMode {
	MENU, GAME
};

class Control{
public:
	//net stuff
	sf::UdpSocket socket;
	char buffer[1024];
	std::size_t received = 0;
	sf::IpAddress sender;
	unsigned short port = 0;
	std::vector<std::pair<sf::IpAddress, int>> addresses;
	std::deque<std::string> messages;

	std::ofstream replay;

	controlMode mode = MENU;

	System sys;

	std::vector<bool> keys = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	std::vector<std::vector<int> > keyMatches;


	//time
	int dt = 20;
	int timePrev = 0;
	bool stop = 0;
	int iteration = 0;


	Control();
	~Control();

	void step();
	void receive();
	void checkMessages();
	std::string configParametersAdress = "parameters.conf";
	void loadConfig();
	void saveConfig();
};

