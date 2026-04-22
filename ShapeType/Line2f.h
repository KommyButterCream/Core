#pragma once
#include "ShapeTypeBase.h"

namespace Core
{
	namespace ShapeType
	{
	struct Line2f
	{
	public:
		using value_type = float;

		// < Variables >
		float sx{ 0.0f };
		float sy{ 0.0f };
		float ex{ 0.0f };
		float ey{ 0.0f };

		// < Constructor & Destructor >
		constexpr Line2f() noexcept = default;
		Line2f(float sx, float sy, float ex, float ey) noexcept;

		Line2f(const Line2f& rhs) = default;
		Line2f(Line2f&& rhs) noexcept = default;
		Line2f& operator=(const Line2f& rhs) = default;
		Line2f& operator=(Line2f&& rhs) noexcept = default;

		~Line2f() = default;

		// < Methods >
		void Set(float sx, float sy, float ex, float ey) noexcept;
		void SetStart(float x, float y) noexcept;
		void SetEnd(float x, float y) noexcept;
		void Reset() noexcept;

		void Offset(float dx, float dy) noexcept;
		void OffsetX(float dx) noexcept;
		void OffsetY(float dy) noexcept;

		float Length() const noexcept;
		float LengthSq() const noexcept;

		float Angle() const noexcept;

		void Direction(float& dx, float& dy) const noexcept;
		void Center(float& cx, float& cy) const noexcept;
	};
	}
}