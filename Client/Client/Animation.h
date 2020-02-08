#pragma once
#include <string>
#include "geometry.h"
#include "Color.h"

struct AnimationState {
	Vec2 pos;
	Vec2 box;
	double direction = 0;
	Color color;
	AnimationState() {};
	AnimationState(Vec2 _pos, Vec2 _box, double _direction, Color _color) {
		pos = _pos;
		box = _box;
		direction = _direction;
		color = _color;
	}
};

class Animation {
public:
	AnimationState stateStart;
	AnimationState stateFinish;
	AnimationState state;
	std::string img;
	double timeStart = 0;
	double timeFinish = 0;
	double time = 0;
	Animation();
	Animation(std::string _img, AnimationState _stateStart, AnimationState _stateFinish, double _timeStart, double _timeFinish);
	~Animation();
	void setState();
};

