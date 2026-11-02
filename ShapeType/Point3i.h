#pragma once
#include "ShapeTypeBase.h"

namespace Core
{
	namespace ShapeType
	{
	struct Point3i
	{
	public:
		using value_type = int32_t;

		// < Variables >
		int32_t x{ 0 };
		int32_t y{ 0 };
		int32_t z{ 0 };

		// < Constructor & Destructor >
		constexpr Point3i() noexcept = default;
		Point3i(int32_t x, int32_t y, int32_t z) noexcept;

		Point3i(const Point3i& rhs) = default;
		Point3i(Point3i&& rhs) noexcept = default;
		Point3i& operator=(const Point3i& rhs) = default;
		Point3i& operator=(Point3i&& rhs) noexcept = default;

		~Point3i() = default;

		// < Methods >
		void Set(int32_t x, int32_t y, int32_t z) noexcept;
		void Reset() noexcept;

		void Offset(int32_t dx, int32_t dy, int32_t dz) noexcept;
		void OffsetX(int32_t dx) noexcept;
		void OffsetY(int32_t dy) noexcept;
		void OffsetZ(int32_t dz) noexcept;

		// < Operators >
		Point3i operator+(const Point3i& rhs) const noexcept;
		Point3i operator-(const Point3i& rhs) const noexcept;
		Point3i operator*(int32_t scalar) const noexcept;
		Point3i operator/(int32_t scalar) const noexcept;

		Point3i& operator+=(const Point3i& rhs) noexcept;
		Point3i& operator-=(const Point3i& rhs) noexcept;
		Point3i& operator*=(int32_t scalar) noexcept;
		Point3i& operator/=(int32_t scalar) noexcept;

		bool operator==(const Point3i& rhs) const noexcept;
		bool operator!=(const Point3i& rhs) const noexcept;
	};
	}
}