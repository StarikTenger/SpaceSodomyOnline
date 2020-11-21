#pragma once
class Vec2 {
public:
	double x=0, y=0;
	Vec2() {};
	Vec2(double x1, double y1);
};

Vec2 operator+(const Vec2& a, const Vec2& b);
Vec2 operator-(const Vec2& a, const Vec2& b);
Vec2 operator*(const Vec2& a, double k);
Vec2 operator/(const Vec2& a, double k);
Vec2 operator+=(Vec2& a, const Vec2& b);
Vec2 operator-=(Vec2& a, const Vec2& b);
double operator*(const Vec2& a, const Vec2& b);