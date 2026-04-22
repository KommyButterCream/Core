#include "Point3f.h"

using Core::ShapeType::Point3f;

Point3f::Point3f(float x, float y, float z) noexcept
	: x(x), y(y), z(z)
{
}

void Point3f::Set(float x, float y, float z) noexcept
{
	this->x = x;
	this->y = y;
	this->z = z;
}

void Point3f::Reset() noexcept
{
	x = y = z = 0;
}

void Point3f::Offset(float dx, float dy, float dz) noexcept
{
	x += dx; y += dy; z += dz;
}

void Point3f::OffsetX(float dx) noexcept { x += dx; }
void Point3f::OffsetY(float dy) noexcept { y += dy; }
void Point3f::OffsetZ(float dz) noexcept { z += dz; }

Point3f Point3f::operator+(const Point3f& rhs) const noexcept
{
	return { x + rhs.x, y + rhs.y, z + rhs.z };
}

Point3f Point3f::operator-(const Point3f& rhs) const noexcept
{
	return { x - rhs.x, y - rhs.y, z - rhs.z };
}

Point3f Point3f::operator*(float scalar) const noexcept
{
	return { x * scalar, y * scalar, z * scalar };
}

Point3f Point3f::operator/(float scalar) const noexcept
{
	return { x / scalar, y / scalar, z / scalar };
}

Point3f& Point3f::operator+=(const Point3f& rhs) noexcept
{
	x += rhs.x; y += rhs.y; z += rhs.z;
	return *this;
}

Point3f& Point3f::operator-=(const Point3f& rhs) noexcept
{
	x -= rhs.x; y -= rhs.y; z -= rhs.z;
	return *this;
}

Point3f& Point3f::operator*=(float scalar) noexcept
{
	x *= scalar; y *= scalar; z *= scalar;
	return *this;
}

Point3f& Point3f::operator/=(float scalar) noexcept
{
	x /= scalar; y /= scalar; z /= scalar;
	return *this;
}

bool Point3f::operator==(const Point3f& rhs) const noexcept
{
	return x == rhs.x && y == rhs.y && z == rhs.z;
}

bool Point3f::operator!=(const Point3f& rhs) const noexcept
{
	return !(*this == rhs);
}