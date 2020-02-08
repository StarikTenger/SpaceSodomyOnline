#include <algorithm>
#include "Animation.h"

Animation::Animation(){
}
Animation::Animation(std::string _img, AnimationState _stateStart, AnimationState _stateFinish, double _timeStart, double _timeFinish) {
	img = _img;
	stateStart = _stateStart;
	stateFinish = _stateFinish;
	time = timeStart = _timeStart;
	timeFinish = _timeFinish;
}

Animation::~Animation(){
}

void Animation::setState() { // Setting state from current animation time
	double k = std::min(1.0, std::max(0.0, (1 - (timeFinish - time) / (timeFinish - timeStart))));
	state.pos = stateStart.pos + (stateFinish.pos - stateStart.pos)*k;
	state.box = stateStart.box + (stateFinish.box - stateStart.box)*k;
	state.direction = stateStart.direction + (stateFinish.direction - stateStart.direction)*k;
	state.color = stateStart.color + (stateFinish.color - stateStart.color)*k;
}
