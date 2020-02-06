#include "DrawSystem.h"
#include <math.h>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <iostream>
#include <sstream>

DrawSystem::DrawSystem(){
	window = new sf::RenderWindow(sf::VideoMode(600, 600), "Space Sodomy (Orgy)");
	loadTextures();
	
}

DrawSystem::~DrawSystem(){}

void DrawSystem::drawScene() {
	// Configuring camera
	System& sys = *system;
	w = window->getSize().x;
	h = window->getSize().y;

	for (const auto& object : sys.objects) {
		if (object.type == Object::SHIP && object.id == sys.id) {
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
		if (object.type == Object::SHIP) {
			image("ship", object.pos.x, object.pos.y, object.r * 2, object.r * 2, object.dir);
			image("shipColors", object.pos.x, object.pos.y, object.r * 2, object.r * 2, object.dir, object.color);

			// engines
			double r1 = object.r * 2.0 * 44.0 / 24.0;
			if (object.orders[Object::MOVE_FORWARD])
				image("fireForward", object.pos.x, object.pos.y, r1, r1, object.dir, object.color);
			if (object.orders[Object::MOVE_BACKWARD])
				image("fireBackward", object.pos.x, object.pos.y, r1, r1, object.dir, object.color);
			if (object.orders[Object::MOVE_LEFT])
				image("fireLeft", object.pos.x, object.pos.y, r1, r1, object.dir, object.color);
			if (object.orders[Object::MOVE_RIGHT])
				image("fireRight", object.pos.x, object.pos.y, r1, r1, object.dir, object.color);
			if (object.orders[Object::TURN_LEFT])
				image("fireTurnLeft", object.pos.x, object.pos.y, r1, r1, object.dir, object.color);
			if (object.orders[Object::TURN_RIGHT])
				image("fireTurnRight", object.pos.x, object.pos.y, r1, r1, object.dir, object.color);

			// hp
			{			
				auto shift = Vec2(0, 0) - geom::direction(cam.dir) * 0.5;
				double l = object.hp / object.hpMax * 1;
				image("box", object.pos.x + shift.x, object.pos.y + shift.y, 1, 0.1, cam.dir + M_PI / 2, { 20, 100, 20, 255 });
				shift = Vec2(0, 0) - geom::rotate({ (1 - l) / 2, -0.5 }, cam.dir + M_PI / 2);
				image("box", object.pos.x + shift.x, object.pos.y + shift.y, l, 0.1, cam.dir + M_PI / 2, { 0, 255, 0, 255 });
			}

			// energy
			{
				auto shift = Vec2(0, 0) - geom::direction(cam.dir) * 0.6;
				double l = object.energy / object.energyMax * 1;
				image("box", object.pos.x + shift.x, object.pos.y + shift.y, 1, 0.1, cam.dir + M_PI / 2, { 0, 107, 145, 255 });
				shift = Vec2(0, 0) - geom::rotate({ (1 - l) / 2, -0.6 }, cam.dir + M_PI / 2);
				image("box", object.pos.x + shift.x, object.pos.y + shift.y, l, 0.1, cam.dir + M_PI / 2, { 3, 186, 252, 255 });
			}

		}
		if (object.type == Object::BULLET) {
			double r1 = object.r * 2.0 * 2;
			image("bullet", object.pos.x, object.pos.y, r1, r1, object.dir, object.color);
		}
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
