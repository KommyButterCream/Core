#pragma once
#include "ShapeTypeBase.h"

namespace Core
{
	namespace ShapeType
	{
	struct Line2d
	{
	public:
		using value_type = double;

		// < Variables >
		double sx{ 0.0 };
		double sy{ 0.0 };
		double ex{ 0.0 };
		double ey{ 0.0 };

		// < Constructor & Destructor >
		constexpr Line2d() noexcept = default;
		Line2d(double sx, double sy, double ex, double ey) noexcept;

		Line2d(const Line2d& rhs) = default;
		Line2d(Line2d&& rhs) noexcept = default;
		Line2d& operator=(const Line2d& rhs) = default;
		Line2d& operator=(Line2d&& rhs) noexcept = default;

		~Line2d() = default;

		// < Methods >
		void Set(double sx, double sy, double ex, double ey) noexcept;
		void SetStart(double x, double y) noexcept;
		void SetEnd(double x, double y) noexcept;
		void Reset() noexcept;

		void Offset(double dx, double dy) noexcept;
		void OffsetX(double dx) noexcept;
		void OffsetY(double dy) noexcept;

		double Length() const noexcept;
		double LengthSq() const noexcept;

		double Angle() const noexcept;

		void Direction(double& dx, double& dy) const noexcept;
		void Center(double& cx, double& cy) const noexcept;
	};
	}
}