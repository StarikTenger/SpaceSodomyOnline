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
			// beam
			if (object.id == sys.id) {
				beam(object.pos, geom::dir(object.vel + geom::direction(object.dir) * sys.bulletVel), { 255, 0, 0, 130 });
			}

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

			if (object.id != sys.id) {
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

		}
		if (object.type == Object::BULLET) {
			if (sys.privilegies) {
				Color col = object.color;
				col.a = 100;
				beam(object.pos, geom::dir(object.vel), col);
			}
			double r1 = object.r * 2.0 * 2;
			image("bullet", object.pos.x, object.pos.y, r1, r1, object.dir, object.color);
		}
	}

	drawWalls();
}

void DrawSystem::drawInterface() {
	// Configuring camera
	System& sys = *system;
	w = window->getSize().x;
	h = window->getSize().y;

	// Absolute view
	window->setView(sf::View(sf::FloatRect(0, 0, w, h)));

	

	// hp, energy
	for (const auto& object : sys.objects) {
		if (object.type == Object::SHIP && object.id == sys.id) {
			double size = w / 5;
			double sizeH = h / 30;
			double alpha = 220;
			// hp
			{
				Vec2 shift = {w / 2, h - sizeH * 2};
				double l = object.hp / object.hpMax * size;
				image("box", shift.x, shift.y, size, sizeH, 0, { 20, 100, 20, alpha });
				image("box", shift.x - (size - l) / 2, shift.y, l, sizeH, 0, { 0, 255, 0, alpha });
			}

			// energy 
			{
				Vec2 shift = { w / 2, h - sizeH };
				double l = object.energy / object.energyMax * size;
				image("box", shift.x, shift.y, size, sizeH, 0, { 0, 107, 145, alpha });
				image("box", shift.x  - (size - l) / 2, shift.y, l, sizeH, 0, { 3, 186, 252, alpha });
			}
		}
	}

	image("interface", w / 2, h / 2, w, h, 0);

	// list
	for (int i = 0; i < sys.players.size(); i++) {
		//std::string str =  + ;
		double size = h / 30;
		double sizeW = w / 60;
		//image("box", 0 , h - sys.players.size() * size + i * size, size * 15, size, 0, {0, 0, 0, 255});
		text("ID" + std::to_string(sys.players[i].id), sizeW * 2 , h - sys.players.size() * size + i * size, size, sys.players[i].color);
		text("" + std::to_string(sys.players[i].kills), sizeW * 10 , h - sys.players.size() * size + i * size, size, sys.players[i].color);
		text("" + std::to_string(sys.players[i].deaths), sizeW * 14 , h - sys.players.size() * size + i * size, size, sys.players[i].color);
	}
}

void DrawSystem::draw() {
	System& sys = *system;
	window->clear();

	drawScene();
	
	drawInterface();
}
