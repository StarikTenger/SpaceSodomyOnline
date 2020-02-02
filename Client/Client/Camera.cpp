#include "Camera.h"

Camera::Camera(){
}
Camera::Camera(Vec2 _pos, Vec2 _border, double _scale) {
	pos = _pos;
	border = _border;
	scale = _scale;
}

Camera::~Camera(){
}

Vec2 Camera::transform(Vec2 p) {
	return geom::rotate((p - pos)*scale, -dir) + border / 2;
}
