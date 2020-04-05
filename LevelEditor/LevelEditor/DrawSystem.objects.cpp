#include "DrawSystem.h"
#include <math.h>
#include <algorithm>

#include <iostream>


void DrawSystem::drawShip(Ship* s) {
	if (!s)
		return;
	auto p = s->body.pos;
	double a = geom::angle(s->body.vel);
	double d = geom::distance({}, s->body.vel) * 1;
	int n = 5;
	if (!dynamic_cast<Robot*>(s))
		for (int i = 0; i < n; i++) {
			Vector2d pos = p + geom::direction(a)*d*i / n;
			image("arrow", pos.x, pos.y, blockSize*0.5, blockSize*0.5, a);
		}

	if (s->engine.directMode && s->orders.forward)
		image("directFireRed", p.x, p.y, blockSize, blockSize, s->body.direction + s->engine.direction);

	else {
		if (dynamic_cast<Robot*>(s))
			image("robot", p.x, p.y, blockSize, blockSize, s->body.direction);
		else
			image("ship", p.x, p.y, blockSize, blockSize, s->body.direction);
	}
	if (!s->engine.directMode) {
		if (s->orders.forward)
			image("mainFire", p.x, p.y, blockSize, blockSize, s->body.direction);
		if (s->orders.backward)
			image("backFire", p.x, p.y, blockSize, blockSize, s->body.direction);
		if (s->orders.right)
			image("leftFire", p.x, p.y, blockSize, blockSize, s->body.direction);
		if (s->orders.left)
			image("rightFire", p.x, p.y, blockSize, blockSize, s->body.direction);
		if (s->orders.turnRight)
			image("leftTurnFire", p.x, p.y, blockSize, blockSize, s->body.direction);
		if (s->orders.turnLeft)
			image("rightTurnFire", p.x, p.y, blockSize, blockSize, s->body.direction);
	}
}


void DrawSystem::drawTurret(Turret* s) {
	if (!s)
		return;
	auto p = s->body.pos;
	//image("circle", p.x, p.y, s->activeRadius * 2, s->activeRadius * 2, 0);
	if (s->anger > s->angerThreshold)
		image("turretAngry", p.x, p.y, blockSize, blockSize, s->body.direction);
	else {
		image("turret", p.x, p.y, blockSize, blockSize, s->body.direction);
		if (s->anger > 0) {
			int n = 4;
			double radius = (s->angerThreshold - s->anger) / s->angerThreshold;
			for (int i = 0; i < n; i++) {
				double a = 2 * M_PI*i / n;
				Vector2d pos = p + geom::direction(a)*radius;
				image("angerParticle", pos.x, pos.y, blockSize, blockSize, a);
			}
		}
	}
}


void  DrawSystem::drawExit(Exit* s) {
	if (!s)
		return;
	auto p = s->body.pos;
	image("exit", p.x, p.y, blockSize, blockSize, s->body.direction*0.2);
	image("exit1", p.x, p.y, blockSize, blockSize, s->body.direction*0.8);
	image("exit2", p.x, p.y, blockSize, blockSize, s->body.direction);
}


void  DrawSystem::drawBonus(Bonus* s) {
	if (!s)
		return;
	auto p = s->body.pos;
	double k = 0;
	double newSize = blockSize * (0.5 + k);
	if (s->type == Bonus::ENERGY)
		image("bonusEnergy", p.x, p.y, newSize, newSize, s->body.direction);
	if (s->type == Bonus::HP)
		image("bonusHp", p.x, p.y, newSize, newSize, s->body.direction);
	if (s->type == Bonus::BERSERK)
		image("bonusBerserk", p.x, p.y, newSize, newSize, s->body.direction);
	if (s->type == Bonus::IMMORTAL)
		image("bonusImmortal", p.x, p.y, newSize, newSize, s->body.direction);
	if (s->type == Bonus::BOOST)
		image("bonusBoost", p.x, p.y, newSize, newSize, s->body.direction);
	if (s->type == Bonus::LASER)
		image("bonusLaser", p.x, p.y, newSize, newSize, s->body.direction);
}

void DrawSystem::drawRocketLauncher(RocketLauncher* s) {
	if (!s)
		return;
	auto p = s->body.pos;
	double da = M_PI * 2 / (double)s->gun.directions;
	auto angles = geom::angleDistribution(s->body.direction, s->gun.divergenceAngle, s->gun.directions);
	for (double a : angles) {
		image("rocketLauncher", p.x, p.y, blockSize, blockSize, a);
	}
}

void DrawSystem::drawLaserCarrier(LaserCarrier* s) {
	if (!s)
		return;
	auto p = s->body.pos;
	
}
void DrawSystem::drawLaserCarrierBase(LaserCarrier* s) {
	if (!s)
		return;
	auto p = s->body.pos;
	for (auto l : s->lasers) {
		Vector2d pos = (l.base + l.end) / 2;
		image("laserBase", pos.x, pos.y, geom::distance(l.base, l.end) - 0, 1, s->body.direction + l.direction);
	}
	for (auto l : s->lasers) {
		image("laserCarrier", p.x, p.y, blockSize, blockSize, s->body.direction + l.direction);
	}
}

void DrawSystem::drawRobot(Robot* s) {
	if (!s)
		return;
	drawShip(s);
}

void DrawSystem::drawGenerator(Generator* s) {
	if (!s)
		return;
	auto p = s->body.pos;
	image("generatorBody", p.x, p.y, blockSize, blockSize, s->body.direction);
	image("generatorLight", p.x, p.y, blockSize, blockSize, s->body.direction, system->colorMatches[s->color]);
}



