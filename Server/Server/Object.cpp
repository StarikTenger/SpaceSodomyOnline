#include "Object.h"

Object::Object() {

}

Object::Object(int _id, int _team, Color _color, Vec2 _pos, double _m, double _dir) {
	id = _id;
	team = _team;
	color = _color;
	pos = _pos;
	m = _m;
	dir = _dir;
}

void Object::init() {
	hp = hpMax;
	energy = energyMax;
	stamina = staminaMax;
}