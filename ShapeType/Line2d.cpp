#include "Line2d.h"
#include <cmath>

using Core::ShapeType::Line2d;

Line2d::Line2d(double sx, double sy, double ex, double ey) noexcept
	: sx(sx), sy(sy), ex(ex), ey(ey)
{
}

void Line2d::Set(double sx, double sy, double ex, double ey) noexcept
{
	this->sx = sx;
	this->sy = sy;
	this->ex = ex;
	this->ey = ey;
}

void Line2d::SetStart(double x, double y) noexcept
{
	sx = x;
	sy = y;
}

void Line2d::SetEnd(double x, double y) noexcept
{
	ex = x;
	ey = y;
}

void Line2d::Reset() noexcept
{
	sx = sy = ex = ey = 0.0;
}

void Line2d::Offset(double dx, double dy) noexcept
{
	sx += dx; sy += dy;
	ex += dx; ey += dy;
}

void Line2d::OffsetX(double dx) noexcept
{
	sx += dx;
	ex += dx;
}

void Line2d::OffsetY(double dy) noexcept
{
	sy += dy;
	ey += dy;
}

double Line2d::LengthSq() const noexcept
{
	const double dx = ex - sx;
	const double dy = ey - sy;
	return dx * dx + dy * dy;
}

double Line2d::Length() const noexcept
{
	return sqrt(LengthSq());
}

double Line2d::Angle() const noexcept
{
	return atan2(ey - sy, ex - sx);
}

void Line2d::Direction(double& dx, double& dy) const noexcept
{
	dx = ex - sx;
	dy = ey - sy;
}

void Line2d::Center(double& cx, double& cy) const noexcept
{
	cx = (sx + ex) * 0.5;
	cy = (sy + ey) * 0.5;
}
