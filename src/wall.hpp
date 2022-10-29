#pragma once

#include "vec2.hpp"
#include "olcPixelGameEngine.h"
#include "defs.hpp"

class Wall
{
public:
	Coords a;
	Coords b;

	olc::Sprite *sprite;

	CollisionResult getCollision(Coords origin, Vec2 ray);
};