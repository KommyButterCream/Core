#include "Rect2i.h"
#include "Point2f.h"
#include "Size2f.h"

#include <algorithm>

using Core::ShapeType::Point2f;
using Core::ShapeType::Rect2i;
using Core::ShapeType::Size2f;

Rect2i::Rect2i(int32_t l, int32_t t, int32_t r, int32_t b) noexcept
	: left(l), top(t), right(r), bottom(b)
{
}

void Rect2i::Set(int32_t l, int32_t t, int32_t r, int32_t b) noexcept
{
	left = l;
	top = t;
	right = r;
	bottom = b;
}

void Rect2i::Reset() noexcept
{
	left = top = right = bottom = 0;
}

int32_t Rect2i::Width() const noexcept
{
	return right - left;
}

int32_t Rect2i::Height() const noexcept
{
	return bottom - top;
}

Size2f Rect2i::Size() const noexcept
{
	return { static_cast<float>(Width()), static_cast<float>(Height()) };
}

int32_t Rect2i::Area() const noexcept
{
	return Empty() ? 0 : Width() * Height();
}

bool Rect2i::Empty() const noexcept
{
	return left >= right || top >= bottom;
}

Point2f Rect2i::Center() const noexcept
{
	return {
		(left + right) * 0.5f,
		(top + bottom) * 0.5f
	};
}

void Rect2i::Normalize() noexcept
{
	if (left > right) std::swap(left, right);
	if (top > bottom) std::swap(top, bottom);
}

bool Rect2i::Contains(float x, float y) const noexcept
{
	return x >= static_cast<float>(left) && x <= static_cast<float>(right) &&
		y >= static_cast<float>(top) && y <= static_cast<float>(bottom);
}

bool Rect2i::Contains(int32_t x, int32_t y) const noexcept
{
	return x >= left && x <= right &&
		y >= top && y <= bottom;
}

bool Rect2i::Contains(const Point2f& p) const noexcept
{
	return Contains(p.x, p.y);
}

bool Rect2i::Contains(const Rect2i& r) const noexcept
{
	return r.left >= left && r.right <= right &&
		r.top >= top && r.bottom <= bottom;
}

bool Rect2i::Intersects(const Rect2i& r) const noexcept
{
	return !(r.right <= left || r.left >= right ||
		r.bottom <= top || r.top >= bottom);
}

Rect2i Rect2i::Intersect(const Rect2i& r) const noexcept
{
	Rect2i out{
		std::max(left, r.left),
		std::max(top, r.top),
		std::min(right, r.right),
		std::min(bottom, r.bottom)
	};

	if (out.Empty())
		out.Reset();

	return out;
}

Rect2i Rect2i::Unite(const Rect2i& r) const noexcept
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

Rect2i& Rect2i::IntersectWith(const Rect2i& r) noexcept
{
	*this = Intersect(r);
	return *this;
}

Rect2i& Rect2i::UniteWith(const Rect2i& r) noexcept
{
	*this = Unite(r);
	return *this;
}

void Rect2i::Offset(int32_t dx, int32_t dy) noexcept
{
	left += dx; right += dx;
	top += dy;  bottom += dy;
}

void Rect2i::Inflate(int32_t dx, int32_t dy) noexcept
{
	left -= dx; right += dx;
	top -= dy;  bottom += dy;
}

void Rect2i::Deflate(int32_t dx, int32_t dy) noexcept
{
	Inflate(-dx, -dy);
}

void Rect2i::ScaleFromCenter(int32_t sx, int32_t sy) noexcept
{
	const auto c = Center();
	const int32_t hw = static_cast<const int32_t>(Width() * 0.5f * sx);
	const int32_t hh = static_cast<const int32_t>(Height() * 0.5f * sy);

	left = static_cast<int32_t>(c.x - hw);
	right = static_cast<int32_t>(c.x + hw);
	top = static_cast<int32_t>(c.y - hh);
	bottom = static_cast<int32_t>(c.y + hh);
}

void Rect2i::MoveCenterTo(int32_t cx, int32_t cy) noexcept
{
	const auto c = Center();
	Offset(static_cast<int32_t>(cx - c.x), static_cast<int32_t>(cy - c.y));
}

Rect2i Rect2i::operator&(const Rect2i& r) const noexcept
{
	return Intersect(r);
}

Rect2i Rect2i::operator|(const Rect2i& r) const noexcept
{
	return Unite(r);
}

Rect2i& Rect2i::operator&=(const Rect2i& r) noexcept
{
	return IntersectWith(r);
}

Rect2i& Rect2i::operator|=(const Rect2i& r) noexcept
{
	return UniteWith(r);
}

bool Rect2i::operator==(const Rect2i& r) const noexcept
{
	return left == r.left && top == r.top &&
		right == r.right && bottom == r.bottom;
}

bool Rect2i::operator!=(const Rect2i& r) const noexcept
{
	return !(*this == r);
}
