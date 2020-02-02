#include "Object.h"

Object::Object() {

}

Object::Object(Vec2 _pos, double _m, double _dir) {
	pos = _pos;
	m = _m;
	dir = _dir;
}