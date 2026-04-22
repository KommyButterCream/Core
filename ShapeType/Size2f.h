#pragma once

#include "ShapeTypeBase.h"

namespace Core
{
	namespace ShapeType
	{
	struct Size2i;
	struct Size2d;

	struct Size2f
	{
	public:
		using value_type = float;

		// < Constructors >
		constexpr Size2f() noexcept = default;
		constexpr Size2f(float width, float height) noexcept;

		Size2f(const Size2f&) = default;
		Size2f(Size2f&&) noexcept = default;
		Size2f& operator=(const Size2f&) = default;
		Size2f& operator=(Size2f&&) noexcept = default;
		~Size2f() = default;

		// < Members >
		float width{ 0.0f };
		float height{ 0.0f };

		// < Basic Operations >
		void Set(float width, float height) noexcept;
		void Reset() noexcept;

		// < Queries >
		float Area() const noexcept;
		float AspectRatio() const noexcept;
		bool IsEmpty() const noexcept;      // width <= 0 || height <= 0
		bool IsValid() const noexcept;      // width >= 0 && height >= 0

		// < Utilities >
		void ClampMin(float minValue) noexcept;
		void ClampMax(float maxValue) noexcept;
		bool EqualsEpsilon(const Size2f& rhs, float eps = 1e-6f) const noexcept;

		// < Static >
		static constexpr Size2f Zero() noexcept;

		// < Operators >
		Size2f& operator += (const Size2f& rhs) noexcept;
		Size2f& operator -= (const Size2f& rhs) noexcept;
		Size2f& operator *= (float scalar) noexcept;
		Size2f& operator /= (float scalar) noexcept;

		Size2f operator + (const Size2f& rhs) const noexcept;
		Size2f operator - (const Size2f& rhs) const noexcept;
		Size2f operator * (float scalar) const noexcept;
		Size2f operator / (float scalar) const noexcept;

		bool operator == (const Size2f& rhs) const noexcept;
		bool operator != (const Size2f& rhs) const noexcept;
	};
	}
}