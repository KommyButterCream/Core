#pragma once

#include "ShapeTypeBase.h"

#include <array>

#include "Point2d.h"
#include "Size2d.h"

namespace Core
{
	namespace ShapeType
	{
	struct Rect2d;

	struct RotatedRect2d
	{
		// < Type Definitions >
		using value_type = double;
		using VertexArray = std::array<Point2d, 4>;

		enum VertexIndex : int32_t
		{
			TopLeft = 0,
			TopRight,
			BottomRight,
			BottomLeft
		};

		// < Variables >
		Point2d center{ 0.0, 0.0 };    // rectangle center
		Size2d  size{ 0.0, 0.0 };      // width / height
		double   angleRad{ 0.0 };  // rotation angle (radian, CCW)

		// < Constructor & Destructor >
		constexpr RotatedRect2d() noexcept = default;
		RotatedRect2d(const Point2d& center,
			const Size2d& size,
			double angleRad) noexcept;
		RotatedRect2d(double cx, double cy,
			double width, double height,
			double angleRad) noexcept;

		RotatedRect2d(const RotatedRect2d&) = default;
		RotatedRect2d(RotatedRect2d&&) noexcept = default;
		RotatedRect2d& operator=(const RotatedRect2d&) = default;
		RotatedRect2d& operator=(RotatedRect2d&&) noexcept = default;
		~RotatedRect2d() = default;

		// < Set / Reset >
		constexpr void Set(double cx, double cy,
			double width, double height,
			double angleRad) noexcept;
		constexpr void Reset() noexcept;

		// < Getters >
		constexpr double Width()  const noexcept;
		constexpr double Height() const noexcept;
		constexpr const Point2d& GetCenter() const noexcept;
		constexpr const Size2d& GetSize() const noexcept;
		constexpr double GetAngle() const noexcept;

		// < Geometry >
		VertexArray GetVertices() const noexcept;
		Rect2d      GetBoundingRect() const noexcept;

		// < Transform >
		constexpr void Rotate(double deltaAngleRad) noexcept;
		constexpr void SetAngle(double angleRad) noexcept;
		constexpr void Translate(double dx, double dy) noexcept;

	public:
		// < Utility >
		constexpr bool IsValid() const noexcept;
	};
	}
}