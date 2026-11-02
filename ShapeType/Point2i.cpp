#include "Point2i.h"

using Core::ShapeType::Point2i;

Point2i::Point2i(int32_t x, int32_t y) noexcept
	: x(x), y(y)
{
}

void Point2i::Set(int32_t x, int32_t y) noexcept
{
	this->x = x;
	this->y = y;
}

void Point2i::Reset() noexcept
{
	x = y =  0;
}

void Point2i::Offset(int32_t dx, int32_t dy) noexcept
{
	x += dx; y += dy;
}

void Point2i::OffsetX(int32_t dx) noexcept { x += dx; }
void Point2i::OffsetY(int32_t dy) noexcept { y += dy; }

Point2i Point2i::operator+(const Point2i& rhs) const noexcept
{
	return { x + rhs.x, y + rhs.y };
}

Point2i Point2i::operator-(const Point2i& rhs) const noexcept
{
	return { x - rhs.x, y - rhs.y };
}

Point2i Point2i::operator*(int32_t scalar) const noexcept
{
	return { x * scalar, y * scalar };
}

Point2i Point2i::operator/(int32_t scalar) const noexcept
{
	return { x / scalar, y / scalar };
}

Point2i& Point2i::operator+=(const Point2i& rhs) noexcept
{
	x += rhs.x; y += rhs.y;
	return *this;
}

Point2i& Point2i::operator-=(const Point2i& rhs) noexcept
{
	x -= rhs.x; y -= rhs.y;
	return *this;
}

Point2i& Point2i::operator*=(int32_t scalar) noexcept
{
	x *= scalar; y *= scalar;
	return *this;
}

Point2i& Point2i::operator/=(int32_t scalar) noexcept
{
	x /= scalar; y /= scalar;
	return *this;
}

bool Point2i::operator==(const Point2i& rhs) const noexcept
{
	return x == rhs.x && y == rhs.y;
}

bool Point2i::operator!=(const Point2i& rhs) const noexcept
{
	return !(*this == rhs);
}