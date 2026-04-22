#include "Circle2d.h"
#include "Point2d.h"
#include "Rect2d.h"

#include <cmath>

using Core::ShapeType::Circle2d;
using Core::ShapeType::Point2d;
using Core::ShapeType::Rect2d;

Circle2d::Circle2d(double cx, double cy, double r) noexcept
	: x(cx), y(cy), radius(r)
{
}

Circle2d::Circle2d(const Point2d& center, double r) noexcept
	: x(center.x), y(center.y), radius(r)
{
}

void Circle2d::Set(double cx, double cy, double r) noexcept
{
	x = cx;
	y = cy;
	radius = r;
}

void Circle2d::Set(const Point2d& center, double r) noexcept
{
	x = center.x;
	y = center.y;
	radius = r;
}

void Circle2d::Reset() noexcept
{
	x = 0.0;
	y = 0.0;
	radius = 0.0;
}

Point2d Circle2d::Center() const noexcept
{
	return Point2d{ x, y };
}

double Circle2d::Area() const noexcept
{
	constexpr double PI_VALUE = 3.14159265358979323846;

	return PI_VALUE * radius * radius;
}

bool Circle2d::Contains(double px, double py) const noexcept
{
	const double dx = px - x;
	const double dy = py - y;
	return (dx * dx + dy * dy) <= (radius * radius);
}

bool Circle2d::Contains(const Point2d& p) const noexcept
{
	return Contains(p.x, p.y);
}

Rect2d Circle2d::BoundingBoxF() const noexcept
{
	return Rect2d(
		x - radius,
		y - radius,
		x + radius,
		y + radius
	);
}