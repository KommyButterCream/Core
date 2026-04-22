#pragma once
#include "ShapeTypeBase.h"

namespace Core
{
	namespace ShapeType
	{
	struct Point4d
	{
	public:
		using value_type = double;

		// < Variables >
		double x{ 0.0 };
		double y{ 0.0 };
		double z{ 0.0 };
		double w{ 0.0 };

		// < Constructor & Destructor >
		constexpr Point4d() noexcept = default;
		Point4d(double x, double y, double z, double w) noexcept;

		Point4d(const Point4d&) = default;
		Point4d(Point4d&&) noexcept = default;
		Point4d& operator=(const Point4d&) = default;
		Point4d& operator=(Point4d&&) noexcept = default;

		~Point4d() = default;

		// < Methods >
		void Set(double x, double y, double z, double w) noexcept;
		void Reset() noexcept;

		void Offset(double dx, double dy, double dz, double dw) noexcept;

		// < Operators >
		Point4d operator+(const Point4d& rhs) const noexcept;
		Point4d operator-(const Point4d& rhs) const noexcept;
		Point4d operator*(double scalar) const noexcept;
		Point4d operator/(double scalar) const noexcept;

		Point4d& operator+=(const Point4d& rhs) noexcept;
		Point4d& operator-=(const Point4d& rhs) noexcept;
		Point4d& operator*=(double scalar) noexcept;
		Point4d& operator/=(double scalar) noexcept;

		bool operator==(const Point4d& rhs) const noexcept;
		bool operator!=(const Point4d& rhs) const noexcept;
	};
	}
}