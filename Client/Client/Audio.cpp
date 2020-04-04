#include "Audio.h"
#include <iostream>


Audio::Audio(){
	loadSound("damage", "sounds/damage.wav");
	loadSound("launch", "sounds/launch.wav");
	loadSound("explosion", "sounds/explosion.wav");
	loadSound("fire", "sounds/fire.wav");
	loadSound("knock", "sounds/knock.wav");
	loadSound("bip", "sounds/bip.wav");
	loadSound("death", "sounds/death.wav");
}

Audio::~Audio(){
}

void Audio::loadSound(std::string name, std::string path) {
	sf::SoundBuffer* buffer = new sf::SoundBuffer();
	buffer->loadFromFile(path);
	sf::Sound* sound = new sf::Sound();
	sound->setBuffer(*buffer);
	sounds.insert(std::make_pair(name,
		sound
	));
}

void Audio::play(std::string name) {
	play(name, 100);
}
void Audio::play(std::string name, double volume) {
	sounds[name]->setVolume(volume);
	sounds[name]->play();
}
void Audio::play(std::string name, Vec2 pos, double volume) {
	for (int i = 0; i < activeSounds.size(); i++) {
		if (activeSounds[i]->getStatus() != sf::Sound::Playing) {
			//std::cout << activeSounds.size() << " deleted\n";
			delete activeSounds[i];
			activeSounds.erase(activeSounds.begin() + i);
			i--;
		}
	}
	sf::Listener::setDirection(1.f, 0.f, 0.f);
	sf::Sound* sound = new sf::Sound();
	*sound = *sounds[name];
	sound->setPosition(volume, pos.x, pos.y);
	sound->setVolume(100);
	sound->play();
	activeSounds.push_back(sound);
}

void Audio::play(std::string name, Vec2 pos, double volume, Camera cam) {
	pos -= cam.pos;
	pos = geom::rotate(pos, -cam.dir);
	play(name, pos, 1 / cam.scale);
}
