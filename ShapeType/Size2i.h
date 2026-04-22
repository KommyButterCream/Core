#pragma once

#include "ShapeTypeBase.h"
#include <stdint.h>

namespace Core
{
	namespace ShapeType
	{
	struct Size2i;
	struct Size2d;

	struct Size2i
	{
	public:
		using value_type = int32_t;

		// < Constructors >
		constexpr Size2i() noexcept = default;
		constexpr Size2i(int32_t width, int32_t height) noexcept;

		Size2i(const Size2i&) = default;
		Size2i(Size2i&&) noexcept = default;
		Size2i& operator=(const Size2i&) = default;
		Size2i& operator=(Size2i&&) noexcept = default;
		~Size2i() = default;

		// < Members >
		int32_t width{ 0 };
		int32_t height{ 0 };

		// < Basic Operations >
		void Set(int32_t width, int32_t height) noexcept;
		void Reset() noexcept;

		// < Queries >
		int32_t Area() const noexcept;
		int32_t AspectRatio() const noexcept;
		bool IsEmpty() const noexcept;      // width <= 0 || height <= 0
		bool IsValid() const noexcept;      // width >= 0 && height >= 0

		// < Utilities >
		void ClampMin(int32_t minValue) noexcept;
		void ClampMax(int32_t maxValue) noexcept;
		bool EqualsEpsilon(const Size2i& rhs) const noexcept;

		// < Static >
		static constexpr Size2i Zero() noexcept;

		// < Operators >
		Size2i& operator += (const Size2i& rhs) noexcept;
		Size2i& operator -= (const Size2i& rhs) noexcept;
		Size2i& operator *= (int32_t scalar) noexcept;
		Size2i& operator /= (int32_t scalar) noexcept;

		Size2i operator + (const Size2i& rhs) const noexcept;
		Size2i operator - (const Size2i& rhs) const noexcept;
		Size2i operator * (int32_t scalar) const noexcept;
		Size2i operator / (int32_t scalar) const noexcept;

		bool operator == (const Size2i& rhs) const noexcept;
		bool operator != (const Size2i& rhs) const noexcept;
	};
	}
}