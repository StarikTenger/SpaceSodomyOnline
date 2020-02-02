#include "Vec2.h"

Vec2::Vec2(double x1, double y1) {
	x = x1;
	y = y1;
}
Vec2 operator+(const Vec2& a, const Vec2& b) {
	return Vec2(a.x + b.x, a.y + b.y);
}
Vec2 operator-(const Vec2& a, const Vec2& b) {
	return Vec2(a.x - b.x, a.y - b.y);
}
Vec2 operator*(const Vec2& a, double k) {
	return Vec2(a.x*k, a.y*k);
}
Vec2 operator/(const Vec2& a, double k) {
	return Vec2(a.x / k, a.y / k);
}
Vec2 operator+=(Vec2& a, const Vec2& b) {
	return a = a + b;
}
Vec2 operator-=(Vec2& a, const Vec2& b) {
	return a = a - b;
}
