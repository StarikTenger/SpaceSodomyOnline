#pragma once
#include <SFML/Audio.hpp>
#include <string>
#include <map>
#include "Vec2.h"
#include "Camera.h"

class Audio {
public:
	std::map<std::string, sf::Sound*> sounds;
	std::vector<sf::Sound*> activeSounds;
	Audio();
	~Audio();
	void loadSound(std::string name, std::string path);
	void play(std::string name);
	void play(std::string name, double volume);
	void play(std::string name, Vec2 pos, double volume);
	void play(std::string name, Vec2 pos, double volume, Camera cam);
};

