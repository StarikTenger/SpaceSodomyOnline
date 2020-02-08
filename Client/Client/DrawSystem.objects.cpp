#include "DrawSystem.h"
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