#include "Control.h"
#include "getMilliCount.h"
#include <iostream>
#include <fstream>
#include <sstream>

void Control::actions() {
	Vector2d pos = getCursorPos();
	if (keys[CONTROL]) {
		if (keys[S] && !keysPrev[S]) {
			sys.save("level.lvl");
			std::cout << "file saved\n";
			saved = 1;
		}
		if (keys[SHIFT] && keys[N] && !keysPrev[N]) {
			std::cout << "new\n";
			for (Unit* u : sys.units) {
				delete u;
			}
			sys = System("levelTemplate.lvl");
		}
	}
	else if (mouse.pos.x < menu.box.x * drawSys.cam.border.y && mouse.pos.y < menu.box.y * drawSys.cam.border.y) {
		double h = drawSys.cam.border.y;
		for (Button* b : menu.buttons) {
			if (mouse.pos.x > (b->pos.x - b->box.x / 2)* h&& mouse.pos.x < (b->pos.x + b->box.x / 2) * h &&
				mouse.pos.y >(b->pos.y - b->box.y / 2)* h&& mouse.pos.y < (b->pos.y + b->box.y / 2) * h) {
				b->value += b->step * mouse.delta;
				if (b->value < b->step && b->value > -b->step) {
					b->value = 0;
				}
				if (b->value > b->bound.y) {
					if (b->type) {
						b->value = b->bound.x;
					}
					else {
						b->value = b->bound.y;
					}
				}
				if (b->value < b->bound.x) {
					if (b->type) {
						b->value = b->bound.y;
					}
					else {
						b->value = b->bound.x;
					}
				}
			}
		}
	}
	else {
		switch (mode) {
		case _MOVE_CAMERA:
			if (mouse.state) {
				drawSys.cam.pos += (mouse.posPrev - mouse.pos) / drawSys.cam.scale;
			}
			break;
		}
		double dS = pow(1.1, mouse.delta);
		drawSys.cam.scale *= dS;
		drawSys.cam.pos += (drawSys.cam.border / 2 - mouse.pos) / drawSys.cam.scale * (1 - dS); //it works

		if (keys[ERASE]) {
			if (pos.x > 0 && pos.x < sys.field.size() && pos.y > 0 && pos.y < sys.field.size()) {
				sys.field[(int)pos.x][(int)pos.y].type = 0;
				sys.field[(int)pos.x][(int)pos.y].allowed = 1;
				sys.field[(int)pos.x][(int)pos.y].spikes = 0;
				sys.field[(int)pos.x][(int)pos.y].forceField = 0;
				sys.field[(int)pos.x][(int)pos.y].color = (int)menu.b_color->value;
				saved = 0;
			}
		}
		if (keys[SET_WALL]) {
			if (pos.x > 0 && pos.x < sys.field.size() && pos.y > 0 && pos.y < sys.field.size()) {
				sys.field[(int)pos.x][(int)pos.y].type = 1;
				sys.field[(int)pos.x][(int)pos.y].allowed = 1;
				sys.field[(int)pos.x][(int)pos.y].spikes = 0;
				sys.field[(int)pos.x][(int)pos.y].color = (int)menu.b_color->value;
				saved = 0;
			}
		}
		if (keys[SET_CORNER_A]) {
			if (pos.x > 0 && pos.x < sys.field.size() && pos.y > 0 && pos.y < sys.field.size()) {
				sys.field[(int)pos.x][(int)pos.y].type = CORNER_A;
				sys.field[(int)pos.x][(int)pos.y].allowed = 1;
				sys.field[(int)pos.x][(int)pos.y].spikes = 0;
				sys.field[(int)pos.x][(int)pos.y].color = (int)menu.b_color->value;
				saved = 0;
			}
		}
		if (keys[SET_CORNER_B]) {
			if (pos.x > 0 && pos.x < sys.field.size() && pos.y > 0 && pos.y < sys.field.size()) {
				sys.field[(int)pos.x][(int)pos.y].type = CORNER_B;
				sys.field[(int)pos.x][(int)pos.y].allowed = 1;
				sys.field[(int)pos.x][(int)pos.y].spikes = 0;
				sys.field[(int)pos.x][(int)pos.y].color = (int)menu.b_color->value;
				saved = 0;
			}
		}
		if (keys[SET_CORNER_C]) {
			if (pos.x > 0 && pos.x < sys.field.size() && pos.y > 0 && pos.y < sys.field.size()) {
				sys.field[(int)pos.x][(int)pos.y].type = CORNER_C;
				sys.field[(int)pos.x][(int)pos.y].allowed = 1;
				sys.field[(int)pos.x][(int)pos.y].spikes = 0;
				sys.field[(int)pos.x][(int)pos.y].color = (int)menu.b_color->value;
				saved = 0;
			}
		}
		if (keys[SET_CORNER_D]) {
			if (pos.x > 0 && pos.x < sys.field.size() && pos.y > 0 && pos.y < sys.field.size()) {
				sys.field[(int)pos.x][(int)pos.y].type = CORNER_D;
				sys.field[(int)pos.x][(int)pos.y].allowed = 1;
				sys.field[(int)pos.x][(int)pos.y].spikes = 0;
				sys.field[(int)pos.x][(int)pos.y].color = (int)menu.b_color->value;
				saved = 0;
			}
		}
		if (keys[SET_SPIKES]) {
			if (pos.x > 0 && pos.x < sys.field.size() && pos.y > 0 && pos.y < sys.field.size()) {
				sys.field[(int)pos.x][(int)pos.y].type = 1;
				sys.field[(int)pos.x][(int)pos.y].allowed = 1;
				sys.field[(int)pos.x][(int)pos.y].spikes = 1;
				sys.field[(int)pos.x][(int)pos.y].color = (int)menu.b_color->value;
				saved = 0;
			}
		}
		if (keys[X]) {
			if (pos.x > 0 && pos.x < sys.field.size() && pos.y > 0 && pos.y < sys.field.size()) {
				sys.field[(int)pos.x][(int)pos.y].type = 0;
				sys.field[(int)pos.x][(int)pos.y].spikes = 0;
				//sys.field[(int)pos.x][(int)pos.y].allowed = 0;
				sys.fill((int)pos.x, (int)pos.y, 0);
				saved = 0;
			}
		}
		if (keys[Z]) {
			if (pos.x > 0 && pos.x < sys.field.size() && pos.y > 0 && pos.y < sys.field.size()) {
				sys.field[(int)pos.x][(int)pos.y].type = 0;
				sys.field[(int)pos.x][(int)pos.y].spikes = 0;
				//sys.field[(int)pos.x][(int)pos.y].allowed = 0;
				sys.fill((int)pos.x, (int)pos.y, 1);
				saved = 0;
			}
		}
		if (keys[NUM1] && !keysPrev[NUM1]) {
			if (pos.x > 0 && pos.x < sys.field.size() && pos.y > 0 && pos.y < sys.field.size()) {
				Bonus* bonus = new Bonus();
				bonus->body.pos = Vector2d((int)pos.x + 0.5, (int)pos.y + 0.5);
				bonus->type = Bonus::ENERGY;
				sys.units.push_back(bonus);
			}
		}
		if (keys[NUM2] && !keysPrev[NUM2]) {
			if (pos.x > 0 && pos.x < sys.field.size() && pos.y > 0 && pos.y < sys.field.size()) {
				Bonus* bonus = new Bonus();
				bonus->body.pos = Vector2d((int)pos.x + 0.5, (int)pos.y + 0.5);
				bonus->type = Bonus::HP;
				sys.units.push_back(bonus);
			}
		}
		if (keys[NUM3] && !keysPrev[NUM3]) {
			if (pos.x > 0 && pos.x < sys.field.size() && pos.y > 0 && pos.y < sys.field.size()) {
				Bonus* bonus = new Bonus();
				bonus->body.pos = Vector2d((int)pos.x + 0.5, (int)pos.y + 0.5);
				bonus->type = Bonus::BERSERK;
				sys.units.push_back(bonus);
			}
		}
		if (keys[NUM4] && !keysPrev[NUM4]) {
			if (pos.x > 0 && pos.x < sys.field.size() && pos.y > 0 && pos.y < sys.field.size()) {
				Bonus* bonus = new Bonus();
				bonus->body.pos = Vector2d((int)pos.x + 0.5, (int)pos.y + 0.5);
				bonus->type = Bonus::IMMORTAL;
				sys.units.push_back(bonus);
			}
		}
		if (keys[NUM5] && !keysPrev[NUM5]) {
			if (pos.x > 0 && pos.x < sys.field.size() && pos.y > 0 && pos.y < sys.field.size()) {
				Bonus* bonus = new Bonus();
				bonus->body.pos = Vector2d((int)pos.x + 0.5, (int)pos.y + 0.5);
				bonus->type = Bonus::BOOST;
				sys.units.push_back(bonus);
			}
		}
		if (keys[NUM6] && !keysPrev[NUM6]) {
			if (pos.x > 0 && pos.x < sys.field.size() && pos.y > 0 && pos.y < sys.field.size()) {
				Bonus* bonus = new Bonus();
				bonus->body.pos = Vector2d((int)pos.x + 0.5, (int)pos.y + 0.5);
				bonus->type = Bonus::LASER;
				sys.units.push_back(bonus);
			}
		}
		if (keys[F] && pos.x > 0 && pos.x < sys.field.size() && pos.y > 0 && pos.y < sys.field.size()) {
			sys.field[(int)pos.x][(int)pos.y].forceField = (int)menu.b_dir->value + 1;
		}

		if (keys[DELETE] && !keysPrev[DELETE] || keys[Q] && !keysPrev[Q]) {
			for (int i = 0; i < sys.units.size(); i++) {
				if (geom::distance(pos, sys.units[i]->body.pos) < 0.5) {
					delete sys.units[i];
					sys.units.erase(sys.units.begin() + i);
					i--;
				}
			}
		}
	}
}