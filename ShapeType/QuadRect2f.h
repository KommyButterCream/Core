#pragma once
#include "ShapeTypeBase.h"
#include "Point2f.h"

namespace Core
{
	namespace ShapeType
	{
	struct Rect2f;

	struct QuadRect2f
	{
		using value_type = float;

		// < Variables >
		Point2f TopLeft{ 0.0f, 0.0f };
		Point2f TopRight{ 0.0f, 0.0f };
		Point2f BottomRight{ 0.0f, 0.0f };
		Point2f BottomLeft{ 0.0f, 0.0f };

		// < Constructor >
		constexpr QuadRect2f() noexcept = default;

		QuadRect2f(
			const Point2f& tl,
			const Point2f& tr,
			const Point2f& br,
			const Point2f& bl) noexcept;

		QuadRect2f(
			float tl_x, float tl_y,
			float tr_x, float tr_y,
			float br_x, float br_y,
			float bl_x, float bl_y) noexcept;

		QuadRect2f(const QuadRect2f& rhs) = default;
		QuadRect2f(QuadRect2f&& rhs) noexcept = default;
		QuadRect2f& operator=(const QuadRect2f& rhs) = default;
		QuadRect2f& operator=(QuadRect2f&& rhs) noexcept = default;

		// < Methods >
		void Set(
			const Point2f& tl,
			const Point2f& tr,
			const Point2f& br,
			const Point2f& bl) noexcept;

		void Reset() noexcept;

		Point2f Center() const noexcept;
		float Area() const noexcept;
		bool Empty() const noexcept;

		bool IsConvex() const noexcept;
		bool Contains(const Point2f& p) const noexcept;

		void Offset(float dx, float dy) noexcept;
		void ScaleFromCenter(float sx, float sy) noexcept;

		Rect2f ComputeBoundingRect() const noexcept;
		Rect2f ComputeInnerRect() const noexcept;

		// < Conversion >
		Rect2f ToRect2f() const noexcept;

		// < Operators >
		bool operator==(const QuadRect2f& rhs) const noexcept;
		bool operator!=(const QuadRect2f& rhs) const noexcept;
	};
	}
}