#include "Color.h"
#include <cmath>

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

Color fromHSV(double H, double S, double V) {
	double C = S * V;
	double X = C * (1 - abs(fmod(H / 60.0, 2) - 1));
	double m = V - C;
	double Rs, Gs, Bs;

	if (H >= 0 && H < 60) {
		Rs = C;
		Gs = X;
		Bs = 0;
	}
	else if (H >= 60 && H < 120) {
		Rs = X;
		Gs = C;
		Bs = 0;
	}
	else if (H >= 120 && H < 180) {
		Rs = 0;
		Gs = C;
		Bs = X;
	}
	else if (H >= 180 && H < 240) {
		Rs = 0;
		Gs = X;
		Bs = C;
	}
	else if (H >= 240 && H < 300) {
		Rs = X;
		Gs = 0;
		Bs = C;
	}
	else {
		Rs = C;
		Gs = 0;
		Bs = X;
	}

	Color color;
	color.r = (Rs + m) * 255;
	color.g = (Gs + m) * 255;
	color.b = (Bs + m) * 255;
	return color;
}
