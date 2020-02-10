#include "DrawSystem.h"
#include <math.h>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <iostream>


void DrawSystem::drawWalls() {
	System& sys = *system;
	Color color(0, 151, 255);
	for (int x = 0; x < sys.field.size(); x++) {
		for (int y = 0; y < sys.field[0].size(); y++) {
			fillRect(x + 0.5, y + 0.5, 1, 1, Color(20, 20, 20));
			strokeRect(x + 0.5, y + 0.5, 1, 1, Color(0, 0, 0));
		}
	}
	for (int x = 0; x < sys.field.size(); x++) {
		for (int y = 0; y < sys.field[0].size(); y++) {
			if (sys.field[x][y].type) {
				Color color1 = color;
				int type = 0;
				if (sys.field[x][y].spikes) {
					type = 1;
					color = Color(255, 0, 0);
					color1 = Color(255, 255, 255);
				}
				int up, down, left, right;
				up = down = left = right = 0;
				if (sys.field[x][y].type == WALL)
					image("wall", x + 0.5, y + 0.5, 1, 1, 0, sys.colorMatches[sys.field[x][y].color]);

				if (x > 0 && !sys.field[x - 1][y].type) {
					left = 1;
				}
				if (x < sys.field.size() - 1 && !sys.field[x + 1][y].type) {
					right = 1;
				}
				if (y > 0 && !sys.field[x][y - 1].type) {
					up = 1;
				}
				if (y < sys.field.size() - 1 && !sys.field[x][y + 1].type) {
					down = 1;
				}
				if (type) {
					if (up && right)
						image("glowCornerRed", x + 1 + 0.5, y - 1 + 0.5, 1, 1, M_PI * 0, color1);
					if (down && right)
						image("glowCornerRed", x + 1 + 0.5, y + 1 + 0.5, 1, 1, M_PI * 0.5, color1);
					if (down && left)
						image("glowCornerRed", x - 1 + 0.5, y + 1 + 0.5, 1, 1, M_PI * 1, color1);
					if (up && left)
						image("glowCornerRed", x - 1 + 0.5, y - 1 + 0.5, 1, 1, M_PI * 1.5, color1);
				}
			}
		}
	}

	for (int x = 0; x < sys.field.size(); x++) {
		for (int y = 0; y < sys.field[0].size(); y++) {
			if (sys.field[x][y].type) {
				color = sys.colorMatches[sys.field[x][y].color];
				Color color1 = color;
				int spikes = 0;
				if (sys.field[x][y].spikes) {
					spikes = 1;
					
					color = Color(255, 0, 0);
					color1 = Color(255, 255, 255);
				}
				int up, down, left, right;
				up = down = left = right = 0;

				if (sys.field[x][y].type == EMPTY)
					continue;
				int type = sys.field[x][y].type;
				if (x > 0 && !sys.field[x - 1][y].type && (type == WALL || type == CORNER_A || type == CORNER_D)) {
					left = 1;
					if (spikes)
						image("glowRed", x - 1 + 0.5, y + 0.5, 1, 1, M_PI, color1);
					image("wallBorder", x + 0.5, y + 0.5, 1, 1, M_PI, color1);
				}
				if (x < sys.field.size() - 1 && !sys.field[x + 1][y].type && (type == WALL || type == CORNER_B || type == CORNER_C)) {
					right = 1;
					if (spikes)
						image("glowRed", x + 1 + 0.5, y + 0.5, 1, 1, 0, color1);
					image("wallBorder", x + 0.5, y + 0.5, 1, 1, 0, color1);
				}
				if (y > 0 && !sys.field[x][y - 1].type && (type == WALL || type == CORNER_A || type == CORNER_B)) {
					up = 1;
					if (spikes)
						image("glowRed", x + 0.5, y - 1 + 0.5, 1, 1, -M_PI * 0.5, color1);
					image("wallBorder", x + 0.5, y + 0.5, 1, 1, -M_PI * 0.5, color1);
				}
				if (y < sys.field.size() - 1 && !sys.field[x][y + 1].type && (type == WALL || type == CORNER_C || type == CORNER_D)) {
					down = 1;
					if (spikes)
						image("glowRed", x + 0.5, y + 1 + 0.5, 1, 1, M_PI * 0.5, color1);
					image("wallBorder", x + 0.5, y + 0.5, 1, 1, M_PI * 0.5, color1);
				}
			}
		}
	}
	double s = sqrt(2);
	for (int x = 0; x < sys.field.size(); x++) {
		for (int y = 0; y < sys.field[0].size(); y++) {
			color = sys.colorMatches[sys.field[x][y].color];
			switch (sys.field[x][y].type) {
			case CORNER_A:
				image("wallA", x + 0.5, y + 0.5, 1, 1, 0, color);
				image("wallBorder", x, y, s, s, M_PI*0.25, color);
				break;
			case CORNER_B:
				image("wallB", x + 0.5, y + 0.5, 1, 1, 0, color);
				image("wallBorder", x + 1, y, s, s, M_PI*0.75, color);
				break;
			case CORNER_C:
				image("wallC", x + 0.5, y + 0.5, 1, 1, 0, color);
				image("wallBorder", x + 1, y + 1, s, s, M_PI*1.25, color);
				break;
			case CORNER_D:
				image("wallD", x + 0.5, y + 0.5, 1, 1, 0, color);
				image("wallBorder", x, y + 1, s, s, M_PI*1.75, color);
				break;
			}
		}
	}
}

