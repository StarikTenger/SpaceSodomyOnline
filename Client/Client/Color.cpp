#include "Color.h"
#include <algorithm>

Color::Color(double r1, double g1, double b1, double a1) {
	r = r1;
	g = g1;
	b = b1;
	a = a1;
}
Color::Color(double r1, double g1, double b1) {
	r = r1;
	g = g1;
	b = b1;
	a = 255;
}
Color operator+(const Color& a, const Color& b) {
	return Color(a.r + b.r, a.g + b.g, a.b + b.b, a.a + b.a);
}
Color operator-(const Color& a, const Color& b) {
	return Color(a.r - b.r, a.g - b.g, a.b - b.b, a.a - b.a);
}
Color operator*(const Color& a, const double& b) {
	return Color(a.r * b, a.g * b, a.b * b, a.a * b);
}
Color operator/(const Color& a, const double& b) {
	return Color(a.r / b, a.g / b, a.b / b, a.a / b);
}