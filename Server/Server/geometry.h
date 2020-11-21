#pragma once
#include "Vec2.h"
#include <vector>

# define M_PI   3.14159265358979323846  /* pi */
# define EPS    0.0001

namespace geom {
	bool isCross(Vec2 a, Vec2 b, Vec2 c, Vec2 d);
	Vec2 getCross(Vec2 a, Vec2 b, Vec2 c, Vec2 d);
	Vec2 direction(double a);
	Vec2 direction(Vec2  a, Vec2  b);
	double dir(Vec2 p);
	double dir(Vec2 a, Vec2 b, Vec2 c);
	double distance(Vec2  a, Vec2 b);
	double distance(Vec2  p, Vec2 a, Vec2 b);
	Vec2 rotate(Vec2 p, double dir);
	double square(std::vector<Vec2> polygon);
	bool inPolygon(Vec2 point, std::vector<Vec2> polygon);
	std::vector<double> angleDistribution(double direction, double width, int n);
	double normalizeAngle(double dir);
}