#include "System.h"
#include "geometry.h"
#include "random.h"

#include <vector>
#include <algorithm>
#include <iostream>

#include "getMilliCount.h"

using namespace random;
using namespace std;

bool System::collision(Object& body, std::pair<Vec2, Vec2> wall) {
	double blockSize = 1;
	bool touch = 0;

	// Calculating mid perpendicular
	Vec2 center = (wall.first + wall.second) / 2;
	std::pair<Vec2, Vec2> perpendicular;
	perpendicular.first = geom::rotate(wall.first - center, M_PI / 2) + center;
	perpendicular.second = geom::rotate(wall.second - center, M_PI / 2) + center;

	// Length of wall
	double length = geom::distance(wall.first, wall.second);

	// Pos after movement
	Vec2 posNew = body.pos + body.vel * dt;

	// Checking collision
	if (geom::distance(posNew, wall.first, wall.second) < body.r &&
		geom::distance(posNew, perpendicular.first, perpendicular.second) < length / 2 &&
		geom::distance(posNew, wall.first, wall.second) < geom::distance(body.pos, wall.first, wall.second)) {
		touch = 1;
		Vec2 velNew = geom::rotate(body.vel, -geom::dir(wall.first - wall.second));
		velNew.y *= -bounce;
		body.vel = geom::rotate(velNew, geom::dir(wall.first - wall.second));
	}
	else if (geom::distance(posNew, wall.first) < body.r) { // Point collision
		touch = 1;
		Vec2 velNew = geom::rotate(body.vel, -geom::dir(wall.first - posNew));
		velNew.x *= -bounce;
		body.vel = geom::rotate(velNew, (geom::dir(wall.first - posNew)));
	}
	else if (geom::distance(posNew, wall.second) < body.r) {
		touch = 1;
		Vec2 velNew = geom::rotate(body.vel, -geom::dir(wall.second - posNew));
		velNew.x *= -bounce;
		body.vel = geom::rotate(velNew, (geom::dir(wall.second - posNew)));
	}
	return touch;
}

void System::collision() {
	double blockSize = 1;
	bool touch = 0;

	// Objects
	for (Object& a : objects) {
		for (Object& b : objects) {
			Vec2 aPos = a.pos + a.vel * dt;
			Vec2 bPos = b.pos + b.vel * dt;

			// Damage
			if (a.team == b.team || a.id == b.id || &a == &b || !a.collision || !b.collision)
				continue;

			const auto& player = players[a.id];

			if (geom::distance(a.pos, b.pos) < a.r + b.r) {
				if (a.type == Object::BULLET) {
					if(b.type == Object::ROCKET)
						damage(a, b, 1000);
					damage(a, b, a.damage);
					damage(b, a, 1000);
				}

				if (player.effects[Bonus::BOOST] > 0) {
					damage(a, b, parameters.boost_damage);
				}

				//a.vel += geom::dir(a.pos, b.pos) * dt * 10 / a.m;
			}

		}
	}

	// Walls
	for (Object& u : objects) {
		vector<pair<pair<Vec2, Vec2>, bool> > walls;
		walls.push_back({ {{0, 0}, {0, (double)field.size()}}, 0 });
		walls.push_back({ {{0, 0}, {(double)field.size(), 0}}, 0 });
		walls.push_back({ {{(double)field.size(), (double)field.size()}, {0, (double)field.size()}}, 0 });
		walls.push_back({ {{(double)field.size(), (double)field.size()}, {(double)field.size(), 0}}, 0 });

		int x0 = (int)(u.pos.x / blockSize + 1) - 1;
		int y0 = (int)(u.pos.y / blockSize + 1) - 1;
		for (int x = std::max(0, x0 - 1); x <= std::min((int)field.size() - 1, x0 + 1); x++) {
			for (int y = std::max(0, y0 - 1); y <= std::min((int)field[0].size() - 1, y0 + 1); y++) {
				// Block verticies
				Vec2 pA(x + 0.0, y + 0.0);
				Vec2 pB(x + 1.0, y + 0.0);
				Vec2 pC(x + 1.0, y + 1.0);
				Vec2 pD(x + 0.0, y + 1.0);
				bool danger = field[x][y].spikes;
				if (field[x][y].type == WALL) {
					walls.push_back({ {pA, pB}, danger });
					walls.push_back({ {pB, pC}, danger });
					walls.push_back({ {pC, pD}, danger });
					walls.push_back({ {pD, pA}, danger });
				}
				if (field[x][y].type == CORNER_A) {
					walls.push_back({ {pA, pB}, danger });
					walls.push_back({ {pA, pD}, danger });
					walls.push_back({ {pB, pD}, danger });
				}
				if (field[x][y].type == CORNER_B) {
					walls.push_back({ {pA, pB}, danger });
					walls.push_back({ {pB, pC}, danger });
					walls.push_back({ {pA, pC}, danger });
				}
				if (field[x][y].type == CORNER_C) {
					walls.push_back({ {pB, pC}, danger });
					walls.push_back({ {pC, pD}, danger });
					walls.push_back({ {pB, pD}, danger });
				}
				if (field[x][y].type == CORNER_D) {
					walls.push_back({ {pC, pD}, danger });
					walls.push_back({ {pA, pD}, danger });
					walls.push_back({ {pA, pC}, danger });
				}
			}
		}

		// Damage
		bool touch = 0;
		bool dmg = 0;
		for (const auto& wall : walls) {
			if (collision(u, wall.first)) {
				touch = 1;
				if (wall.second)
					dmg = 1;
			}
		}

		// Check distance (for sure)
		for (auto& w : walls) {
			if (geom::distance(u.pos, w.first.first) < u.r)
				u.pos -= geom::direction(w.first.first, u.pos) * 0.05;
			if (geom::distance(u.pos, w.first.second) < u.r)
				u.pos -= geom::direction(w.first.second, u.pos) * 0.05;
		}

		auto& player = players[u.id];
		if (dmg && u.type == Object::SHIP && player.effects[Bonus::IMMORTAL] < EPS) {
			u.hp -= 1;
			if (u.hp < EPS) {
				wallKills++;
				players[u.id].deaths++;
				players[u.id].kills -= parameters.isSuicideDecreaseKills;

				// Last contact
				if (players[u.id].lastContact) {
					players[players[u.id].lastContact].kills++;
					players[players[u.id].lastContact].progress++;

				}
			}
		}

		// Kill bullets & rockets if contact with wall
		if (touch && ( u.type == Object::BULLET || u.type == Object::ROCKET || u.type == Object::MASS) ) {
			u.hp = 0;
		}

		// If in wall
		if (checkWall(u.pos)) {
			//std::cout << "q";
			u.hp = -4;
		}
	}

	

	// Laser collision
	for (auto& object : objects) {
		const auto& player = players[object.id];
		if (object.type == Object::SHIP && player.effects[Bonus::LASER] > 0) {
			for (auto& target : objects) {
				if (checkAbilityToHit(object, target, 0.4))
					damage(object, target, parameters.bonus_laser_damage);
			}
		}
	}
}