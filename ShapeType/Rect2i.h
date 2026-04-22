#pragma once
#include "ShapeTypeBase.h"
#include <stdint.h>

namespace Core
{
	namespace ShapeType
	{
	struct Point2f;
	struct Size2f;

	struct Rect2i
	{
		using value_type = int32_t;

		// < Variables >
		int32_t left{ 0 };
		int32_t top{ 0 };
		int32_t right{ 0 };
		int32_t bottom{ 0 };

		// < Constructor & Destructor >
		constexpr Rect2i() noexcept = default;
		Rect2i(int32_t l, int32_t t, int32_t r, int32_t b) noexcept;

		Rect2i(const Rect2i&) = default;
		Rect2i(Rect2i&&) noexcept = default;
		Rect2i& operator=(const Rect2i&) = default;
		Rect2i& operator=(Rect2i&&) noexcept = default;

		~Rect2i() = default;

		// < Methods >
		void Set(int32_t l, int32_t t, int32_t r, int32_t b) noexcept;
		void Reset() noexcept;

		int32_t Width() const noexcept;
		int32_t Height() const noexcept;
		Size2f Size() const noexcept;
		int32_t Area() const noexcept;
		bool Empty() const noexcept;

		Point2f Center() const noexcept;

		// < Geometry >
		void Normalize() noexcept;

		bool Contains(float x, float y) const noexcept;
		bool Contains(int32_t x, int32_t y) const noexcept;
		bool Contains(const Point2f& p) const noexcept;
		bool Contains(const Rect2i& r) const noexcept;
		bool Intersects(const Rect2i& r) const noexcept;

		Rect2i Intersect(const Rect2i& r) const noexcept;
		Rect2i Unite(const Rect2i& r) const noexcept;

		Rect2i& IntersectWith(const Rect2i& r) noexcept;
		Rect2i& UniteWith(const Rect2i& r) noexcept;

		// < Transform >
		void Offset(int32_t dx, int32_t dy) noexcept;
		void Inflate(int32_t dx, int32_t dy) noexcept;
		void Deflate(int32_t dx, int32_t dy) noexcept;

		void ScaleFromCenter(int32_t sx, int32_t sy) noexcept;
		void MoveCenterTo(int32_t cx, int32_t cy) noexcept;

		// < Operators >
		Rect2i operator&(const Rect2i& r) const noexcept;
		Rect2i operator|(const Rect2i& r) const noexcept;
		Rect2i& operator&=(const Rect2i& r) noexcept;
		Rect2i& operator|=(const Rect2i& r) noexcept;

		bool operator==(const Rect2i& r) const noexcept;
		bool operator!=(const Rect2i& r) const noexcept;
	};

	using Rect = Rect2i;
	}
}