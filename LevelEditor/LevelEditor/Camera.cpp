#include "Camera.h"

Camera::Camera(){
}
Camera::Camera(Vector2d _pos, Vector2d _border, double _scale) {
	pos = _pos;
	border = _border;
	scale = _scale;
}

Camera::~Camera(){
}

Vector2d Camera::transform(Vector2d p) {
	return geom::rotate((p - pos)*scale, -angle) + border / 2;
}
