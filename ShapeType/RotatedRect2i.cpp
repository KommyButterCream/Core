#include "RotatedRect2i.h"

#include <cmath>
#include <algorithm>

#include "Rect2i.h"
#include "Point2f.h"

using Core::ShapeType::Point2f;
using Core::ShapeType::Point2i;
using Core::ShapeType::Rect2i;
using Core::ShapeType::RotatedRect2i;
using Core::ShapeType::Size2i;

RotatedRect2i::RotatedRect2i(const Point2i& center,
	const Size2i& size,
	float angleRad) noexcept
	: center{ center }
	, size{ size }
	, angleRad{ angleRad }
{
}

RotatedRect2i::RotatedRect2i(int32_t cx, int32_t cy,
	int32_t width, int32_t height,
	float angleRad) noexcept
	: center{ cx, cy }
	, size{ width, height }
	, angleRad{ angleRad }
{
}

constexpr void RotatedRect2i::Set(int32_t cx, int32_t cy,
	int32_t width, int32_t height,
	float angleRad) noexcept
{
	center.x = cx;
	center.y = cy;
	size.width = width;
	size.height = height;
	angleRad = angleRad;
}

constexpr void RotatedRect2i::Reset() noexcept
{
	center.x = 0;
	center.y = 0;
	size.width = 0;
	size.height = 0;
	angleRad = 0.0f;
}

constexpr int32_t RotatedRect2i::Width() const noexcept
{
	return size.width;
}

constexpr int32_t RotatedRect2i::Height() const noexcept
{
	return size.height;
}

constexpr const Point2i& RotatedRect2i::GetCenter() const noexcept
{
	return center;
}

constexpr const Size2i& RotatedRect2i::GetSize() const noexcept
{
	return size;
}

constexpr float RotatedRect2i::GetAngle() const noexcept
{
	return angleRad;
}

constexpr void RotatedRect2i::Rotate(float deltaAngleRad) noexcept
{
	angleRad += deltaAngleRad;
}

constexpr void RotatedRect2i::SetAngle(float angleRad) noexcept
{
	angleRad = angleRad;
}

constexpr void RotatedRect2i::Translate(int32_t dx, int32_t dy) noexcept
{
	center.x += dx;
	center.y += dy;
}

constexpr bool RotatedRect2i::IsValid() const noexcept
{
	return (size.width > 0) && (size.height > 0);
}

RotatedRect2i::VertexArray RotatedRect2i::GetVertices() const noexcept
{
	const float hw = static_cast<float>(size.width) * 0.5f;
	const float hh = static_cast<float>(size.height) * 0.5f;

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
		const float x =
			static_cast<float>(center.x) +
			local[i].x * c -
			local[i].y * s;

		const float y =
			static_cast<float>(center.y) +
			local[i].x * s +
			local[i].y * c;

		verts[i].x = static_cast<int32_t>(std::lround(x));
		verts[i].y = static_cast<int32_t>(std::lround(y));
	}

	return verts;
}

Rect2i RotatedRect2i::GetBoundingRect() const noexcept
{
	const auto verts = GetVertices();

	int32_t minX = verts[0].x;
	int32_t maxX = verts[0].x;
	int32_t minY = verts[0].y;
	int32_t maxY = verts[0].y;

	for (int i = 1; i < 4; ++i)
	{
		minX = std::min(minX, verts[i].x);
		maxX = std::max(maxX, verts[i].x);
		minY = std::min(minY, verts[i].y);
		maxY = std::max(maxY, verts[i].y);
	}

	return Rect2i{ minX, minY, maxX - minX, maxY - minY };
}
