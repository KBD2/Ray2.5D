#pragma once

#include "vec2.hpp"

class Wall
{
public:
	Coords a;
	Coords b;

	int red, green, blue;

	Wall()
	{
		a = Coords(0, 0);
		b = Coords(0, 0);
		red = 0;
		green = 0;
		blue = 0;
	}

	Wall(Coords _a, Coords _b, int _red, int _green, int _blue)
	{
		a = _a;
		b = _b;
		red = _red;
		green = _green;
		blue = _blue;
	}

	bool getCollision(Coords origin, Vec2 ray, Coords &collision);
};