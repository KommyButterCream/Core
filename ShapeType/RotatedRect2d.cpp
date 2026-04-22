#include "RotatedRect2d.h"

#include <cmath>
#include <algorithm>

#include "Rect2d.h"

using Core::ShapeType::Point2d;
using Core::ShapeType::Rect2d;
using Core::ShapeType::RotatedRect2d;
using Core::ShapeType::Size2d;

RotatedRect2d::RotatedRect2d(const Point2d& center,
	const Size2d& size,
	double angleRad) noexcept
	: center{ center }
	, size{ size }
	, angleRad{ angleRad }
{
}

RotatedRect2d::RotatedRect2d(double cx, double cy,
	double width, double height,
	double angleRad) noexcept
	: center{ cx, cy }
	, size{ width, height }
	, angleRad{ angleRad }
{
}

constexpr void RotatedRect2d::Set(double cx, double cy,
	double width, double height,
	double angleRad) noexcept
{
	center.x = cx;
	center.y = cy;
	size.width = width;
	size.height = height;
	angleRad = angleRad;
}

constexpr void RotatedRect2d::Reset() noexcept
{
	center.x = 0.0;
	center.y = 0.0;
	size.width = 0.0;
	size.height = 0.0;
	angleRad = 0.0;
}

constexpr double RotatedRect2d::Width() const noexcept
{
	return size.width;
}

constexpr double RotatedRect2d::Height() const noexcept
{
	return size.height;
}

constexpr const Point2d& RotatedRect2d::GetCenter() const noexcept
{
	return center;
}

constexpr const Size2d& RotatedRect2d::GetSize() const noexcept
{
	return size;
}

constexpr double RotatedRect2d::GetAngle() const noexcept
{
	return angleRad;
}

constexpr void RotatedRect2d::Rotate(double deltaAngleRad) noexcept
{
	angleRad += deltaAngleRad;
}

constexpr void RotatedRect2d::SetAngle(double angleRad) noexcept
{
	angleRad = angleRad;
}

constexpr void RotatedRect2d::Translate(double dx, double dy) noexcept
{
	center.x += dx;
	center.y += dy;
}

constexpr bool RotatedRect2d::IsValid() const noexcept
{
	return (size.width > 0.0) && (size.height > 0.0);
}

RotatedRect2d::VertexArray RotatedRect2d::GetVertices() const noexcept
{
	const double hw = size.width * 0.5;
	const double hh = size.height * 0.5;

	const double c = std::cos(angleRad);
	const double s = std::sin(angleRad);

	VertexArray verts;

	const Point2d local[4] =
	{
		{ -hw, -hh },
		{  hw, -hh },
		{  hw,  hh },
		{ -hw,  hh }
	};

	for (int i = 0; i < 4; ++i)
	{
		verts[i].x = center.x + local[i].x * c - local[i].y * s;
		verts[i].y = center.y + local[i].x * s + local[i].y * c;
	}

	return verts;
}

Rect2d RotatedRect2d::GetBoundingRect() const noexcept
{
	const auto verts = GetVertices();

	double minX = verts[0].x;
	double maxX = verts[0].x;
	double minY = verts[0].y;
	double maxY = verts[0].y;

	for (int i = 1; i < 4; ++i)
	{
		minX = std::min(minX, verts[i].x);
		maxX = std::max(maxX, verts[i].x);
		minY = std::min(minY, verts[i].y);
		maxY = std::max(maxY, verts[i].y);
	}

	return Rect2d{ minX, minY, maxX - minX, maxY - minY };
}
