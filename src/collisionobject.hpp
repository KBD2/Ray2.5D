#pragma once

#include <vector>

#include "vec2.hpp"
#include "wall.hpp"

class CollisionObject
{
public:
	Coords pos;
	double radius;
	Vec2 velocity;

	CollisionObject()
	{
		pos = Coords();
		radius = 100;
		velocity = Vec2();
	}

	CollisionObject(Coords _pos, double _radius)
	{
		pos = _pos;
		radius = _radius;
		velocity = Vec2();
	}

	void setVelocity(Vec2 velocity);
	void setVelocity(double xVel, double yVel);

	void updatePhysics(float fElapsedTime, std::vector<Wall> &walls);
};