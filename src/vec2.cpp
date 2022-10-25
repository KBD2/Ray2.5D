#include <cmath>

#include "vec2.hpp"

Vec2 Vec2::operator/(double divisor)
{
	return Vec2(x / divisor, y / divisor);
}

Vec2 Vec2::operator-(Vec2 n)
{
	return Vec2(x - n.x, y - n.y);
}

Vec2 Vec2::operator+(Vec2 n)
{
	return Vec2(x + n.x, y + n.y);
}

void Vec2::operator+=(Vec2 n)
{
	*this = *this + n;
}

double Vec2::length()
{
	return std::sqrt(std::pow(x, 2) + std::pow(y, 2));
}
Vec2 Vec2::unit()
{
	return *this / length();
}

double Vec2::dist(Vec2 b)
{
	return std::sqrt(std::pow(b.x - x, 2) + std::pow(b.y - y, 2));
}

Vec2 Vec2::operator*(double n)
{
	return Vec2(x * n, y * n);
}

void Vec2::operator*=(double n)
{
	x *= n;
	y *= n;
}

double Vec2::dot(Vec2 b)
{
	return x * b.x + y * b.y;
}

double Vec2::cross(Vec2 b)
{
	return x * b.y - b.x * y;
}

Vec2 Vec2::rotate(double theta)
{
	double cost = std::cos(theta);
	double sint = std::sin(theta);
	return Vec2(
		cost * x - sint * y,
		sint * x + cost * y
	);
}

bool Vec2::operator==(Vec2 b)
{
	return x == b.x && y == b.y;
}

bool Vec2::operator!=(Vec2 b)
{
	return x != b.x || y != b.y;
}

Vec2 Vec2::operator-()
{
	return Vec2(-x, -y);
}

void Vec2::operator-=(Vec2 n)
{
	x -= n.x;
	y -= n.y;
}