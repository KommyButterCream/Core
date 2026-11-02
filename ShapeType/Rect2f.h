#pragma once
#include "ShapeTypeBase.h"

namespace Core
{
	namespace ShapeType
	{
	struct Point2f;
	struct Size2f;

	struct Rect2f
	{
		using value_type = float;

		// < Variables >
		float left{ 0.0f };
		float top{ 0.0f };
		float right{ 0.0f };
		float bottom{ 0.0f };

		// < Constructor & Destructor >
		constexpr Rect2f() noexcept = default;
		Rect2f(float l, float t, float r, float b) noexcept;

		Rect2f(const Rect2f&) = default;
		Rect2f(Rect2f&&) noexcept = default;
		Rect2f& operator=(const Rect2f&) = default;
		Rect2f& operator=(Rect2f&&) noexcept = default;

		~Rect2f() = default;

		// < Methods >
		void Set(float l, float t, float r, float b) noexcept;
		void Reset() noexcept;

		float Width() const noexcept;
		float Height() const noexcept;
		Size2f Size() const noexcept;
		float Area() const noexcept;
		bool Empty() const noexcept;

		Point2f Center() const noexcept;

		// < Geometry >
		void Normalize() noexcept;

		bool Contains(float x, float y) const noexcept;
		bool Contains(const Point2f& p) const noexcept;
		bool Contains(const Rect2f& r) const noexcept;
		bool Intersects(const Rect2f& r) const noexcept;

		Rect2f Intersect(const Rect2f& r) const noexcept;
		Rect2f Unite(const Rect2f& r) const noexcept;

		Rect2f& IntersectWith(const Rect2f& r) noexcept;
		Rect2f& UniteWith(const Rect2f& r) noexcept;

		// < Transform >
		void Offset(float dx, float dy) noexcept;
		void Inflate(float dx, float dy) noexcept;
		void Deflate(float dx, float dy) noexcept;

		void ScaleFromCenter(float sx, float sy) noexcept;
		void MoveCenterTo(float cx, float cy) noexcept;

		// < Operators >
		Rect2f operator&(const Rect2f& r) const noexcept;
		Rect2f operator|(const Rect2f& r) const noexcept;
		Rect2f& operator&=(const Rect2f& r) noexcept;
		Rect2f& operator|=(const Rect2f& r) noexcept;

		bool operator==(const Rect2f& r) const noexcept;
		bool operator!=(const Rect2f& r) const noexcept;
	};
	}
}