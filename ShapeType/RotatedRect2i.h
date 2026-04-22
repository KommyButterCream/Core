#pragma once

#include "ShapeTypeBase.h"

#include <stdint.h>
#include <array>

#include "Point2i.h"
#include "Size2i.h"

namespace Core
{
	namespace ShapeType
	{
	struct Rect2i;

	struct RotatedRect2i
	{
		// < Type Definitions >
		using value_type = int32_t;
		using VertexArray = std::array<Point2i, 4>;

		enum VertexIndex : int32_t
		{
			TopLeft = 0,
			TopRight,
			BottomRight,
			BottomLeft
		};

		// < Variables >
		Point2i center{ 0, 0 };    // rectangle center
		Size2i  size{ 0, 0 };      // width / height
		float   angleRad{ 0.0f };  // rotation angle (radian, CCW)

		// < Constructor & Destructor >
		constexpr RotatedRect2i() noexcept = default;
		RotatedRect2i(const Point2i& center,
			const Size2i& size,
			float angleRad) noexcept;
		RotatedRect2i(int32_t cx, int32_t cy,
			int32_t width, int32_t height,
			float angleRad) noexcept;

		RotatedRect2i(const RotatedRect2i&) = default;
		RotatedRect2i(RotatedRect2i&&) noexcept = default;
		RotatedRect2i& operator=(const RotatedRect2i&) = default;
		RotatedRect2i& operator=(RotatedRect2i&&) noexcept = default;
		~RotatedRect2i() = default;

		// < Set / Reset >
		constexpr void Set(int32_t cx, int32_t cy,
			int32_t width, int32_t height,
			float angleRad) noexcept;
		constexpr void Reset() noexcept;

		// < Getters >
		constexpr int32_t Width()  const noexcept;
		constexpr int32_t Height() const noexcept;
		constexpr const Point2i& GetCenter() const noexcept;
		constexpr const Size2i& GetSize() const noexcept;
		constexpr float GetAngle() const noexcept;

		// < Geometry >
		VertexArray GetVertices() const noexcept;
		Rect2i      GetBoundingRect() const noexcept;

		// < Transform >
		constexpr void Rotate(float deltaAngleRad) noexcept;
		constexpr void SetAngle(float angleRad) noexcept;
		constexpr void Translate(int32_t dx, int32_t dy) noexcept;

	public:
		// < Utility >
		constexpr bool IsValid() const noexcept;
	};
	}
}