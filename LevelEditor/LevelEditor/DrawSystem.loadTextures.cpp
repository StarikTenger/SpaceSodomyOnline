#include "DrawSystem.h"
#include <iostream>
#include <fstream>

void DrawSystem::loadTexture(std::string name, std::string path) {
	sf::Texture* tex = new sf::Texture();
	textures.insert(std::make_pair(name,
		tex
	));
	textures[name]->loadFromFile(path);
}

void DrawSystem::loadTextures() {
	font.loadFromFile("font.ttf");

	std::ifstream file("textures.conf");
	while (file) {
		std::string name, path;
		file >> name >> path;
		loadTexture(name, path);
	}
}