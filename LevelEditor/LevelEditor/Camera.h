#pragma once
#include "geometry.h"

class Camera{
public:
	Vector2d pos;
	Vector2d border;
	double angle = 0;
	double scale = 30;
	double scaleVel = 2;
	Camera();
	Camera(Vector2d _pos, Vector2d _border, double _scale);
	Vector2d transform(Vector2d p);
	~Camera();
};

