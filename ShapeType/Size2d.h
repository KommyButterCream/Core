#pragma once

#include "ShapeTypeBase.h"

namespace Core
{
	namespace ShapeType
	{
	struct Size2i;
	struct Size2d;

	struct Size2d
	{
	public:
		using value_type = double;

		// < Constructors >
		constexpr Size2d() noexcept = default;
		constexpr Size2d(double width, double height) noexcept;

		Size2d(const Size2d&) = default;
		Size2d(Size2d&&) noexcept = default;
		Size2d& operator=(const Size2d&) = default;
		Size2d& operator=(Size2d&&) noexcept = default;
		~Size2d() = default;

		// < Members >
		double width{ 0.0 };
		double height{ 0.0 };

		// < Basic Operations >
		void Set(double width, double height) noexcept;
		void Reset() noexcept;

		// < Queries >
		double Area() const noexcept;
		double AspectRatio() const noexcept;
		bool IsEmpty() const noexcept;      // width <= 0 || height <= 0
		bool IsValid() const noexcept;      // width >= 0 && height >= 0

		// < Utilities >
		void ClampMin(double minValue) noexcept;
		void ClampMax(double maxValue) noexcept;
		bool EqualsEpsilon(const Size2d& rhs, double eps = 1e-6f) const noexcept;

		// < Static >
		static constexpr Size2d Zero() noexcept;

		// < Operators >
		Size2d& operator += (const Size2d& rhs) noexcept;
		Size2d& operator -= (const Size2d& rhs) noexcept;
		Size2d& operator *= (double scalar) noexcept;
		Size2d& operator /= (double scalar) noexcept;

		Size2d operator + (const Size2d& rhs) const noexcept;
		Size2d operator - (const Size2d& rhs) const noexcept;
		Size2d operator * (double scalar) const noexcept;
		Size2d operator / (double scalar) const noexcept;

		bool operator == (const Size2d& rhs) const noexcept;
		bool operator != (const Size2d& rhs) const noexcept;
	};
	}
}