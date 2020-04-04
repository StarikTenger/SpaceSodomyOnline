#pragma once
#include <ctime>
#include <algorithm>
#include <fstream>
#include <vector>
#include <string>
#include <thread>
#include "Parameter.h"

class Replay {
public:
	std::vector<std::string> frames;
	std::vector<Parameter> graph;
	std::thread load;

	int frame = 0;
	int speed = 2;
	bool play = 1;

	Replay() {};
	Replay(std::string path);

	void step();
	void speedUp();
	void speedDown();
};

