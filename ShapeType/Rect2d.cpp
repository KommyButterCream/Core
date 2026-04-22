#include "Rect2d.h"
#include "Point2d.h"
#include "Size2d.h"

#include <algorithm>

using Core::ShapeType::Point2d;
using Core::ShapeType::Rect2d;
using Core::ShapeType::Size2d;

Rect2d::Rect2d(double l, double t, double r, double b) noexcept
	: left(l), top(t), right(r), bottom(b)
{
}

void Rect2d::Set(double l, double t, double r, double b) noexcept
{
	left = l;
	top = t;
	right = r;
	bottom = b;
}

void Rect2d::Reset() noexcept
{
	left = top = right = bottom = 0.0;
}

double Rect2d::Width() const noexcept
{
	return right - left;
}

double Rect2d::Height() const noexcept
{
	return bottom - top;
}

Size2d Rect2d::Size() const noexcept
{
	return { Width(), Height() };
}

double Rect2d::Area() const noexcept
{
	return Empty() ? 0.0 : Width() * Height();
}

bool Rect2d::Empty() const noexcept
{
	return left >= right || top >= bottom;
}

Point2d Rect2d::Center() const noexcept
{
	return {
		(left + right) * 0.5f,
		(top + bottom) * 0.5f
	};
}

void Rect2d::Normalize() noexcept
{
	if (left > right) std::swap(left, right);
	if (top > bottom) std::swap(top, bottom);
}

bool Rect2d::Contains(double x, double y) const noexcept
{
	return x >= left && x <= right &&
		y >= top && y <= bottom;
}

bool Rect2d::Contains(const Point2d& p) const noexcept
{
	return Contains(p.x, p.y);
}

bool Rect2d::Contains(const Rect2d& r) const noexcept
{
	return r.left >= left && r.right <= right &&
		r.top >= top && r.bottom <= bottom;
}

bool Rect2d::Intersects(const Rect2d& r) const noexcept
{
	return !(r.right <= left || r.left >= right ||
		r.bottom <= top || r.top >= bottom);
}

Rect2d Rect2d::Intersect(const Rect2d& r) const noexcept
{
	Rect2d out{
		std::max(left, r.left),
		std::max(top, r.top),
		std::min(right, r.right),
		std::min(bottom, r.bottom)
	};

	if (out.Empty())
		out.Reset();

	return out;
}

Rect2d Rect2d::Unite(const Rect2d& r) const noexcept
{
	if (Empty()) return r;
	if (r.Empty()) return *this;

	return {
		std::min(left, r.left),
		std::min(top, r.top),
		std::max(right, r.right),
		std::max(bottom, r.bottom)
	};
}

Rect2d& Rect2d::IntersectWith(const Rect2d& r) noexcept
{
	*this = Intersect(r);
	return *this;
}

Rect2d& Rect2d::UniteWith(const Rect2d& r) noexcept
{
	*this = Unite(r);
	return *this;
}

void Rect2d::Offset(double dx, double dy) noexcept
{
	left += dx; right += dx;
	top += dy;  bottom += dy;
}

void Rect2d::Inflate(double dx, double dy) noexcept
{
	left -= dx; right += dx;
	top -= dy;  bottom += dy;
}

void Rect2d::Deflate(double dx, double dy) noexcept
{
	Inflate(-dx, -dy);
}

void Rect2d::ScaleFromCenter(double sx, double sy) noexcept
{
	const auto c = Center();
	const double hw = Width() * 0.5f * sx;
	const double hh = Height() * 0.5f * sy;

	left = c.x - hw;
	right = c.x + hw;
	top = c.y - hh;
	bottom = c.y + hh;
}

void Rect2d::MoveCenterTo(double cx, double cy) noexcept
{
	const auto c = Center();
	Offset(cx - c.x, cy - c.y);
}

Rect2d Rect2d::operator&(const Rect2d& r) const noexcept
{
	return Intersect(r);
}

Rect2d Rect2d::operator|(const Rect2d& r) const noexcept
{
	return Unite(r);
}

Rect2d& Rect2d::operator&=(const Rect2d& r) noexcept
{
	return IntersectWith(r);
}

Rect2d& Rect2d::operator|=(const Rect2d& r) noexcept
{
	return UniteWith(r);
}

bool Rect2d::operator==(const Rect2d& r) const noexcept
{
	return left == r.left && top == r.top &&
		right == r.right && bottom == r.bottom;
}

bool Rect2d::operator!=(const Rect2d& r) const noexcept
{
	return !(*this == r);
}
