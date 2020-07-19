#pragma once
#include "Geometry.h"

enum CellType {
	EMPTY,
	WALL,
	CORNER_A,
	CORNER_B,
	CORNER_C,
	CORNER_D
	//A --- B
	//|     |
	//|     |
	//D --- C
};

class Cell{ 
public:
	int type = 0;
	int spikes = 0;
	int color = 0;
	int allowed = 1;
	
	Vec2 forceField = { 0, 0 };

	Cell();
	~Cell();
};

