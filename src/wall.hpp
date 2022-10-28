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

	CollisionResult getCollision(Coords origin, Vec2 ray);
};