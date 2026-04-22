#include "QuadRect2f.h"
#include "Rect2f.h"

#include <algorithm>
#include <cmath>

using Core::ShapeType::Point2f;
using Core::ShapeType::QuadRect2f;
using Core::ShapeType::Rect2f;

QuadRect2f::QuadRect2f(
	const Point2f& tl,
	const Point2f& tr,
	const Point2f& br,
	const Point2f& bl) noexcept
{
	Set(tl, tr, br, bl);
}

QuadRect2f::QuadRect2f(
	float tl_x, float tl_y,
	float tr_x, float tr_y,
	float br_x, float br_y,
	float bl_x, float bl_y) noexcept
{
	Set({ tl_x, tl_y }, { tr_x, tr_y }, { br_x, br_y }, { bl_x, bl_y });
}

void QuadRect2f::Set(
	const Point2f& tl,
	const Point2f& tr,
	const Point2f& br,
	const Point2f& bl) noexcept
{
	TopLeft = tl;
	TopRight = tr;
	BottomRight = br;
	BottomLeft = bl;
}

void QuadRect2f::Reset() noexcept
{
	TopLeft = TopRight = BottomRight = BottomLeft = {};
}

Point2f QuadRect2f::Center() const noexcept
{
	return {
		(TopLeft.x + TopRight.x + BottomRight.x + BottomLeft.x) * 0.25f,
		(TopLeft.y + TopRight.y + BottomRight.y + BottomLeft.y) * 0.25f
	};
}

float QuadRect2f::Area() const noexcept
{
	auto Cross = [](const Point2f& a, const Point2f& b)
		{
			return a.x * b.y - a.y * b.x;
		};

	float area = 0.f;
	area += Cross(TopLeft, TopRight);
	area += Cross(TopRight, BottomRight);
	area += Cross(BottomRight, BottomLeft);
	area += Cross(BottomLeft, TopLeft);

	return std::abs(area) * 0.5f;
}

bool QuadRect2f::Empty() const noexcept
{
	return Area() <= std::numeric_limits<float>::epsilon();
}

bool QuadRect2f::IsConvex() const noexcept
{
	auto CrossZ = [](const Point2f& a, const Point2f& b, const Point2f& c)
		{
			return (b.x - a.x) * (c.y - a.y) -
				(b.y - a.y) * (c.x - a.x);
		};

	float z1 = CrossZ(TopLeft, TopRight, BottomRight);
	float z2 = CrossZ(TopRight, BottomRight, BottomLeft);
	float z3 = CrossZ(BottomRight, BottomLeft, TopLeft);
	float z4 = CrossZ(BottomLeft, TopLeft, TopRight);

	return (z1 > 0 && z2 > 0 && z3 > 0 && z4 > 0) ||
		(z1 < 0 && z2 < 0 && z3 < 0 && z4 < 0);
}

bool QuadRect2f::Contains(const Point2f& p) const noexcept
{
	auto SameSide = [](const Point2f& a, const Point2f& b,
		const Point2f& p1, const Point2f& p2)
		{
			float cp1 = (b.x - a.x) * (p1.y - a.y) - (b.y - a.y) * (p1.x - a.x);
			float cp2 = (b.x - a.x) * (p2.y - a.y) - (b.y - a.y) * (p2.x - a.x);
			return cp1 * cp2 >= 0;
		};

	return
		SameSide(TopLeft, TopRight, p, BottomLeft) &&
		SameSide(TopRight, BottomRight, p, TopLeft) &&
		SameSide(BottomRight, BottomLeft, p, TopRight) &&
		SameSide(BottomLeft, TopLeft, p, BottomRight);
}

void QuadRect2f::Offset(float dx, float dy) noexcept
{
	TopLeft.x += dx;  TopLeft.y += dy;
	TopRight.x += dx; TopRight.y += dy;
	BottomRight.x += dx; BottomRight.y += dy;
	BottomLeft.x += dx; BottomLeft.y += dy;
}

void QuadRect2f::ScaleFromCenter(float sx, float sy) noexcept
{
	Point2f c = Center();

	auto Scale = [&](Point2f& p)
		{
			p.x = c.x + (p.x - c.x) * sx;
			p.y = c.y + (p.y - c.y) * sy;
		};

	Scale(TopLeft);
	Scale(TopRight);
	Scale(BottomRight);
	Scale(BottomLeft);
}

Rect2f QuadRect2f::ComputeBoundingRect() const noexcept
{
	float minX = std::min({ TopLeft.x, TopRight.x, BottomRight.x, BottomLeft.x });
	float minY = std::min({ TopLeft.y, TopRight.y, BottomRight.y, BottomLeft.y });
	float maxX = std::max({ TopLeft.x, TopRight.x, BottomRight.x, BottomLeft.x });
	float maxY = std::max({ TopLeft.y, TopRight.y, BottomRight.y, BottomLeft.y });

	return Rect2f(minX, minY, maxX, maxY);
}

Rect2f QuadRect2f::ComputeInnerRect() const noexcept
{
	float left = std::max({ TopLeft.x, BottomLeft.x });
	float right = std::min({ TopRight.x, BottomRight.x });
	float top = std::max({ TopLeft.y, TopRight.y });
	float bottom = std::min({ BottomLeft.y, BottomRight.y });

	if (left > right || top > bottom)
		return {};

	return Rect2f(left, top, right, bottom);
}

Rect2f QuadRect2f::ToRect2f() const noexcept
{
	return ComputeBoundingRect();
}

bool QuadRect2f::operator==(const QuadRect2f& rhs) const noexcept
{
	return TopLeft == rhs.TopLeft &&
		TopRight == rhs.TopRight &&
		BottomRight == rhs.BottomRight &&
		BottomLeft == rhs.BottomLeft;
}

bool QuadRect2f::operator!=(const QuadRect2f& rhs) const noexcept
{
	return !(*this == rhs);
}
