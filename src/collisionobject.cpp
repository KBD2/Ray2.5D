#include <utility>

#include "defs.hpp"
#include "collisionobject.hpp"

void CollisionObject::setVelocity(Vec2 vel)
{
	velocity = vel;
}

void CollisionObject::setVelocity(double xVel, double yVel)
{
	velocity = Vec2(xVel, yVel);
}

void CollisionObject::updatePhysics(float fElapsedTime, std::vector<Wall> &walls)
{
	pos += velocity * (MOVE_UNITS_PER_SECOND * fElapsedTime);

	// Circle-line segment algorithm improved from https://stackoverflow.com/a/1079478

	for (auto& wall : walls)
	{
		Vec2 wallVec = wall.b - wall.a;

		Coords projection = wallVec * ((pos - wall.a).dot(wallVec) / wallVec.dot(wallVec)) + wall.a;

		Vec2 projectionVec = projection - wall.a;

		double k = std::abs(wallVec.x) > std::abs(wallVec.y) ? projectionVec.x / wallVec.x : projectionVec.y / wallVec.y;

		if (k >= 0.0 && k <= 1.0 && projection.dist(pos) < radius)
		{
			pos += (pos - projection).unit() * (radius - projection.dist(pos));
		}
	}
}