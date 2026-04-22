#pragma once
#include "ShapeTypeBase.h"

namespace Core
{
	namespace ShapeType
	{
	struct Point4i
	{
	public:
		using value_type = int32_t;

		// < Variables >
		int32_t x{ 0 };
		int32_t y{ 0 };
		int32_t z{ 0 };
		int32_t w{ 0 };

		// < Constructor & Destructor >
		constexpr Point4i() noexcept = default;
		Point4i(int32_t x, int32_t y, int32_t z, int32_t w) noexcept;

		Point4i(const Point4i& rhs) = default;
		Point4i(Point4i&& rhs) noexcept = default;
		Point4i& operator=(const Point4i& rhs) = default;
		Point4i& operator=(Point4i&& rhs) noexcept = default;

		~Point4i() = default;

		// < Methods >
		void Set(int32_t x, int32_t y, int32_t z, int32_t w) noexcept;
		void Reset() noexcept;

		void Offset(int32_t dx, int32_t dy, int32_t dz, int32_t dw) noexcept;
		void OffsetX(int32_t dx) noexcept;
		void OffsetY(int32_t dy) noexcept;
		void OffsetZ(int32_t dz) noexcept;
		void OffsetW(int32_t dw) noexcept;

		// < Operators >
		Point4i operator+(const Point4i& rhs) const noexcept;
		Point4i operator-(const Point4i& rhs) const noexcept;
		Point4i operator*(int32_t scalar) const noexcept;
		Point4i operator/(int32_t scalar) const noexcept;

		Point4i& operator+=(const Point4i& rhs) noexcept;
		Point4i& operator-=(const Point4i& rhs) noexcept;
		Point4i& operator*=(int32_t scalar) noexcept;
		Point4i& operator/=(int32_t scalar) noexcept;

		bool operator==(const Point4i& rhs) const noexcept;
		bool operator!=(const Point4i& rhs) const noexcept;
	};
	}
}