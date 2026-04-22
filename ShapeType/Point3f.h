#pragma once
#include "ShapeTypeBase.h"

namespace Core
{
	namespace ShapeType
	{
	struct Point3f
	{
	public:
		using value_type = float;

		// < Variables >
		float x{ 0.0f };
		float y{ 0.0f };
		float z{ 0.0f };

		// < Constructor & Destructor >
		constexpr Point3f() noexcept = default;
		Point3f(float x, float y, float z) noexcept;

		Point3f(const Point3f& rhs) = default;
		Point3f(Point3f&& rhs) noexcept = default;
		Point3f& operator=(const Point3f& rhs) = default;
		Point3f& operator=(Point3f&& rhs) noexcept = default;

		~Point3f() = default;

		// < Methods >
		void Set(float x, float y, float z) noexcept;
		void Reset() noexcept;

		void Offset(float dx, float dy, float dz) noexcept;
		void OffsetX(float dx) noexcept;
		void OffsetY(float dy) noexcept;
		void OffsetZ(float dz) noexcept;

		// < Operators >
		Point3f operator+(const Point3f& rhs) const noexcept;
		Point3f operator-(const Point3f& rhs) const noexcept;
		Point3f operator*(float scalar) const noexcept;
		Point3f operator/(float scalar) const noexcept;

		Point3f& operator+=(const Point3f& rhs) noexcept;
		Point3f& operator-=(const Point3f& rhs) noexcept;
		Point3f& operator*=(float scalar) noexcept;
		Point3f& operator/=(float scalar) noexcept;

		bool operator==(const Point3f& rhs) const noexcept;
		bool operator!=(const Point3f& rhs) const noexcept;
	};
	}
}