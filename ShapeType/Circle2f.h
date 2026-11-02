#pragma once

#include "ShapeTypeBase.h"

namespace Core
{
	namespace ShapeType
	{
		struct Point2f;
		struct Rect2f;

		struct Circle2f
		{
		public:
			using value_type = float;

		public:
			float x{ 0.0f };       // center x
			float y{ 0.0f };       // center y
			float radius{ 0.0f };  // radius

			// --- ctor ---
			constexpr Circle2f() noexcept = default;
			Circle2f(float cx, float cy, float r) noexcept;
			Circle2f(const Point2f& center, float r) noexcept;

			Circle2f(const Circle2f&) = default;
			Circle2f(Circle2f&&) noexcept = default;
			Circle2f& operator=(const Circle2f&) = default;
			Circle2f& operator=(Circle2f&&) noexcept = default;

			~Circle2f() = default;

		public:
			// < Methods >
			void Set(float cx, float cy, float r) noexcept;
			void Set(const Point2f& center, float r) noexcept;
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
