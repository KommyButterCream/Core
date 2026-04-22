#pragma once
#include "ShapeTypeBase.h"

namespace Core
{
	namespace ShapeType
	{
	struct Point3d
	{
	public:
		using value_type = double;

		// < Variables >
		double x{ 0.0 };
		double y{ 0.0 };
		double z{ 0.0 };

		// < Constructor & Destructor >
		constexpr Point3d() noexcept = default;
		Point3d(double x, double y, double z) noexcept;

		Point3d(const Point3d& rhs) = default;
		Point3d(Point3d&& rhs) noexcept = default;
		Point3d& operator=(const Point3d& rhs) = default;
		Point3d& operator=(Point3d&& rhs) noexcept = default;

		~Point3d() = default;

		// < Methods >
		void Set(double x, double y, double z) noexcept;
		void Reset() noexcept;

		void Offset(double dx, double dy, double dz) noexcept;
		void OffsetX(double dx) noexcept;
		void OffsetY(double dy) noexcept;
		void OffsetZ(double dz) noexcept;

		// < Operators >
		Point3d operator+(const Point3d& rhs) const noexcept;
		Point3d operator-(const Point3d& rhs) const noexcept;
		Point3d operator*(double scalar) const noexcept;
		Point3d operator/(double scalar) const noexcept;

		Point3d& operator+=(const Point3d& rhs) noexcept;
		Point3d& operator-=(const Point3d& rhs) noexcept;
		Point3d& operator*=(double scalar) noexcept;
		Point3d& operator/=(double scalar) noexcept;

		bool operator==(const Point3d& rhs) const noexcept;
		bool operator!=(const Point3d& rhs) const noexcept;
	};
	}
}