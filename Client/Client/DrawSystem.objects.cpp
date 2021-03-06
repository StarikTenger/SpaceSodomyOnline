#include "DrawSystem.h"
#include "geometry.h"
#include "random.h"
#include <math.h>
#include <algorithm>

#include <iostream>

void DrawSystem::beam(Vec2 pos, double dir, Color col) {
	Vec2 step = geom::direction(dir) * 0.4;
	Vec2 posPrev = pos;
	for (int i = 0; i < 120; i++) {
		pos += step;
		if (system->checkWall(pos)) {
			pos -= step;
			step = step / 2;
		}
	}
	line(posPrev.x, posPrev.y, pos.x, pos.y, col);
}

void DrawSystem::laserBeam(Vec2 pos, double dir, Color col) {
	Vec2 step = geom::direction(dir) * 0.4;
	Vec2 posPrev = pos;
	for (int i = 0; i < 120; i++) {
		pos += step;
		if (system->checkWall(pos)) {
			pos -= step;
			step = step / 2;
		}
		Vec2 pos1 = pos + geom::rotate(step, M_PI / 2) * random::floatRandom(0, 0.1, 2) * 0.2;
	}

	image("laser", (posPrev.x + pos.x) / 2, (posPrev.y + pos.y) / 2, geom::distance(pos, posPrev), 0.5, dir, col);
	image("laserBase", (posPrev.x + pos.x) / 2, (posPrev.y + pos.y) / 2, geom::distance(pos, posPrev), 0.5, dir);
}

Vec2 DrawSystem::getCenter(System& sys) {
	Vec2 center(0, 0);
	double counter = 0;
	for (const auto& object : sys.objects) {
		if (object.type == Object::SHIP && geom::distance({0, 0}, object.vel) > EPS) {
			double k = 0;
			for (const auto& target : sys.objects) {
				if (&target != &object && target.type == Object::SHIP && geom::distance({ 0, 0 }, target.vel) > EPS) {
					double dis = geom::distance(object.pos, object.pos);
					if (dis < 1)
						dis = 1;
					k += 1.0 / pow(dis, 10);
				}
			}

			center += object.pos * k;
			counter += k;
		}
	}
	if(counter <= EPS)
		return {0, 0};
	return center / counter;
}