#include "RotatedRect2f.h"

#include <cmath>
#include <algorithm>

#include "Rect2f.h"

using Core::ShapeType::Point2f;
using Core::ShapeType::Rect2f;
using Core::ShapeType::RotatedRect2f;
using Core::ShapeType::Size2f;

RotatedRect2f::RotatedRect2f(const Point2f& center,
	const Size2f& size,
	float angleRad) noexcept
	: center{ center }
	, size{ size }
	, angleRad{ angleRad }
{
}

RotatedRect2f::RotatedRect2f(float cx, float cy,
	float width, float height,
	float angleRad) noexcept
	: center{ cx, cy }
	, size{ width, height }
	, angleRad{ angleRad }
{
}

constexpr void RotatedRect2f::Set(float cx, float cy,
	float width, float height,
	float angleRad) noexcept
{
	center.x = cx;
	center.y = cy;
	size.width = width;
	size.height = height;
	angleRad = angleRad;
}

constexpr void RotatedRect2f::Reset() noexcept
{
	center.x = 0.f;
	center.y = 0.f;
	size.width = 0.f;
	size.height = 0.f;
	angleRad = 0.f;
}

constexpr float RotatedRect2f::Width() const noexcept
{
	return size.width;
}

constexpr float RotatedRect2f::Height() const noexcept
{
	return size.height;
}

constexpr const Point2f& RotatedRect2f::GetCenter() const noexcept
{
	return center;
}

constexpr const Size2f& RotatedRect2f::GetSize() const noexcept
{
	return size;
}

constexpr float RotatedRect2f::GetAngle() const noexcept
{
	return angleRad;
}

constexpr void RotatedRect2f::Rotate(float deltaAngleRad) noexcept
{
	angleRad += deltaAngleRad;
}

constexpr void RotatedRect2f::SetAngle(float angleRad) noexcept
{
	angleRad = angleRad;
}

constexpr void RotatedRect2f::Translate(float dx, float dy) noexcept
{
	center.x += dx;
	center.y += dy;
}

constexpr bool RotatedRect2f::IsValid() const noexcept
{
	return (size.width > 0.f) && (size.height > 0.f);
}

RotatedRect2f::VertexArray RotatedRect2f::GetVertices() const noexcept
{
	const float hw = size.width * 0.5f;
	const float hh = size.height * 0.5f;

	const float c = std::cos(angleRad);
	const float s = std::sin(angleRad);

	VertexArray verts;

	const Point2f local[4] =
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

Rect2f RotatedRect2f::GetBoundingRect() const noexcept
{
	const auto verts = GetVertices();

	float minX = verts[0].x;
	float maxX = verts[0].x;
	float minY = verts[0].y;
	float maxY = verts[0].y;

	for (int i = 1; i < 4; ++i)
	{
		minX = std::min(minX, verts[i].x);
		maxX = std::max(maxX, verts[i].x);
		minY = std::min(minY, verts[i].y);
		maxY = std::max(maxY, verts[i].y);
	}

	return Rect2f{ minX, minY, maxX - minX, maxY - minY };
}
