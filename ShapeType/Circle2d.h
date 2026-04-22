#pragma once

#include "ShapeTypeBase.h"

namespace Core
{
	namespace ShapeType
	{
		struct Point2d;
		struct Rect2d;

		struct Circle2d
		{
		public:
			using value_type = double;

		public:
			double x{ 0.0 };       // center x
			double y{ 0.0 };       // center y
			double radius{ 0.0 };  // radius

			// --- ctor ---
			constexpr Circle2d() noexcept = default;
			Circle2d(double cx, double cy, double r) noexcept;
			Circle2d(const Point2d& center, double r) noexcept;

			Circle2d(const Circle2d&) = default;
			Circle2d(Circle2d&&) noexcept = default;
			Circle2d& operator=(const Circle2d&) = default;
			Circle2d& operator=(Circle2d&&) noexcept = default;

			~Circle2d() = default;

		public:
			// < Methods >
			void Set(double cx, double cy, double r) noexcept;
			void Set(const Point2d& center, double r) noexcept;
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
