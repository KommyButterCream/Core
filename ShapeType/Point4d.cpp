#include "Point4d.h"

using Core::ShapeType::Point4d;

Point4d::Point4d(double x, double y, double z, double w) noexcept
	: x(x), y(y), z(z), w(w)
{
}

void Point4d::Set(double x, double y, double z, double w) noexcept
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

void Point4d::Reset() noexcept
{
	x = y = z = w = 0.0;
}

void Point4d::Offset(double dx, double dy, double dz, double dw) noexcept
{
	x += dx; y += dy; z += dz; w += dw;
}

Point4d Point4d::operator+(const Point4d& rhs) const noexcept
{
	return { x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w };
}

Point4d Point4d::operator-(const Point4d& rhs) const noexcept
{
	return { x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w };
}

Point4d Point4d::operator*(double scalar) const noexcept
{
	return { x * scalar, y * scalar, z * scalar, w * scalar };
}

Point4d Point4d::operator/(double scalar) const noexcept
{
	return { x / scalar, y / scalar, z / scalar, w / scalar };
}

Point4d& Point4d::operator+=(const Point4d& rhs) noexcept
{
	x += rhs.x; y += rhs.y; z += rhs.z; w += rhs.w;
	return *this;
}

Point4d& Point4d::operator-=(const Point4d& rhs) noexcept
{
	x -= rhs.x; y -= rhs.y; z -= rhs.z; w -= rhs.w;
	return *this;
}

Point4d& Point4d::operator*=(double scalar) noexcept
{
	x *= scalar; y *= scalar; z *= scalar; w *= scalar;
	return *this;
}

Point4d& Point4d::operator/=(double scalar) noexcept
{
	x /= scalar; y /= scalar; z /= scalar; w /= scalar;
	return *this;
}

bool Point4d::operator==(const Point4d& rhs) const noexcept
{
	return x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w;
}

bool Point4d::operator!=(const Point4d& rhs) const noexcept
{
	return !(*this == rhs);
}