#pragma once
#include "ShapeTypeBase.h"

namespace Core
{
	namespace ShapeType
	{
	struct Point4f
	{
	public:
		using value_type = float;

		// < Variables >
		float x{ 0.0f };
		float y{ 0.0f };
		float z{ 0.0f };
		float w{ 0.0f };

		// < Constructor & Destructor >
		constexpr Point4f() noexcept = default;
		Point4f(float x, float y, float z, float w) noexcept;

		Point4f(const Point4f& rhs) = default;
		Point4f(Point4f&& rhs) noexcept = default;
		Point4f& operator=(const Point4f& rhs) = default;
		Point4f& operator=(Point4f&& rhs) noexcept = default;

		~Point4f() = default;

		// < Methods >
		void Set(float x, float y, float z, float w) noexcept;
		void Reset() noexcept;

		void Offset(float dx, float dy, float dz, float dw) noexcept;
		void OffsetX(float dx) noexcept;
		void OffsetY(float dy) noexcept;
		void OffsetZ(float dz) noexcept;
		void OffsetW(float dw) noexcept;

		// < Operators >
		Point4f operator+(const Point4f& rhs) const noexcept;
		Point4f operator-(const Point4f& rhs) const noexcept;
		Point4f operator*(float scalar) const noexcept;
		Point4f operator/(float scalar) const noexcept;

		Point4f& operator+=(const Point4f& rhs) noexcept;
		Point4f& operator-=(const Point4f& rhs) noexcept;
		Point4f& operator*=(float scalar) noexcept;
		Point4f& operator/=(float scalar) noexcept;

		bool operator==(const Point4f& rhs) const noexcept;
		bool operator!=(const Point4f& rhs) const noexcept;
	};
	}
}