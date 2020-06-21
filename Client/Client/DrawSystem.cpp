#include "DrawSystem.h"
#include "random.h"
#include "getMilliCount.h"
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
	// Smart camera
	smartView = smartView + (getCenter(sys) - smartView) * 0.1;
	if (replay->smartMode)
		cam.pos = smartView;


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

	

	// DRAWING /////////////////////////////////////////////////
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
			if (object.activeAbility == Bonus::LASER) {
				Color col = object.color;
				col.a = 130;
				beam(object.pos, object.dir, col);
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

			// effects
			if (object.effects[Bonus::BERSERK])
				image("effect", object.pos.x, object.pos.y, r1, r1, object.dir+sys.time*5, {255, 20, 20});
			if (object.effects[Bonus::IMMORTAL])
				image("effect", object.pos.x, object.pos.y, r1, r1, object.dir+sys.time, { 200, 200, 20 });
			if (object.effects[Bonus::BOOST])
				animation("bullet",
					AnimationState(object.pos, Vec2( 0.3, 0.3 ) * 5.0, 0, object.color),
					AnimationState(object.pos + geom::direction(random::floatRandom(0, M_PI * 2, 2)) * 0.2, { 0.0, 0.0 }, 0, object.color),
					0.2);
			if (object.effects[Bonus::LASER])
				laserBeam(object.pos, object.dir, object.color);

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
					image("box", object.pos.x + shift.x, object.pos.y + shift. y, l, 0.1, cam.dir + M_PI / 2, { 3, 186, 252, 255 });
				}

				// Target pointer
				if (sys.privilegies) {
					if (sys.checkAbility(sys.mainPlayer, object, 0.06)) 
						image("pointer", object.pos.x, object.pos.y, r1, r1, cam.dir + M_PI / 4);

				}
				
				// name
				{
					auto shift = Vec2(0, 0) - geom::direction(cam.dir) * 0.9;
					std::string str = sys.players[object.id].name;
					if (str.size() > 18)
						str = str.substr(0, 18);
					text(str, object.pos.x + shift.x, object.pos.y + shift.y, 0.03 / 5, cam.dir + M_PI / 2, object.color);
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
		std::string name = "bonusEnergy";
		switch (bonus.type) {
		case Bonus::ENERGY:
			name = "bonusEnergy";
			break;
		case Bonus::HP:
			name = "bonusHp";
			break;
		case Bonus::BERSERK:
			name = "bonusBerserk";
			break;
		case Bonus::IMMORTAL:
			name = "bonusImmortal";
			break;
		case Bonus::BOOST:
			name = "bonusBoost";
			break;
		case Bonus::LASER:
			name = "bonusLaser";
			break;
		}
		
		image(name, bonus.pos.x, bonus.pos.y, r, r, cam.dir + M_PI / 2);
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

	/// ship HUD
	if (mode == 0 || replay->hud && replay->focusId) {
		// Animations
		for (auto& a : animationsInterface) {
			a.time = sys.time;
			a.setState();
			image(a.img, a.state.pos.x, a.state.pos.y, a.state.box.x, a.state.box.y, a.state.direction, a.state.color);
		}

		// hp, energy, active
		for (const auto& object : sys.objects) {
			if (object.type == Object::SHIP && object.id == sys.id) {
				double size = w / 5;
				double sizeH = h / 30;
				double alpha = 220;
				// hp
				{
					Vec2 shift = { w / 2, h - sizeH * 2 };
					double l = object.hp / object.hpMax * size;
					image("box", shift.x, shift.y, size, sizeH, 0, { 20, 100, 20, alpha });
					image("box", shift.x - (size - l) / 2, shift.y, l, sizeH, 0, { 0, 255, 0, alpha });
					if (object.hp < sys.hpPrev) {
						animationInterface("blood",
							AnimationState(Vec2(w, h) / 2, Vec2(w, h), 0, { 255, 255, 255 }),
							AnimationState(Vec2(w, h) / 2, Vec2(w, h), 0, { 255, 255, 255, 0 }),
							1);
					}

					sys.hpPrev = object.hp;
				}

				// energy 
				{
					Vec2 shift = { w / 2, h - sizeH };
					double l = object.energy / object.energyMax * size;
					image("box", shift.x, shift.y, size, sizeH, 0, { 0, 107, 145, alpha });
					image("box", shift.x - (size - l) / 2, shift.y, l, sizeH, 0, { 3, 186, 252, alpha });
				}

				// active
				std::string img = "bonusEmpty";
				switch (object.activeAbility) {
				case Bonus::BERSERK:
					img = "bonusBerserk";
					break;
				case Bonus::IMMORTAL:
					img = "bonusImmortal";
					break;
				case Bonus::BOOST:
					img = "bonusBoost";
					break;
				case Bonus::LASER:
					img = "bonusLaser";
					break;
				}

				size = w * 0.08;
				image(img, w - size * 0.52, h - size * 0.52, size, size, 0);

				// modules
				auto& player = sys.players[object.id];
				std::vector < std::string> moduleImg = {
					"moduleHpUp",
					"moduleEnergyUp",
					"moduleCascade",
					"moduleImpulse",
					"moduleRocket",
					"moduleSplash",
				};
				for (int i = 0; i < 2; i++) {
					std::string img = moduleImg[sys.modules[i] % moduleImg.size()];
					image(img, w - size * 0.52 * (6 - i * 2), h - size * 0.52, size, size, 0, { 50, 50, 50, 200 });
					image(img, w - size * 0.52 * (6 - i * 2), h - size * 0.52 + size * int(player.modules[i] * 24.0) / 24, size, size, 0, { 255, 255, 255 }, { 0.0, player.modules[i] }, { 1.0,  1.0 });
				}


				// stabilization
				if (sys.mainPlayer.orders[Object::STABILIZE_ROTATION]) {
					text("stab", w - size * 1.52, h - size * 0.2, 20, { 255, 255, 255 });
				}
			}
		}

		image("interface", w / 2, h / 2, w, h, 0);
	}

	// replay bar
	if (mode == 1 && replay->hud) {
		double progress = (double)replay->frame / replay->frames.size();
		image("box", w * progress / 2, h, w * progress, 20, 0, { 0, 229, 255, 180 });
		image("box", (w + w * progress) / 2, h, w - w * progress, 20, 0, { 0, 100, 100, 180 });

		if (!replay->focusId) {
			int time = replay->frame * 20;
			text(getTime(time), w - 120, h - 40, 40, Color(181, 247, 255));

			std::string txt = std::to_string(replay->speed / 2);
			if (replay->speed == 1)
				txt = "0.5";
			if (replay->speed == -1)
				txt = "-0.5";

			text(txt, w - 120, h - 85, 40, Color(181, 247, 255));
		}
	}

	// list of players
	if (mode == 0 || replay->hud) {
		// list
		for (int i = 0; i < sys.playerList.size(); i++) {
			//std::string str =  + ;
			double size = h / 30;
			double sizeW = w / 60;
			//image("box", 0 , h - sys.playerList.size() * size + i * size, size * 15, size, 0, {0, 0, 0, 255});
			text(sys.playerList[i].name, sizeW * 4, h - sys.playerList.size() * size + i * size, size / 2, sys.playerList[i].color);
			text("" + std::to_string(sys.playerList[i].kills), sizeW * 9, h - sys.playerList.size() * size + i * size, size / 1.5, sys.playerList[i].color);
			text("" + std::to_string(sys.playerList[i].deaths), sizeW * 11, h - sys.playerList.size() * size + i * size, size / 1.5, sys.playerList[i].color);
			text("" + std::to_string(sys.playerList[i].progress), sizeW * 13, h - sys.playerList.size() * size + i * size, size / 1.5, sys.playerList[i].color);
		}
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
