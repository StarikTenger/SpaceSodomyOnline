#pragma once

class Color {
public:
	double r, g, b, a;
	Color() {};
	Color(double r1, double g1, double b1, double a1);
	Color(double r1, double g1, double b1);
};

Color operator+(const Color& a, const Color& b);
Color operator-(const Color& a, const Color& b);
Color operator*(const Color& a, const double& b);
Color operator/(const Color& a, const double& b);
Color fromHSV(double H, double S, double V);