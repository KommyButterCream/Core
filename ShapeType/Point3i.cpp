#include "Point3i.h"

using Core::ShapeType::Point3i;

Point3i::Point3i(int32_t x, int32_t y, int32_t z) noexcept
	: x(x), y(y), z(z)
{
}

void Point3i::Set(int32_t x, int32_t y, int32_t z) noexcept
{
	this->x = x;
	this->y = y;
	this->z = z;
}

void Point3i::Reset() noexcept
{
	x = y = z = 0;
}

void Point3i::Offset(int32_t dx, int32_t dy, int32_t dz) noexcept
{
	x += dx; y += dy; z += dz;
}

void Point3i::OffsetX(int32_t dx) noexcept { x += dx; }
void Point3i::OffsetY(int32_t dy) noexcept { y += dy; }
void Point3i::OffsetZ(int32_t dz) noexcept { z += dz; }

Point3i Point3i::operator+(const Point3i& rhs) const noexcept
{
	return { x + rhs.x, y + rhs.y, z + rhs.z };
}

Point3i Point3i::operator-(const Point3i& rhs) const noexcept
{
	return { x - rhs.x, y - rhs.y, z - rhs.z };
}

Point3i Point3i::operator*(int32_t scalar) const noexcept
{
	return { x * scalar, y * scalar, z * scalar };
}

Point3i Point3i::operator/(int32_t scalar) const noexcept
{
	return { x / scalar, y / scalar, z / scalar };
}

Point3i& Point3i::operator+=(const Point3i& rhs) noexcept
{
	x += rhs.x; y += rhs.y; z += rhs.z;
	return *this;
}

Point3i& Point3i::operator-=(const Point3i& rhs) noexcept
{
	x -= rhs.x; y -= rhs.y; z -= rhs.z;
	return *this;
}

Point3i& Point3i::operator*=(int32_t scalar) noexcept
{
	x *= scalar; y *= scalar; z *= scalar;
	return *this;
}

Point3i& Point3i::operator/=(int32_t scalar) noexcept
{
	x /= scalar; y /= scalar; z /= scalar;
	return *this;
}

bool Point3i::operator==(const Point3i& rhs) const noexcept
{
	return x == rhs.x && y == rhs.y && z == rhs.z;
}

bool Point3i::operator!=(const Point3i& rhs) const noexcept
{
	return !(*this == rhs);
}