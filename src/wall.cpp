#include <utility>

#include "wall.hpp"

bool Wall::getCollision(Coords origin, Vec2 ray, Coords &collision)
{
	Vec2 v1 = origin - a;
	Vec2 v2 = b - a;
	Vec2 v3 = Vec2(-ray.y, ray.x);

	double t1 = v2.cross(v1) / v2.dot(v3);
	double t2 = v1.dot(v3) / v2.dot(v3);

	if (t1 >= 0.0 && t2 >= 0.0 && t2 <= 1.0)
	{
		collision = a + v2 * t2;
		return true;
	}
	return false;
}