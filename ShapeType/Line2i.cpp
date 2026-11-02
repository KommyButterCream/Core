#include "Line2i.h"
#include <cmath>

using Core::ShapeType::Line2i;

Line2i::Line2i(int32_t sx, int32_t sy, int32_t ex, int32_t ey) noexcept
	: sx(sx), sy(sy), ex(ex), ey(ey)
{
}

void Line2i::Set(int32_t sx, int32_t sy, int32_t ex, int32_t ey) noexcept
{
	this->sx = sx;
	this->sy = sy;
	this->ex = ex;
	this->ey = ey;
}

void Line2i::SetStart(int32_t x, int32_t y) noexcept
{
	sx = x;
	sy = y;
}

void Line2i::SetEnd(int32_t x, int32_t y) noexcept
{
	ex = x;
	ey = y;
}

void Line2i::Reset() noexcept
{
	sx = sy = ex = ey = 0;
}

void Line2i::Offset(int32_t dx, int32_t dy) noexcept
{
	sx += dx; sy += dy;
	ex += dx; ey += dy;
}

void Line2i::OffsetX(int32_t dx) noexcept
{
	sx += dx;
	ex += dx;
}

void Line2i::OffsetY(int32_t dy) noexcept
{
	sy += dy;
	ey += dy;
}

float Line2i::LengthSq() const noexcept
{
	const float dx = static_cast<float>(ex - sx);
	const float dy = static_cast<float>(ey - sy);
	return dx * dx + dy * dy;
}

float Line2i::Length() const noexcept
{
	return sqrtf(LengthSq());
}

float Line2i::Angle() const noexcept
{
	return static_cast<float>(
		atan2(static_cast<double>(ey - sy), static_cast<double>(ex - sx)));
}

void Line2i::Direction(int32_t& dx, int32_t& dy) const noexcept
{
	dx = ex - sx;
	dy = ey - sy;
}

void Line2i::Center(float& cx, float& cy) const noexcept
{
	cx = static_cast<float>(sx + ex) * 0.5f;
	cy = static_cast<float>(sy + ey) * 0.5f;
}
