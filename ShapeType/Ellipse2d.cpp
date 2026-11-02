#include "Ellipse2d.h"
#include "Point2d.h"
#include "Rect2d.h"

#include <cmath>
#include <algorithm>

using Core::ShapeType::Ellipse2d;
using Core::ShapeType::Point2d;
using Core::ShapeType::Rect2d;

Ellipse2d::Ellipse2d(double cx, double cy, double rx, double ry, double angle) noexcept
	: x(cx), y(cy), radiusX(rx), radiusY(ry), angleRad(angle)
{

}

Ellipse2d::Ellipse2d(const Point2d& center, double rx, double ry, double angle) noexcept
	: x(center.x), y(center.y), radiusX(rx), radiusY(ry), angleRad(angle)
{
}

void Ellipse2d::Set(double cx, double cy, double rx, double ry, double angle) noexcept
{
	x = cx;
	y = cy;
	radiusX = rx;
	radiusY = ry;
	angleRad = angle;
}

void Ellipse2d::Set(const Point2d& center, double rx, double ry, double angle) noexcept
{
	x = center.x;
	y = center.y;
	radiusX = rx;
	radiusY = ry;
	angleRad = angle;
}

void Ellipse2d::Reset() noexcept
{
	x = 0.0;
	y = 0.0;
	radiusX = 0.0;
	radiusY = 0.0;
	angleRad = 0.0;
}

Point2d Ellipse2d::Center() const noexcept
{
	return Point2d{ x, y };
}

double Ellipse2d::Area() const noexcept
{
	constexpr double PI_VALUE = 3.14159265358979323846;

	return PI_VALUE * radiusX * radiusY;
}

bool Ellipse2d::Contains(double px, double py) const noexcept
{
	if (radiusX <= 0.0 || radiusY <= 0.0)
		return false;

	const double dx = px - x;
	const double dy = py - y;

	const double c = std::cos(angleRad);
	const double s = std::sin(angleRad);

	// inverse rotation
	const double lx = dx * c + dy * s;
	const double ly = -dx * s + dy * c;

	const double nx = lx / radiusX;
	const double ny = ly / radiusY;

	return (nx * nx + ny * ny) <= 1.0;
}

bool Ellipse2d::Contains(const Point2d& p) const noexcept
{
	return Contains(p.x, p.y);
}

Rect2d Ellipse2d::BoundingBoxF() const noexcept
{
	if (radiusX <= 0.0 || radiusY <= 0.0)
		return Rect2d(x, y, x, y);

	const double c = std::cos(angleRad);
	const double s = std::sin(angleRad);

	// ellipse extreme extents after rotation
	const double ex = std::abs(radiusX * c) + std::abs(radiusY * s);
	const double ey = std::abs(radiusX * s) + std::abs(radiusY * c);

	return Rect2d(
		x - ex, y - ey,
		x + ex, y + ey
	);
}