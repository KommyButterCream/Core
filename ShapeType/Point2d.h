#pragma once
#include "ShapeTypeBase.h"

namespace Core
{
	namespace ShapeType
	{
	struct Point2d
	{
	public:
		using value_type = double;

		// < Variables >
		double x{ 0.0 };
		double y{ 0.0 };

		// < Constructor & Destructor >
		constexpr Point2d() noexcept = default;
		Point2d(double x, double y) noexcept;

		Point2d(const Point2d& rhs) = default;
		Point2d(Point2d&& rhs) noexcept = default;
		Point2d& operator=(const Point2d& rhs) = default;
		Point2d& operator=(Point2d&& rhs) noexcept = default;

		~Point2d() = default;

		// < Methods >
		void Set(double x, double y) noexcept;
		void Reset() noexcept;

		void Offset(double dx, double dy) noexcept;
		void OffsetX(double dx) noexcept;
		void OffsetY(double dy) noexcept;

		// < Operators >
		Point2d operator+(const Point2d& rhs) const noexcept;
		Point2d operator-(const Point2d& rhs) const noexcept;
		Point2d operator*(double scalar) const noexcept;
		Point2d operator/(double scalar) const noexcept;

		Point2d& operator+=(const Point2d& rhs) noexcept;
		Point2d& operator-=(const Point2d& rhs) noexcept;
		Point2d& operator*=(double scalar) noexcept;
		Point2d& operator/=(double scalar) noexcept;

		bool operator==(const Point2d& rhs) const noexcept;
		bool operator!=(const Point2d& rhs) const noexcept;
	};
	}
}