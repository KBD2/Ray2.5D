#pragma once

class Vec2
{
public:
	double x, y;

	Vec2()
	{
		x = 0.0;
		y = 0.0;
	}

	Vec2(double _x, double _y)
	{
		x = _x;
		y = _y;
	}

	Vec2 operator/(double divisor);
	Vec2 operator-(Vec2 n);
	void operator-=(Vec2 n);
	Vec2 operator+(Vec2 n);
	Vec2 operator*(double n);
	void operator*=(double n);
	void operator+=(Vec2 n);

	double length();
	Vec2 unit();
	double dist(Vec2 b);
	double dot(Vec2 b);
	double cross(Vec2 b);
	Vec2 rotate(double theta);

	bool operator==(Vec2 b);
	bool operator!=(Vec2 b);
	Vec2 operator-();
};