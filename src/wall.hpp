#pragma once

#include "vec2.hpp"

class Wall
{
public:
	Vec2 a;
	Vec2 b;

	int red, green, blue;

	Wall()
	{
		a = Vec2(0, 0);
		b = Vec2(0, 0);
		red = 0;
		green = 0;
		blue = 0;
	}

	Wall(Vec2 _a, Vec2 _b, int _red, int _green, int _blue)
	{
		a = _a;
		b = _b;
		red = _red;
		green = _green;
		blue = _blue;
	}

	double getCollision(Vec2 origin, Vec2 ray);
};