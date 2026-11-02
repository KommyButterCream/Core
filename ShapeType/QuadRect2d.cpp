#include "QuadRect2d.h"
#include "Rect2d.h"

#include <algorithm>
#include <cmath>

using Core::ShapeType::Point2d;
using Core::ShapeType::QuadRect2d;
using Core::ShapeType::Rect2d;

QuadRect2d::QuadRect2d(
	const Point2d& tl,
	const Point2d& tr,
	const Point2d& br,
	const Point2d& bl) noexcept
{
	Set(tl, tr, br, bl);
}

QuadRect2d::QuadRect2d(
	double tl_x, double tl_y,
	double tr_x, double tr_y,
	double br_x, double br_y,
	double bl_x, double bl_y) noexcept
{
	Set({ tl_x, tl_y }, { tr_x, tr_y }, { br_x, br_y }, { bl_x, bl_y });
}

void QuadRect2d::Set(
	const Point2d& tl,
	const Point2d& tr,
	const Point2d& br,
	const Point2d& bl) noexcept
{
	TopLeft = tl;
	TopRight = tr;
	BottomRight = br;
	BottomLeft = bl;
}

void QuadRect2d::Reset() noexcept
{
	TopLeft = TopRight = BottomRight = BottomLeft = {};
}

Point2d QuadRect2d::Center() const noexcept
{
	return {
		(TopLeft.x + TopRight.x + BottomRight.x + BottomLeft.x) * 0.25f,
		(TopLeft.y + TopRight.y + BottomRight.y + BottomLeft.y) * 0.25f
	};
}

double QuadRect2d::Area() const noexcept
{
	auto Cross = [](const Point2d& a, const Point2d& b)
		{
			return a.x * b.y - a.y * b.x;
		};

	double area = 0.0;
	area += Cross(TopLeft, TopRight);
	area += Cross(TopRight, BottomRight);
	area += Cross(BottomRight, BottomLeft);
	area += Cross(BottomLeft, TopLeft);

	return std::abs(area) * 0.5f;
}

bool QuadRect2d::Empty() const noexcept
{
	return Area() <= std::numeric_limits<double>::epsilon();
}

bool QuadRect2d::IsConvex() const noexcept
{
	auto CrossZ = [](const Point2d& a, const Point2d& b, const Point2d& c)
		{
			return (b.x - a.x) * (c.y - a.y) -
				(b.y - a.y) * (c.x - a.x);
		};

	double z1 = CrossZ(TopLeft, TopRight, BottomRight);
	double z2 = CrossZ(TopRight, BottomRight, BottomLeft);
	double z3 = CrossZ(BottomRight, BottomLeft, TopLeft);
	double z4 = CrossZ(BottomLeft, TopLeft, TopRight);

	return (z1 > 0 && z2 > 0 && z3 > 0 && z4 > 0) ||
		(z1 < 0 && z2 < 0 && z3 < 0 && z4 < 0);
}

bool QuadRect2d::Contains(const Point2d& p) const noexcept
{
	auto SameSide = [](const Point2d& a, const Point2d& b,
		const Point2d& p1, const Point2d& p2)
		{
			double cp1 = (b.x - a.x) * (p1.y - a.y) - (b.y - a.y) * (p1.x - a.x);
			double cp2 = (b.x - a.x) * (p2.y - a.y) - (b.y - a.y) * (p2.x - a.x);
			return cp1 * cp2 >= 0;
		};

	return
		SameSide(TopLeft, TopRight, p, BottomLeft) &&
		SameSide(TopRight, BottomRight, p, TopLeft) &&
		SameSide(BottomRight, BottomLeft, p, TopRight) &&
		SameSide(BottomLeft, TopLeft, p, BottomRight);
}

void QuadRect2d::Offset(double dx, double dy) noexcept
{
	TopLeft.x += dx;  TopLeft.y += dy;
	TopRight.x += dx; TopRight.y += dy;
	BottomRight.x += dx; BottomRight.y += dy;
	BottomLeft.x += dx; BottomLeft.y += dy;
}

void QuadRect2d::ScaleFromCenter(double sx, double sy) noexcept
{
	Point2d c = Center();

	auto Scale = [&](Point2d& p)
		{
			p.x = c.x + (p.x - c.x) * sx;
			p.y = c.y + (p.y - c.y) * sy;
		};

	Scale(TopLeft);
	Scale(TopRight);
	Scale(BottomRight);
	Scale(BottomLeft);
}

Rect2d QuadRect2d::ComputeBoundingRect() const noexcept
{
	double minX = std::min({ TopLeft.x, TopRight.x, BottomRight.x, BottomLeft.x });
	double minY = std::min({ TopLeft.y, TopRight.y, BottomRight.y, BottomLeft.y });
	double maxX = std::max({ TopLeft.x, TopRight.x, BottomRight.x, BottomLeft.x });
	double maxY = std::max({ TopLeft.y, TopRight.y, BottomRight.y, BottomLeft.y });

	return Rect2d(minX, minY, maxX, maxY);
}

Rect2d QuadRect2d::ComputeInnerRect() const noexcept
{
	double left = std::max({ TopLeft.x, BottomLeft.x });
	double right = std::min({ TopRight.x, BottomRight.x });
	double top = std::max({ TopLeft.y, TopRight.y });
	double bottom = std::min({ BottomLeft.y, BottomRight.y });

	if (left > right || top > bottom)
		return {};

	return Rect2d(left, top, right, bottom);
}

Rect2d QuadRect2d::ToRect2d() const noexcept
{
	return ComputeBoundingRect();
}

bool QuadRect2d::operator==(const QuadRect2d& rhs) const noexcept
{
	return TopLeft == rhs.TopLeft &&
		TopRight == rhs.TopRight &&
		BottomRight == rhs.BottomRight &&
		BottomLeft == rhs.BottomLeft;
}

bool QuadRect2d::operator!=(const QuadRect2d& rhs) const noexcept
{
	return !(*this == rhs);
}
