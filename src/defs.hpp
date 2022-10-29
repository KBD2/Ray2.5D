#pragma once

#include "vec2.hpp"
#include "olcPixelGameEngine.h"

struct CollisionResult {
	bool didCollide;
	Coords collisionPoint;
	double texturePosition;
	olc::Sprite* sprite;
};

const float MOVE_UNITS_PER_SECOND = 400.0f;
const int MAX_DRAW_DISTANCE = 2500;