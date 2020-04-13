#include "DrawSystem.h"
#include <math.h>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <sstream>

#include <iostream>



DrawSystem::DrawSystem(){
	window = new sf::RenderWindow(sf::VideoMode(600, 600), "Level Editor");
	loadTextures();
}

DrawSystem::~DrawSystem(){}


Vector2d DrawSystem::getCursorPos() {
	return geom::rotate((mouse.pos - cam.border / 2) / cam.scale + cam.pos, cam.angle);
}
void DrawSystem::drawScene() {
	w = window->getSize().x;
	h = window->getSize().y;
	sf::View view(sf::FloatRect(
		sf::Vector2f(cam.pos.x - w * 1 / cam.scale / 2, cam.pos.y - h * 1 / cam.scale / 2),
		sf::Vector2f(w * 1 / cam.scale, h * 1 / cam.scale)
	));
	view.setRotation(0);
	window->setView(view);
	
	image("backwall",
		(system->field.size())*blockSize / 2,
		(system->field.size())*blockSize / 2,
		(system->field.size())*blockSize,
		(system->field.size())*blockSize, 0);



	drawWalls();

	for (auto u : system->units) {
		drawShip(dynamic_cast<Ship*>(u));
		drawTurret(dynamic_cast<Turret*>(u));
		drawExit(dynamic_cast<Exit*>(u));
		drawBonus(dynamic_cast<Bonus*>(u));
		drawRocketLauncher(dynamic_cast<RocketLauncher*>(u));
		drawLaserCarrierBase(dynamic_cast<LaserCarrier*>(u));
		drawRobot(dynamic_cast<Robot*>(u));
		drawGenerator(dynamic_cast<Generator*>(u));
	}
}

void DrawSystem::drawInterface() {
	window->setView(sf::View(sf::FloatRect(0, 0, w, h)));

	fillRect(menu->box.x*h/2, menu->box.y*h/2, menu->box.x*h, menu->box.y*h, Color(50, 50, 60, 250));
	for (Button* b : menu->buttons) {
		fillRect(b->pos.x*h, b->pos.y*h, b->box.x*h, b->box.y*h, Color(180, 180, 180));
		if (b->type == 0) {
			std::stringstream ss;
			ss << b->value;
			std::string str;
			ss >> str;
			text(b->text, b->pos.x*h, b->pos.y*h - 0.01*h, 0.06*h, Color(70, 70, 70));
			text(str, b->pos.x*h, b->pos.y*h + 0.04*h, 0.04*h, Color(70, 70, 70));
		}
		if (b->type == 1) {
			if (b == menu->b_color) {
				fillRect(b->pos.x*h, b->pos.y*h, b->box.x*h*0.7, b->box.y*h*0.7, Color(20, 20, 20, 255));
				fillRect(b->pos.x*h, b->pos.y*h, b->box.x*h*0.6, b->box.y*h*0.6, system->colorMatches[(int)b->value]);
			}
			else {
				for (int i = 0; i < 8; i++) {
					Vector2d pos = Vector2d(b->pos.x*h, b->pos.y*h) + geom::direction(b->step*i * M_PI / 4) * b->box.x*h*0.45;
					image("pointer", pos.x, pos.y, b->box.x*h / 10, b->box.y*h / 10, b->step*i * M_PI / 4 + M_PI);
				}
				image("pointer", b->pos.x*h, b->pos.y*h, b->box.x*h, b->box.y*h, b->value * M_PI/4);
			}
		}
	}

	Vector2d pos = getCursorPos();
	for (Unit* u : system->units) {
		if (geom::distance(pos, u->body.pos) < 0.5) {
			
			
			std::stringstream ss;
			int n = 0;
			ss << "POS: (" << (int)u->body.pos.x << "; " << (int)u->body.pos.y << ") END "; n++;
			ss << "DIR: " << u->body.direction *180 / M_PI << "° END "; n++;
			ss << "W: " << u->body.w / 2 / M_PI << "Hz END "; n++;

			Shooter* shooter;
			if (shooter = dynamic_cast<Shooter*>(u)) {
				ss << "BV: " << shooter->gun.bulletVelocity << "m/s END "; n++;
				ss << "CD: " << shooter->gun.cooldownTime << "s END "; n++;
				ss << "DIRS: " << shooter->gun.directions << " END "; n++;
			}
			LaserCarrier* laserCarrier;
			if (laserCarrier = dynamic_cast<LaserCarrier*>(u)) {
				ss << "DIRS: " << laserCarrier->lasers.size() << " END "; n++;
			}
			std::string str;
			while (ss) {
				std::string str1;
				ss >> str1;
				if (str1 == "END") {
					str += "\n";
				} else
					str += str1 + " ";
			}
			
			double size = 0.2*0.1;
			Vector2d box = Vector2d(0.15, size*n*1.4) * h;
			size *= h;
			fillRect(mouse.pos.x + box.x / 2, mouse.pos.y + box.y / 2, box.x, box.y, Color(0, 0, 0, 200));
			
			text(str, mouse.pos.x + box.x/2 , mouse.pos.y + box.y  , size, Color(180, 180, 180));
		}
	}
}

void DrawSystem::draw() {
	cam.border = Vector2d(window->getSize().x, window->getSize().y);
	System& sys = *system;
	window->clear();
	drawScene();	
	drawInterface();

}
