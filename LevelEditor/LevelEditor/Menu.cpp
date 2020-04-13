#include "Menu.h"



Menu::Menu(){
	box = {0.25, 1};
	for(int i=0; i<6; i++){
		buttons.push_back(new Button());
		buttons.back()->pos = Vector2d(1.0 / 7, 1.0 / 7- 0.35 / 6) + Vector2d(0, 1.0 / 6) * i;
		buttons.back()->box = {0.14, 0.14};
	}
	b_dir = buttons[0];
	b_w = buttons[1];
	b_bv = buttons[2];
	b_cd = buttons[3];
	b_dirs = buttons[4];
	b_color = buttons[5];

	buttons[0]->text = "DIR";
	buttons[1]->text = "W";
	buttons[2]->text = "BV";
	buttons[3]->text = "CD";
	buttons[4]->text = "DIRS";
	buttons[5]->text = "DIRS";

	buttons[0]->value = 0;
	buttons[1]->value = 0;
	buttons[2]->value = 5;
	buttons[3]->value = 1;
	buttons[4]->value = 1;
	buttons[5]->value = 0;

	buttons[0]->step = 1;
	buttons[1]->step = 0.1;
	buttons[2]->step = 0.5;
	buttons[3]->step = 0.05;
	buttons[4]->step = 1;
	buttons[5]->step = 1;

	b_dir->type = 1;
	b_color->type = 1;

	b_dir->bound = Vector2d(0, 8);
	b_w->bound = Vector2d(-10, 10);
	b_bv->bound = Vector2d(0, 50);
	b_cd->bound = Vector2d(0, 1000);
	b_dirs->bound = Vector2d(1, 12);
	b_color->bound = Vector2d(0, 5);
}


Menu::~Menu(){
}
