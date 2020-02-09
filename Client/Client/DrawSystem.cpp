#include "DrawSystem.h"
#include "random.h"
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


	// Objects
	for (const auto& object : sys.objects) {
		// Ship
		if (object.type == Object::SHIP) {
			// beam
			if (object.id == sys.id || sys.privilegies) {
				beam(object.pos, geom::dir(object.vel + geom::direction(object.dir) * sys.bulletVel), { 255, 0, 0, 130 });
			}

			// model
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

			// effect
			if (object.berserk)
				image("effect", object.pos.x, object.pos.y, r1, r1, object.dir+sys.time*5, {255, 20, 20});
			if (object.immortal)
				image("effect", object.pos.x, object.pos.y, r1, r1, object.dir+sys.time, { 200, 200, 20 });

			// bars
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
		// Bullet
		if (object.type == Object::BULLET) {
			// beam
			if (0 && sys.privilegies) {
				Color col = object.color;
				col.a = 100;
				beam(object.pos, geom::dir(object.vel), col);
			}
			// model
			double r1 = object.r * 3;
			image("bullet", object.pos.x, object.pos.y, r1, r1, object.dir, object.color);
			image("bullet", object.pos.x, object.pos.y, r1 * 0.8, r1 * 0.8, object.dir, {255, 255, 255});

			// set animation
			animation("bullet", 
				AnimationState(object.pos, {0.3, 0.3}, 0, object.color),
				AnimationState(object.pos + geom::direction(random::floatRandom(0, M_PI*2, 2)) * 0.2, { 0.0, 0.0 }, 0, object.color), 
				0.2);
		}
	}

	// Bonuses
	for (const auto& bonus : sys.bonuses) {
		double r = 0.5;
		if (bonus.type == Bonus::ENERGY) {
			image("bonusEnergy", bonus.pos.x, bonus.pos.y, r, r, cam.dir + M_PI / 2);
		}
		if (bonus.type == Bonus::HP) {
			image("bonusHp", bonus.pos.x, bonus.pos.y, r, r, cam.dir + M_PI / 2);
		}
		if (bonus.type == Bonus::BERSERK) {
			image("bonusBerserk", bonus.pos.x, bonus.pos.y, r, r, cam.dir + M_PI / 2);
		}
		if (bonus.type == Bonus::IMMORTAL) {
			image("bonusImmortal", bonus.pos.x, bonus.pos.y, r, r, cam.dir + M_PI / 2);
		}
	}

	// Animations
	for (auto& a : animations) {
		a.time = sys.time;
		a.setState();
		image(a.img, a.state.pos.x, a.state.pos.y, a.state.box.x, a.state.box.y, a.state.direction, a.state.color);
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

	// Animations
	for (auto& a : animationsInterface) {
		a.time = sys.time;
		a.setState();
		image(a.img, a.state.pos.x, a.state.pos.y, a.state.box.x, a.state.box.y, a.state.direction, a.state.color);
	}

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
				if (object.hp < sys.hpPrev) {
					animationInterface("blood",
						AnimationState(Vec2(w, h) / 2, Vec2(w, h), 0, {255, 255, 255}),
						AnimationState(Vec2(w, h) / 2, Vec2(w, h), 0, { 255, 255, 255, 0}),
						1);
				}

				sys.hpPrev = object.hp;
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
		text(sys.players[i].name, sizeW * 4 , h - sys.players.size() * size + i * size, size/2, sys.players[i].color);
		text("" + std::to_string(sys.players[i].kills), sizeW * 9 , h - sys.players.size() * size + i * size, size/1.5, sys.players[i].color);
		text("" + std::to_string(sys.players[i].deaths), sizeW * 11 , h - sys.players.size() * size + i * size, size / 1.5, sys.players[i].color);
	}
}

void DrawSystem::draw() {
	System& sys = *system;
	window->clear();

	drawScene();
	
	drawInterface();

	// kill animations
	for (int i = 0; i < animations.size(); i++) {
		if (animations[i].time >= animations[i].timeFinish || animations[i].time < animations[i].timeStart) {
			animations.erase(animations.begin() + i);
			i--;
		}
	}
	for (int i = 0; i < animationsInterface.size(); i++) {
		if (animationsInterface[i].time >= animationsInterface[i].timeFinish || animationsInterface[i].time < animationsInterface[i].timeStart) {
			animationsInterface.erase(animationsInterface.begin() + i);
			i--;
		}
	}
}
