#pragma once

#include "ShapeTypeBase.h"

namespace Core
{
	namespace ShapeType
	{
	struct Point2f;
	struct Rect2f;

	struct Ellipse2f
	{
	public:
		using value_type = float;

	public:
		// < Variables >
		float x{ 0.0f };           // < center x >
		float y{ 0.0f };           // < center y >
		float radiusX{ 0.0f };     // < x radius >
		float radiusY{ 0.0f };     // < y radius >
		float angleRad{ 0.0f };    // < rotation angle (radian, CCW) >

		// < Constructor >
		constexpr Ellipse2f() noexcept = default;

		Ellipse2f(float cx, float cy,
			float rx, float ry,
			float angleRad = 0.0f) noexcept;

		Ellipse2f(const Point2f& center,
			float rx, float ry,
			float angleRad = 0.0f) noexcept;

		Ellipse2f(const Ellipse2f&) = default;
		Ellipse2f(Ellipse2f&&) noexcept = default;
		Ellipse2f& operator=(const Ellipse2f&) = default;
		Ellipse2f& operator=(Ellipse2f&&) noexcept = default;

		~Ellipse2f() = default;

	public:
		// < Set / Reset >
		void Set(float cx, float cy, float rx, float ry, float angleRad) noexcept;

		void Set(const Point2f& center, float rx, float ry, float angleRad) noexcept;

		void Reset() noexcept;

		// < Geometry >
		Point2f Center() const noexcept;
		float Area() const noexcept;

		bool Contains(float px, float py) const noexcept;
		bool Contains(const Point2f& p) const noexcept;

		// < Helpers >
		Rect2f BoundingBoxF() const noexcept;
	};
	}
}