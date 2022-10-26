#include <utility>

#include "wall.hpp"

CollisionResult Wall::getCollision(Coords origin, Vec2 ray)
{
	CollisionResult ret = {
		false,
		Coords(),
		0.0,
		NULL
	};

	Vec2 v1 = origin - a;
	Vec2 v2 = b - a;
	Vec2 v3 = Vec2(-ray.y, ray.x);

	double t1 = v2.cross(v1) / v2.dot(v3);
	double t2 = v1.dot(v3) / v2.dot(v3);

	if (t1 >= 0.0 && t2 >= 0.0 && t2 <= 1.0)
	{
		ret.didCollide = true;
		ret.collisionPoint = a + v2 * t2;
		ret.texturePosition = t2;
		ret.sprite = sprite;
	}
	return ret;
}