#pragma once
#include <vector>
#include "Button.h"

class Menu {
public:
	std::vector<Button*> buttons;
	Vector2d box;
	Button* b_dir;
	Button* b_w;
	Button* b_bv;
	Button* b_cd;
	Button* b_dirs;
	Button* b_color;
	Menu();
	~Menu();
};

