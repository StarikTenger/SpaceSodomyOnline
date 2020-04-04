#include "Replay.h"
#include "System.h"
#include <iostream>
#include <thread>

Replay::Replay(std::string path) {
	std::ifstream replayFile = std::ifstream(path);
	std::string str;
	while (std::getline(replayFile, str)) {
		frames.push_back(str);
	}
	replayFile.close();
}
void Replay::step() {
	if(play)
		frame += speed * 1;
	if (frame < 0)
		frame = 0;
	if (frame >= frames.size())
		frame = frames.size() - 1;

}

void Replay::speedUp() {
	if (speed == -1)
		speed = 1;
	else if (speed < -1)
		speed /= 2;
	else
		speed *= 2;
}
void Replay::speedDown() {
	if (speed == 1)
		speed = -1;
	else if (speed > 1)
		speed /= 2;
	else
		speed *= 2;
}