#pragma once

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

class Cell {
public:
	int type = 0;
	int spikes = 0;
	int color = 0;
	Cell();
	~Cell();
};

