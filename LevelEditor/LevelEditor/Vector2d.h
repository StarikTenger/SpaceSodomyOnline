#pragma once
class Vector2d {
public:
	double x=0, y=0;
	Vector2d() {};
	Vector2d(double x1, double y1);
};

Vector2d operator+(const Vector2d& a, const Vector2d& b);
Vector2d operator-(const Vector2d& a, const Vector2d& b);
Vector2d operator*(const Vector2d& a, double k);
Vector2d operator/(const Vector2d& a, double k);
Vector2d operator+=(Vector2d& a, const Vector2d& b);
Vector2d operator-=(Vector2d& a, const Vector2d& b);