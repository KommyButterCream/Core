#pragma once

#include "ShapeTypeBase.h"

namespace Core
{
	namespace ShapeType
	{
	struct Point2d;
	struct Rect2d;

	struct Ellipse2d
	{
	public:
		using value_type = double;

	public:
		// < Variables >
		double x{ 0.0 };           // < center x >
		double y{ 0.0 };           // < center y >
		double radiusX{ 0.0 };     // < x radius >
		double radiusY{ 0.0 };     // < y radius >
		double angleRad{ 0.0 };    // < rotation angle (radian, CCW) >

		// < Constructor >
		constexpr Ellipse2d() noexcept = default;

		Ellipse2d(double cx, double cy,
			double rx, double ry,
			double angleRad = 0.0) noexcept;

		Ellipse2d(const Point2d& center,
			double rx, double ry,
			double angleRad = 0.0) noexcept;

		Ellipse2d(const Ellipse2d&) = default;
		Ellipse2d(Ellipse2d&&) noexcept = default;
		Ellipse2d& operator=(const Ellipse2d&) = default;
		Ellipse2d& operator=(Ellipse2d&&) noexcept = default;

		~Ellipse2d() = default;

	public:
		// < Set / Reset >
		void Set(double cx, double cy, double rx, double ry, double angleRad) noexcept;

		void Set(const Point2d& center, double rx, double ry, double angleRad) noexcept;

		void Reset() noexcept;

		// < Geometry >
		Point2d Center() const noexcept;
		double Area() const noexcept;

		bool Contains(double px, double py) const noexcept;
		bool Contains(const Point2d& p) const noexcept;

		// < Helpers >
		Rect2d BoundingBoxF() const noexcept;
	};
	}
}