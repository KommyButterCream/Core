#include "Ellipse2f.h"
#include "Point2f.h"
#include "Rect2f.h"

#include <cmath>
#include <algorithm>

using Core::ShapeType::Ellipse2f;
using Core::ShapeType::Point2f;
using Core::ShapeType::Rect2f;

Ellipse2f::Ellipse2f(float cx, float cy, float rx, float ry, float angle) noexcept
	: x(cx), y(cy), radiusX(rx), radiusY(ry), angleRad(angle)
{

}

Ellipse2f::Ellipse2f(const Point2f& center, float rx, float ry,	float angle) noexcept
	: x(center.x), y(center.y), radiusX(rx), radiusY(ry), angleRad(angle)
{
}

void Ellipse2f::Set(float cx, float cy,	float rx, float ry,	float angle) noexcept
{
	x = cx;
	y = cy;
	radiusX = rx;
	radiusY = ry;
	angleRad = angle;
}

void Ellipse2f::Set(const Point2f& center, float rx, float ry, float angle) noexcept
{
	x = center.x;
	y = center.y;
	radiusX = rx;
	radiusY = ry;
	angleRad = angle;
}

void Ellipse2f::Reset() noexcept
{
	x = 0.0f;
	y = 0.0f;
	radiusX = 0.0f;
	radiusY = 0.0f;
	angleRad = 0.0f;
}

Point2f Ellipse2f::Center() const noexcept
{
	return Point2f{ x, y };
}

float Ellipse2f::Area() const noexcept
{
	constexpr double PI_VALUE = 3.14159265358979323846;

	return static_cast<float>(PI_VALUE) * radiusX * radiusY;
}

bool Ellipse2f::Contains(float px, float py) const noexcept
{
	if (radiusX <= 0.0f || radiusY <= 0.0f)
		return false;

	const float dx = px - x;
	const float dy = py - y;

	const float c = std::cosf(angleRad);
	const float s = std::sinf(angleRad);

	// inverse rotation
	const float lx = dx * c + dy * s;
	const float ly = -dx * s + dy * c;

	const float nx = lx / radiusX;
	const float ny = ly / radiusY;

	return (nx * nx + ny * ny) <= 1.0f;
}

bool Ellipse2f::Contains(const Point2f& p) const noexcept
{
	return Contains(p.x, p.y);
}

Rect2f Ellipse2f::BoundingBoxF() const noexcept
{
	if (radiusX <= 0.0f || radiusY <= 0.0f)
		return Rect2f(x, y, x, y);

	const float c = std::cosf(angleRad);
	const float s = std::sinf(angleRad);

	// ellipse extreme extents after rotation
	const float ex = std::abs(radiusX * c) + std::abs(radiusY * s);
	const float ey = std::abs(radiusX * s) + std::abs(radiusY * c);

	return Rect2f(
		x - ex, y - ey,
		x + ex, y + ey
	);
}