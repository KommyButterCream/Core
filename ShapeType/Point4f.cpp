#include "Point4f.h"

using Core::ShapeType::Point4f;

Point4f::Point4f(float x, float y, float z, float w) noexcept
	: x(x), y(y), z(z), w(w)
{
}

void Point4f::Set(float x, float y, float z, float w) noexcept
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

void Point4f::Reset() noexcept
{
	x = y = z = w = 0.0f;
}

void Point4f::Offset(float dx, float dy, float dz, float dw) noexcept
{
	x += dx;
	y += dy;
	z += dz;
	w += dw;
}

void Point4f::OffsetX(float dx) noexcept { x += dx; }
void Point4f::OffsetY(float dy) noexcept { y += dy; }
void Point4f::OffsetZ(float dz) noexcept { z += dz; }
void Point4f::OffsetW(float dw) noexcept { w += dw; }

Point4f Point4f::operator+(const Point4f& rhs) const noexcept
{
	return { x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w };
}

Point4f Point4f::operator-(const Point4f& rhs) const noexcept
{
	return { x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w };
}

Point4f Point4f::operator*(float scalar) const noexcept
{
	return { x * scalar, y * scalar, z * scalar, w * scalar };
}

Point4f Point4f::operator/(float scalar) const noexcept
{
	float inv = 1.0f / scalar;
	return { x * inv, y * inv, z * inv, w * inv };
}

Point4f& Point4f::operator+=(const Point4f& rhs) noexcept
{
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	w += rhs.w;
	return *this;
}

Point4f& Point4f::operator-=(const Point4f& rhs) noexcept
{
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;
	w -= rhs.w;
	return *this;
}

Point4f& Point4f::operator*=(float scalar) noexcept
{
	x *= scalar;
	y *= scalar;
	z *= scalar;
	w *= scalar;
	return *this;
}

Point4f& Point4f::operator/=(float scalar) noexcept
{
	float inv = 1.0f / scalar;
	x *= inv;
	y *= inv;
	z *= inv;
	w *= inv;
	return *this;
}

bool Point4f::operator==(const Point4f& rhs) const noexcept
{
	return x == rhs.x &&
		y == rhs.y &&
		z == rhs.z &&
		w == rhs.w;
}

bool Point4f::operator!=(const Point4f& rhs) const noexcept
{
	return !(*this == rhs);
}