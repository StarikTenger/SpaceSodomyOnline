#pragma once
#include "Vec2.h""
#include <vector>

class BonusInfo {
public:
	int limit = 0;
	int number = 0;
	double countdownTime = 10;
	double countdown = 0;

	std::vector<Vec2> positions;
	BonusInfo() {};
	BonusInfo(int _limit, double _countdownTime);
};

