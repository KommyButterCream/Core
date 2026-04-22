#include "Point4i.h"

using Core::ShapeType::Point4i;

Point4i::Point4i(int32_t x, int32_t y, int32_t z, int32_t w) noexcept
	: x(x), y(y), z(z), w(w)
{
}

void Point4i::Set(int32_t x, int32_t y, int32_t z, int32_t w) noexcept
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

void Point4i::Reset() noexcept
{
	x = y = z = w = 0;
}

void Point4i::Offset(int32_t dx, int32_t dy, int32_t dz, int32_t dw) noexcept
{
	x += dx; y += dy; z += dz; w += dw;
}

void Point4i::OffsetX(int32_t dx) noexcept { x += dx; }
void Point4i::OffsetY(int32_t dy) noexcept { y += dy; }
void Point4i::OffsetZ(int32_t dz) noexcept { z += dz; }
void Point4i::OffsetW(int32_t dw) noexcept { w += dw; }

Point4i Point4i::operator+(const Point4i& rhs) const noexcept
{
	return { x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w };
}

Point4i Point4i::operator-(const Point4i& rhs) const noexcept
{
	return { x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w };
}

Point4i Point4i::operator*(int32_t scalar) const noexcept
{
	return { x * scalar, y * scalar, z * scalar, w * scalar };
}

Point4i Point4i::operator/(int32_t scalar) const noexcept
{
	return { x / scalar, y / scalar, z / scalar, w / scalar };
}

Point4i& Point4i::operator+=(const Point4i& rhs) noexcept
{
	x += rhs.x; y += rhs.y; z += rhs.z; w += rhs.w;
	return *this;
}

Point4i& Point4i::operator-=(const Point4i& rhs) noexcept
{
	x -= rhs.x; y -= rhs.y; z -= rhs.z; w -= rhs.w;
	return *this;
}

Point4i& Point4i::operator*=(int32_t scalar) noexcept
{
	x *= scalar; y *= scalar; z *= scalar; w *= scalar;
	return *this;
}

Point4i& Point4i::operator/=(int32_t scalar) noexcept
{
	x /= scalar; y /= scalar; z /= scalar; w /= scalar;
	return *this;
}

bool Point4i::operator==(const Point4i& rhs) const noexcept
{
	return x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w;
}

bool Point4i::operator!=(const Point4i& rhs) const noexcept
{
	return !(*this == rhs);
}