#include "Object.h"

Object::Object() {

}

Object::Object(int _id, Color _color, Vec2 _pos, double _m, double _dir){
	id = _id;
	color = _color;
	pos = _pos;
	m = _m;
	dir = _dir;
}