#pragma once
#include <ctime>
#include <algorithm>
#include <fstream>
#include <vector>
#include <string>

class Replay {
public:
	std::vector<std::string> frames;
	int frame = 0;
	int speed = 1;
	bool play = 1;

	Replay() {};
	Replay(std::string path);

	void step();
	void speedUp();
	void speedDown();
};

