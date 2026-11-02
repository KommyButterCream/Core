#pragma once
#include "ShapeTypeBase.h"

namespace Core
{
	namespace ShapeType
	{
	struct Point2i
	{
	public:
		using value_type = int32_t;

		// < Variables >
		int32_t x{ 0 };
		int32_t y{ 0 };

		// < Constructor & Destructor >
		constexpr Point2i() noexcept = default;
		Point2i(int32_t x, int32_t y) noexcept;

		Point2i(const Point2i& rhs) = default;
		Point2i(Point2i&& rhs) noexcept = default;
		Point2i& operator=(const Point2i& rhs) = default;
		Point2i& operator=(Point2i&& rhs) noexcept = default;

		~Point2i() = default;

		// < Methods >
		void Set(int32_t x, int32_t y) noexcept;
		void Reset() noexcept;

		void Offset(int32_t dx, int32_t dy) noexcept;
		void OffsetX(int32_t dx) noexcept;
		void OffsetY(int32_t dy) noexcept;

		// < Operators >
		Point2i operator+(const Point2i& rhs) const noexcept;
		Point2i operator-(const Point2i& rhs) const noexcept;
		Point2i operator*(int32_t scalar) const noexcept;
		Point2i operator/(int32_t scalar) const noexcept;

		Point2i& operator+=(const Point2i& rhs) noexcept;
		Point2i& operator-=(const Point2i& rhs) noexcept;
		Point2i& operator*=(int32_t scalar) noexcept;
		Point2i& operator/=(int32_t scalar) noexcept;

		bool operator==(const Point2i& rhs) const noexcept;
		bool operator!=(const Point2i& rhs) const noexcept;
	};

	using Point = Point2i;
	}
}