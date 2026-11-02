#include "Line2f.h"
#include <cmath>

using Core::ShapeType::Line2f;

Line2f::Line2f(float sx, float sy, float ex, float ey) noexcept
	: sx(sx), sy(sy), ex(ex), ey(ey)
{
}

void Line2f::Set(float sx, float sy, float ex, float ey) noexcept
{
	this->sx = sx;
	this->sy = sy;
	this->ex = ex;
	this->ey = ey;
}

void Line2f::SetStart(float x, float y) noexcept
{
	sx = x;
	sy = y;
}

void Line2f::SetEnd(float x, float y) noexcept
{
	ex = x;
	ey = y;
}

void Line2f::Reset() noexcept
{
	sx = sy = ex = ey = 0.0f;
}

void Line2f::Offset(float dx, float dy) noexcept
{
	sx += dx; sy += dy;
	ex += dx; ey += dy;
}

void Line2f::OffsetX(float dx) noexcept
{
	sx += dx;
	ex += dx;
}

void Line2f::OffsetY(float dy) noexcept
{
	sy += dy;
	ey += dy;
}

float Line2f::LengthSq() const noexcept
{
	const float dx = ex - sx;
	const float dy = ey - sy;
	return dx * dx + dy * dy;
}

float Line2f::Length() const noexcept
{
	return sqrtf(LengthSq());
}

float Line2f::Angle() const noexcept
{
	return static_cast<float>(
		atan2(static_cast<double>(ey - sy), static_cast<double>(ex - sx)));
}

void Line2f::Direction(float& dx, float& dy) const noexcept
{
	dx = ex - sx;
	dy = ey - sy;
}

void Line2f::Center(float& cx, float& cy) const noexcept
{
	cx = (sx + ex) * 0.5f;
	cy = (sy + ey) * 0.5f;
}
