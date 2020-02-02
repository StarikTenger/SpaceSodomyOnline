#include "DrawSystem.h"
#include <math.h>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <iostream>
#include <sstream>

DrawSystem::DrawSystem(){
	window = new sf::RenderWindow(sf::VideoMode(600, 600), "Water");
	loadTextures();
	
}

DrawSystem::~DrawSystem(){}

void DrawSystem::drawScene() {
	// Configuring camera
	System& sys = *system;
	w = window->getSize().x;
	h = window->getSize().y;

	for (const auto& object : sys.objects) {
		if (object.id == sys.id) {
			cam.pos = object.pos;
			cam.dir = object.dir;
		}
	}

	// Absolute view
	window->setView(sf::View(sf::FloatRect(0, 0, w, h)));

	// Background
	image("background",
		w / 2,
		h / 2,
		w * 1.5,
		w * 1.5, -cam.dir
	);
	
	
	// Relative view
	sf::View view(sf::FloatRect(
		sf::Vector2f(cam.pos.x - w * 1 / cam.scale / 2, cam.pos.y - h * 1 / cam.scale / 2),
		sf::Vector2f(w * 1 / cam.scale, h * 1 / cam.scale)
	));
	view.setRotation((cam.dir * 180 / M_PI) + 90);
	window->setView(view);

	// Background
	image("backwall1",
		(sys.field.size()) * blockSize / 2,
		(sys.field.size()) * blockSize / 2,
		(sys.field.size()) * blockSize * 2 * 2,
		(sys.field.size()) * blockSize * 2 * 2
		, 0,
		1.0 / 2
	);


	// Drawing ships
	for (const auto& object : sys.objects) {
		image("ship", object.pos.x, object.pos.y, object.r*2, object.r * 2, object.dir);
	}

	drawWalls();
}

void DrawSystem::drawInterface() {
	
}

void DrawSystem::draw() {
	System& sys = *system;
	window->clear();

	drawScene();
	
	drawInterface();
}
