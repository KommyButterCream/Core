#include "Rect2f.h"
#include "Point2f.h"
#include "Size2f.h"

#include <algorithm>

using Core::ShapeType::Point2f;
using Core::ShapeType::Rect2f;
using Core::ShapeType::Size2f;

Rect2f::Rect2f(float l, float t, float r, float b) noexcept
	: left(l), top(t), right(r), bottom(b)
{
}

void Rect2f::Set(float l, float t, float r, float b) noexcept
{
	left = l;
	top = t;
	right = r;
	bottom = b;
}

void Rect2f::Reset() noexcept
{
	left = top = right = bottom = 0.0f;
}

float Rect2f::Width() const noexcept
{
	return right - left;
}

float Rect2f::Height() const noexcept
{
	return bottom - top;
}

Size2f Rect2f::Size() const noexcept
{
	return { Width(), Height() };
}

float Rect2f::Area() const noexcept
{
	return Empty() ? 0.0f : Width() * Height();
}

bool Rect2f::Empty() const noexcept
{
	return left >= right || top >= bottom;
}

Point2f Rect2f::Center() const noexcept
{
	return {
		(left + right) * 0.5f,
		(top + bottom) * 0.5f
	};
}

void Rect2f::Normalize() noexcept
{
	if (left > right) std::swap(left, right);
	if (top > bottom) std::swap(top, bottom);
}

bool Rect2f::Contains(float x, float y) const noexcept
{
	return x >= left && x <= right &&
		y >= top && y <= bottom;
}

bool Rect2f::Contains(const Point2f& p) const noexcept
{
	return Contains(p.x, p.y);
}

bool Rect2f::Contains(const Rect2f& r) const noexcept
{
	return r.left >= left && r.right <= right &&
		r.top >= top && r.bottom <= bottom;
}

bool Rect2f::Intersects(const Rect2f& r) const noexcept
{
	return !(r.right <= left || r.left >= right ||
		r.bottom <= top || r.top >= bottom);
}

Rect2f Rect2f::Intersect(const Rect2f& r) const noexcept
{
	Rect2f out{
		std::max(left, r.left),
		std::max(top, r.top),
		std::min(right, r.right),
		std::min(bottom, r.bottom)
	};

	if (out.Empty())
		out.Reset();

	return out;
}

Rect2f Rect2f::Unite(const Rect2f& r) const noexcept
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

Rect2f& Rect2f::IntersectWith(const Rect2f& r) noexcept
{
	*this = Intersect(r);
	return *this;
}

Rect2f& Rect2f::UniteWith(const Rect2f& r) noexcept
{
	*this = Unite(r);
	return *this;
}

void Rect2f::Offset(float dx, float dy) noexcept
{
	left += dx; right += dx;
	top += dy;  bottom += dy;
}

void Rect2f::Inflate(float dx, float dy) noexcept
{
	left -= dx; right += dx;
	top -= dy;  bottom += dy;
}

void Rect2f::Deflate(float dx, float dy) noexcept
{
	Inflate(-dx, -dy);
}

void Rect2f::ScaleFromCenter(float sx, float sy) noexcept
{
	const auto c = Center();
	const float hw = Width() * 0.5f * sx;
	const float hh = Height() * 0.5f * sy;

	left = c.x - hw;
	right = c.x + hw;
	top = c.y - hh;
	bottom = c.y + hh;
}

void Rect2f::MoveCenterTo(float cx, float cy) noexcept
{
	const auto c = Center();
	Offset(cx - c.x, cy - c.y);
}

Rect2f Rect2f::operator&(const Rect2f& r) const noexcept
{
	return Intersect(r);
}

Rect2f Rect2f::operator|(const Rect2f& r) const noexcept
{
	return Unite(r);
}

Rect2f& Rect2f::operator&=(const Rect2f& r) noexcept
{
	return IntersectWith(r);
}

Rect2f& Rect2f::operator|=(const Rect2f& r) noexcept
{
	return UniteWith(r);
}

bool Rect2f::operator==(const Rect2f& r) const noexcept
{
	return left == r.left && top == r.top &&
		right == r.right && bottom == r.bottom;
}

bool Rect2f::operator!=(const Rect2f& r) const noexcept
{
	return !(*this == r);
}
