#include "Circle2f.h"
#include "Point2f.h"
#include "Rect2f.h"

#include <cmath>

using Core::ShapeType::Circle2f;
using Core::ShapeType::Point2f;
using Core::ShapeType::Rect2f;

Circle2f::Circle2f(float cx, float cy, float r) noexcept
	: x(cx), y(cy), radius(r)
{
}

Circle2f::Circle2f(const Point2f& center, float r) noexcept
	: x(center.x), y(center.y), radius(r)
{
}

void Circle2f::Set(float cx, float cy, float r) noexcept
{
	x = cx;
	y = cy;
	radius = r;
}

void Circle2f::Set(const Point2f& center, float r) noexcept
{
	x = center.x;
	y = center.y;
	radius = r;
}

void Circle2f::Reset() noexcept
{
	x = 0.0f;
	y = 0.0f;
	radius = 0.0f;
}

Point2f Circle2f::Center() const noexcept
{
	return Point2f{ x, y };
}

float Circle2f::Area() const noexcept
{
	constexpr double PI_VALUE = 3.14159265358979323846;

	return static_cast<float>(PI_VALUE) * radius * radius;
}

bool Circle2f::Contains(float px, float py) const noexcept
{
	const float dx = px - x;
	const float dy = py - y;
	return (dx * dx + dy * dy) <= (radius * radius);
}

bool Circle2f::Contains(const Point2f& p) const noexcept
{
	return Contains(p.x, p.y);
}

Rect2f Circle2f::BoundingBoxF() const noexcept
{
	return Rect2f(
		x - radius,
		y - radius,
		x + radius,
		y + radius
	);
}