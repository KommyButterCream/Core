#pragma once

#include "ShapeTypeBase.h"

#include <array>

#include "Point2f.h"
#include "Size2f.h"

namespace Core
{
	namespace ShapeType
	{
	struct Rect2f;

	struct RotatedRect2f
	{
		// < Type Definitions >
		using value_type = float;
		using VertexArray = std::array<Point2f, 4>;

		enum VertexIndex : int32_t
		{
			TopLeft = 0,
			TopRight,
			BottomRight,
			BottomLeft
		};

		// < Variables >
		Point2f center{ 0.0f, 0.0f };    // rectangle center
		Size2f  size{ 0.0f, 0.0f };      // width / height
		float   angleRad{ 0.0f };  // rotation angle (radian, CCW)

		// < Constructor & Destructor >
		constexpr RotatedRect2f() noexcept = default;
		RotatedRect2f(const Point2f& center,
			const Size2f& size,
			float angleRad) noexcept;
		RotatedRect2f(float cx, float cy,
			float width, float height,
			float angleRad) noexcept;

		RotatedRect2f(const RotatedRect2f&) = default;
		RotatedRect2f(RotatedRect2f&&) noexcept = default;
		RotatedRect2f& operator=(const RotatedRect2f&) = default;
		RotatedRect2f& operator=(RotatedRect2f&&) noexcept = default;
		~RotatedRect2f() = default;

		// < Set / Reset >
		constexpr void Set(float cx, float cy,
			float width, float height,
			float angleRad) noexcept;
		constexpr void Reset() noexcept;

		// < Getters >
		constexpr float Width()  const noexcept;
		constexpr float Height() const noexcept;
		constexpr const Point2f& GetCenter() const noexcept;
		constexpr const Size2f& GetSize() const noexcept;
		constexpr float GetAngle() const noexcept;

		// < Geometry >
		VertexArray GetVertices() const noexcept;
		Rect2f      GetBoundingRect() const noexcept;

		// < Transform >
		constexpr void Rotate(float deltaAngleRad) noexcept;
		constexpr void SetAngle(float angleRad) noexcept;
		constexpr void Translate(float dx, float dy) noexcept;

	public:
		// < Utility >
		constexpr bool IsValid() const noexcept;
	};
	}
}