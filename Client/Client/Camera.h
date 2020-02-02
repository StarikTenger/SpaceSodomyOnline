#pragma once
#include "geometry.h"

class Camera{
public:
	Vec2 pos;
	Vec2 border;
	double dir = 0;
	double scale = 30;
	double scaleVel = 2;
	Camera();
	Camera(Vec2 _pos, Vec2 _border, double _scale);
	Vec2 transform(Vec2 p);
	~Camera();
};

