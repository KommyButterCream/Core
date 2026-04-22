#pragma once
#include "ShapeTypeBase.h"

namespace Core
{
	namespace ShapeType
	{
	struct Point2f
	{
	public:
		using value_type = float;

		// < Variables >
		float x{ 0.0f };
		float y{ 0.0f };

		// < Constructor & Destructor >
		constexpr Point2f() noexcept = default;
		Point2f(float x, float y) noexcept;

		Point2f(const Point2f& rhs) = default;
		Point2f(Point2f&& rhs) noexcept = default;
		Point2f& operator=(const Point2f& rhs) = default;
		Point2f& operator=(Point2f&& rhs) noexcept = default;

		~Point2f() = default;

		// < Methods >
		void Set(float x, float y) noexcept;
		void Reset() noexcept;

		void Offset(float dx, float dy) noexcept;
		void OffsetX(float dx) noexcept;
		void OffsetY(float dy) noexcept;

		// < Operators >
		Point2f operator+(const Point2f& rhs) const noexcept;
		Point2f operator-(const Point2f& rhs) const noexcept;
		Point2f operator*(float scalar) const noexcept;
		Point2f operator/(float scalar) const noexcept;

		Point2f& operator+=(const Point2f& rhs) noexcept;
		Point2f& operator-=(const Point2f& rhs) noexcept;
		Point2f& operator*=(float scalar) noexcept;
		Point2f& operator/=(float scalar) noexcept;

		bool operator==(const Point2f& rhs) const noexcept;
		bool operator!=(const Point2f& rhs) const noexcept;
	};
	}
}