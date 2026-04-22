#include "Point2d.h"

using Core::ShapeType::Point2d;

Point2d::Point2d(double x, double y) noexcept
	: x(x), y(y)
{
}

void Point2d::Set(double x, double y) noexcept
{
	this->x = x;
	this->y = y;
}

void Point2d::Reset() noexcept
{
	x = y = 0;
}

void Point2d::Offset(double dx, double dy) noexcept
{
	x += dx; y += dy;
}

void Point2d::OffsetX(double dx) noexcept { x += dx; }
void Point2d::OffsetY(double dy) noexcept { y += dy; }

Point2d Point2d::operator+(const Point2d& rhs) const noexcept
{
	return { x + rhs.x, y + rhs.y };
}

Point2d Point2d::operator-(const Point2d& rhs) const noexcept
{
	return { x - rhs.x, y - rhs.y };
}

Point2d Point2d::operator*(double scalar) const noexcept
{
	return { x * scalar, y * scalar };
}

Point2d Point2d::operator/(double scalar) const noexcept
{
	return { x / scalar, y / scalar };
}

Point2d& Point2d::operator+=(const Point2d& rhs) noexcept
{
	x += rhs.x; y += rhs.y;
	return *this;
}

Point2d& Point2d::operator-=(const Point2d& rhs) noexcept
{
	x -= rhs.x; y -= rhs.y;
	return *this;
}

Point2d& Point2d::operator*=(double scalar) noexcept
{
	x *= scalar; y *= scalar;
	return *this;
}

Point2d& Point2d::operator/=(double scalar) noexcept
{
	x /= scalar; y /= scalar;
	return *this;
}

bool Point2d::operator==(const Point2d& rhs) const noexcept
{
	return x == rhs.x && y == rhs.y;
}

bool Point2d::operator!=(const Point2d& rhs) const noexcept
{
	return !(*this == rhs);
}