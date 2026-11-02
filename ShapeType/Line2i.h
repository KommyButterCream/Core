#pragma once
#include "ShapeTypeBase.h"
#include <stdint.h>

namespace Core
{
	namespace ShapeType
	{
	struct Line2i
	{
	public:
		using value_type = int32_t;

		// < Variables >
		int32_t sx{ 0 };
		int32_t sy{ 0 };
		int32_t ex{ 0 };
		int32_t ey{ 0 };

		// < Constructor & Destructor >
		constexpr Line2i() noexcept = default;
		Line2i(int32_t sx, int32_t sy, int32_t ex, int32_t ey) noexcept;

		Line2i(const Line2i& rhs) = default;
		Line2i(Line2i&& rhs) noexcept = default;
		Line2i& operator=(const Line2i& rhs) = default;
		Line2i& operator=(Line2i&& rhs) noexcept = default;

		~Line2i() = default;

		// < Methods >
		void Set(int32_t sx, int32_t sy, int32_t ex, int32_t ey) noexcept;
		void SetStart(int32_t x, int32_t y) noexcept;
		void SetEnd(int32_t x, int32_t y) noexcept;
		void Reset() noexcept;

		void Offset(int32_t dx, int32_t dy) noexcept;
		void OffsetX(int32_t dx) noexcept;
		void OffsetY(int32_t dy) noexcept;

		float Length() const noexcept;
		float LengthSq() const noexcept;

		float Angle() const noexcept;

		void Direction(int32_t& dx, int32_t& dy) const noexcept;
		void Center(float& cx, float& cy) const noexcept;
	};

	using Line = Line2i;
	}
}