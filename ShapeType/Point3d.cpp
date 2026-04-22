#include "Point3d.h"

using Core::ShapeType::Point3d;

Point3d::Point3d(double x, double y, double z) noexcept
	: x(x), y(y), z(z)
{
}

void Point3d::Set(double x, double y, double z) noexcept
{
	this->x = x;
	this->y = y;
	this->z = z;
}

void Point3d::Reset() noexcept
{
	x = y = z = 0;
}

void Point3d::Offset(double dx, double dy, double dz) noexcept
{
	x += dx; y += dy; z += dz;
}

void Point3d::OffsetX(double dx) noexcept { x += dx; }
void Point3d::OffsetY(double dy) noexcept { y += dy; }
void Point3d::OffsetZ(double dz) noexcept { z += dz; }

Point3d Point3d::operator+(const Point3d& rhs) const noexcept
{
	return { x + rhs.x, y + rhs.y, z + rhs.z };
}

Point3d Point3d::operator-(const Point3d& rhs) const noexcept
{
	return { x - rhs.x, y - rhs.y, z - rhs.z };
}

Point3d Point3d::operator*(double scalar) const noexcept
{
	return { x * scalar, y * scalar, z * scalar };
}

Point3d Point3d::operator/(double scalar) const noexcept
{
	return { x / scalar, y / scalar, z / scalar };
}

Point3d& Point3d::operator+=(const Point3d& rhs) noexcept
{
	x += rhs.x; y += rhs.y; z += rhs.z;
	return *this;
}

Point3d& Point3d::operator-=(const Point3d& rhs) noexcept
{
	x -= rhs.x; y -= rhs.y; z -= rhs.z;
	return *this;
}

Point3d& Point3d::operator*=(double scalar) noexcept
{
	x *= scalar; y *= scalar; z *= scalar;
	return *this;
}

Point3d& Point3d::operator/=(double scalar) noexcept
{
	x /= scalar; y /= scalar; z /= scalar;
	return *this;
}

bool Point3d::operator==(const Point3d& rhs) const noexcept
{
	return x == rhs.x && y == rhs.y && z == rhs.z;
}

bool Point3d::operator!=(const Point3d& rhs) const noexcept
{
	return !(*this == rhs);
}