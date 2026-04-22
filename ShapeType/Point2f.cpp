#include "Point2f.h"

using Core::ShapeType::Point2f;

Point2f::Point2f(float x, float y) noexcept
	: x(x), y(y)
{
}

void Point2f::Set(float x, float y) noexcept
{
	this->x = x;
	this->y = y;
}

void Point2f::Reset() noexcept
{
	x = y = 0;
}

void Point2f::Offset(float dx, float dy) noexcept
{
	x += dx; y += dy;
}

void Point2f::OffsetX(float dx) noexcept { x += dx; }
void Point2f::OffsetY(float dy) noexcept { y += dy; }

Point2f Point2f::operator+(const Point2f& rhs) const noexcept
{
	return { x + rhs.x, y + rhs.y };
}

Point2f Point2f::operator-(const Point2f& rhs) const noexcept
{
	return { x - rhs.x, y - rhs.y };
}

Point2f Point2f::operator*(float scalar) const noexcept
{
	return { x * scalar, y * scalar };
}

Point2f Point2f::operator/(float scalar) const noexcept
{
	return { x / scalar, y / scalar };
}

Point2f& Point2f::operator+=(const Point2f& rhs) noexcept
{
	x += rhs.x; y += rhs.y;
	return *this;
}

Point2f& Point2f::operator-=(const Point2f& rhs) noexcept
{
	x -= rhs.x; y -= rhs.y;
	return *this;
}

Point2f& Point2f::operator*=(float scalar) noexcept
{
	x *= scalar; y *= scalar;
	return *this;
}

Point2f& Point2f::operator/=(float scalar) noexcept
{
	x /= scalar; y /= scalar;
	return *this;
}

bool Point2f::operator==(const Point2f& rhs) const noexcept
{
	return x == rhs.x && y == rhs.y;
}

bool Point2f::operator!=(const Point2f& rhs) const noexcept
{
	return !(*this == rhs);
}