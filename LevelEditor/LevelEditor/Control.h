#pragma once
#include <ctime>
#include <algorithm>
#include <fstream>
#include <SFML/Graphics.hpp>

#include "keys.h"
#include "DrawSystem.h"
#include "System.h"
#include "geometry.h"
#include "Menu.h"
#include "Mouse.h"

enum MouseMode {
	_MOVE_CAMERA,
	_ERASE, _WALL, 
	_CORNER_A,
	_CORNER_B,
	_CORNER_C,
	_CORNER_D
};

class Control{
public:
	System sys;
	Menu menu;
	DrawSystem drawSys;
	Mouse mouse;
	MouseMode mode = _MOVE_CAMERA;
	std::vector<bool> keys = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	std::vector<bool> keysPrev = keys;
	std::vector<std::vector<int> > keyMatches;
	bool saved = 1;

	//time
	int dt = 20;
	int timePrev = 0;
	bool stop = 0;
	int iteration = 0;


	Control();
	~Control();

	void events();	
	void step();
	void actions();
	Vector2d getCursorPos();
};

