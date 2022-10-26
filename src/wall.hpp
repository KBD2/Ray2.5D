#pragma once

#include "vec2.hpp"
#include "olcPixelGameEngine.h"

struct CollisionResult {
	bool didCollide;
	Coords collisionPoint;
	double texturePosition;
	olc::Sprite *sprite;
};

class Wall
{
public:
	Coords a;
	Coords b;

	olc::Sprite *sprite;

	Wall()
	{
		a = Coords(0, 0);
		b = Coords(0, 0);
		sprite = NULL;
	}

	Wall(Coords _a, Coords _b, olc::Sprite *_sprite)
	{
		a = _a;
		b = _b;
		sprite = _sprite;
	}

	CollisionResult getCollision(Coords origin, Vec2 ray);
};