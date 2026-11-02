#include "QuadRect2i.h"
#include "Rect2i.h"

#include <algorithm>
#include <cmath>

using Core::ShapeType::Point2f;
using Core::ShapeType::Point2i;
using Core::ShapeType::QuadRect2i;
using Core::ShapeType::Rect2i;

QuadRect2i::QuadRect2i(
	const Point2i& tl,
	const Point2i& tr,
	const Point2i& br,
	const Point2i& bl) noexcept
{
	Set(tl, tr, br, bl);
}

QuadRect2i::QuadRect2i(
	int32_t tl_x, int32_t tl_y,
	int32_t tr_x, int32_t tr_y,
	int32_t br_x, int32_t br_y,
	int32_t bl_x, int32_t bl_y) noexcept
{
	Set({ tl_x, tl_y }, { tr_x, tr_y }, { br_x, br_y }, { bl_x, bl_y });
}

void QuadRect2i::Set(
	const Point2i& tl,
	const Point2i& tr,
	const Point2i& br,
	const Point2i& bl) noexcept
{
	TopLeft = tl;
	TopRight = tr;
	BottomRight = br;
	BottomLeft = bl;
}

void QuadRect2i::Reset() noexcept
{
	TopLeft = TopRight = BottomRight = BottomLeft = {};
}

Point2f QuadRect2i::Center() const noexcept
{
	return {
		(TopLeft.x + TopRight.x + BottomRight.x + BottomLeft.x) * 0.25f,
		(TopLeft.y + TopRight.y + BottomRight.y + BottomLeft.y) * 0.25f
	};
}

float QuadRect2i::Area() const noexcept
{
	auto Cross = [](const Point2i& a, const Point2i& b)
		{
			return a.x * b.y - a.y * b.x;
		};

	int32_t area = 0;
	area += Cross(TopLeft, TopRight);
	area += Cross(TopRight, BottomRight);
	area += Cross(BottomRight, BottomLeft);
	area += Cross(BottomLeft, TopLeft);

	return std::abs(area) * 0.5f;
}

bool QuadRect2i::Empty() const noexcept
{
	return Area() <= std::numeric_limits<int32_t>::epsilon();
}

bool QuadRect2i::IsConvex() const noexcept
{
	auto CrossZ = [](const Point2i& a, const Point2i& b, const Point2i& c)
		{
			return (b.x - a.x) * (c.y - a.y) -
				(b.y - a.y) * (c.x - a.x);
		};

	int32_t z1 = CrossZ(TopLeft, TopRight, BottomRight);
	int32_t z2 = CrossZ(TopRight, BottomRight, BottomLeft);
	int32_t z3 = CrossZ(BottomRight, BottomLeft, TopLeft);
	int32_t z4 = CrossZ(BottomLeft, TopLeft, TopRight);

	return (z1 > 0 && z2 > 0 && z3 > 0 && z4 > 0) ||
		(z1 < 0 && z2 < 0 && z3 < 0 && z4 < 0);
}

bool QuadRect2i::Contains(const Point2i& p) const noexcept
{
	auto SameSide = [](const Point2i& a, const Point2i& b,
		const Point2i& p1, const Point2i& p2)
		{
			int32_t cp1 = (b.x - a.x) * (p1.y - a.y) - (b.y - a.y) * (p1.x - a.x);
			int32_t cp2 = (b.x - a.x) * (p2.y - a.y) - (b.y - a.y) * (p2.x - a.x);
			return cp1 * cp2 >= 0;
		};

	return
		SameSide(TopLeft, TopRight, p, BottomLeft) &&
		SameSide(TopRight, BottomRight, p, TopLeft) &&
		SameSide(BottomRight, BottomLeft, p, TopRight) &&
		SameSide(BottomLeft, TopLeft, p, BottomRight);
}

void QuadRect2i::Offset(int32_t dx, int32_t dy) noexcept
{
	TopLeft.x += dx;  TopLeft.y += dy;
	TopRight.x += dx; TopRight.y += dy;
	BottomRight.x += dx; BottomRight.y += dy;
	BottomLeft.x += dx; BottomLeft.y += dy;
}

void QuadRect2i::ScaleFromCenter(int32_t sx, int32_t sy) noexcept
{
	Point2f c = Center();

	auto Scale = [&](Point2i& p)
		{
			float fx = c.x + (static_cast<float>(p.x) - c.x) * sx;
			float fy = c.y + (static_cast<float>(p.y) - c.y) * sy;

			p.x = static_cast<int32_t>(std::round(fx));
			p.y = static_cast<int32_t>(std::round(fy));
		};

	Scale(TopLeft);
	Scale(TopRight);
	Scale(BottomRight);
	Scale(BottomLeft);
}

Rect2i QuadRect2i::ComputeBoundingRect() const noexcept
{
	int32_t minX = std::min({ TopLeft.x, TopRight.x, BottomRight.x, BottomLeft.x });
	int32_t minY = std::min({ TopLeft.y, TopRight.y, BottomRight.y, BottomLeft.y });
	int32_t maxX = std::max({ TopLeft.x, TopRight.x, BottomRight.x, BottomLeft.x });
	int32_t maxY = std::max({ TopLeft.y, TopRight.y, BottomRight.y, BottomLeft.y });

	return Rect2i(minX, minY, maxX, maxY);
}

Rect2i QuadRect2i::ComputeInnerRect() const noexcept
{
	int32_t left = std::max({ TopLeft.x, BottomLeft.x });
	int32_t right = std::min({ TopRight.x, BottomRight.x });
	int32_t top = std::max({ TopLeft.y, TopRight.y });
	int32_t bottom = std::min({ BottomLeft.y, BottomRight.y });

	if (left > right || top > bottom)
		return {};

	return Rect2i(left, top, right, bottom);
}

Rect2i QuadRect2i::ToRect2i() const noexcept
{
	return ComputeBoundingRect();
}

bool QuadRect2i::operator==(const QuadRect2i& rhs) const noexcept
{
	return TopLeft == rhs.TopLeft &&
		TopRight == rhs.TopRight &&
		BottomRight == rhs.BottomRight &&
		BottomLeft == rhs.BottomLeft;
}

bool QuadRect2i::operator!=(const QuadRect2i& rhs) const noexcept
{
	return !(*this == rhs);
}
