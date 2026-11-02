#pragma once
#include "ShapeTypeBase.h"

namespace Core
{
	namespace ShapeType
	{
	struct Point2d;
	struct Size2d;

	struct Rect2d
	{
		using value_type = double;

		// < Variables >
		double left{ 0.0 };
		double top{ 0.0 };
		double right{ 0.0 };
		double bottom{ 0.0 };

		// < Constructor & Destructor >
		constexpr Rect2d() noexcept = default;
		Rect2d(double l, double t, double r, double b) noexcept;

		Rect2d(const Rect2d&) = default;
		Rect2d(Rect2d&&) noexcept = default;
		Rect2d& operator=(const Rect2d&) = default;
		Rect2d& operator=(Rect2d&&) noexcept = default;

		~Rect2d() = default;

		// < Methods >
		void Set(double l, double t, double r, double b) noexcept;
		void Reset() noexcept;

		double Width() const noexcept;
		double Height() const noexcept;
		Size2d Size() const noexcept;
		double Area() const noexcept;
		bool Empty() const noexcept;

		Point2d Center() const noexcept;

		// < Geometry >
		void Normalize() noexcept;

		bool Contains(double x, double y) const noexcept;
		bool Contains(const Point2d& p) const noexcept;
		bool Contains(const Rect2d& r) const noexcept;
		bool Intersects(const Rect2d& r) const noexcept;

		Rect2d Intersect(const Rect2d& r) const noexcept;
		Rect2d Unite(const Rect2d& r) const noexcept;

		Rect2d& IntersectWith(const Rect2d& r) noexcept;
		Rect2d& UniteWith(const Rect2d& r) noexcept;

		// < Transform >
		void Offset(double dx, double dy) noexcept;
		void Inflate(double dx, double dy) noexcept;
		void Deflate(double dx, double dy) noexcept;

		void ScaleFromCenter(double sx, double sy) noexcept;
		void MoveCenterTo(double cx, double cy) noexcept;

		// < Operators >
		Rect2d operator&(const Rect2d& r) const noexcept;
		Rect2d operator|(const Rect2d& r) const noexcept;
		Rect2d& operator&=(const Rect2d& r) noexcept;
		Rect2d& operator|=(const Rect2d& r) noexcept;

		bool operator==(const Rect2d& r) const noexcept;
		bool operator!=(const Rect2d& r) const noexcept;
	};
	}
}