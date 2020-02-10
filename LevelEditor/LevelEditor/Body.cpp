#include "Body.h"


Body::Body()
{
}

Body::Body(Vector2d _pos, Vector2d _vel, double _m) {
	pos = posPrev = _pos;
	vel = _vel;
	m = _m;
}


Body::~Body()
{
}
