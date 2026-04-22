#pragma once
#include "ShapeTypeBase.h"
#include <stdint.h>
#include "Point2i.h"
#include "Point2f.h"

namespace Core
{
	namespace ShapeType
	{
	struct Rect2i;

	struct QuadRect2i
	{
		using value_type = int32_t;

		// < Variables >
		Point2i TopLeft{ 0, 0 };
		Point2i TopRight{ 0, 0 };
		Point2i BottomRight{ 0, 0 };
		Point2i BottomLeft{ 0, 0 };

		// < Constructor >
		constexpr QuadRect2i() noexcept = default;

		QuadRect2i(
			const Point2i& tl,
			const Point2i& tr,
			const Point2i& br,
			const Point2i& bl) noexcept;

		QuadRect2i(
			int32_t tl_x, int32_t tl_y,
			int32_t tr_x, int32_t tr_y,
			int32_t br_x, int32_t br_y,
			int32_t bl_x, int32_t bl_y) noexcept;

		QuadRect2i(const QuadRect2i& rhs) = default;
		QuadRect2i(QuadRect2i&& rhs) noexcept = default;
		QuadRect2i& operator=(const QuadRect2i& rhs) = default;
		QuadRect2i& operator=(QuadRect2i&& rhs) noexcept = default;

		// < Methods >
		void Set(
			const Point2i& tl,
			const Point2i& tr,
			const Point2i& br,
			const Point2i& bl) noexcept;

		void Reset() noexcept;

		Point2f Center() const noexcept;
		float Area() const noexcept;
		bool Empty() const noexcept;

		bool IsConvex() const noexcept;
		bool Contains(const Point2i& p) const noexcept;

		void Offset(int32_t dx, int32_t dy) noexcept;
		void ScaleFromCenter(int32_t sx, int32_t sy) noexcept;

		Rect2i ComputeBoundingRect() const noexcept;
		Rect2i ComputeInnerRect() const noexcept;

		// < Conversion >
		Rect2i ToRect2i() const noexcept;

		// < Operators >
		bool operator==(const QuadRect2i& rhs) const noexcept;
		bool operator!=(const QuadRect2i& rhs) const noexcept;
	};
	}
}