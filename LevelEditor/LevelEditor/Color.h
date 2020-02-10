#pragma once

class Color {
public:
	int r, g, b, a;
	Color() {};
	Color(int r1, int g1, int b1, int a1);
	Color(int r1, int g1, int b1);
};

Color operator+(const Color& a, const Color& b);
Color operator-(const Color& a, const Color& b);
Color operator*(const Color& a, const double& b);
Color operator/(const Color& a, const double& b);