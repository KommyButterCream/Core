#include "Size2d.h"
#include <algorithm>
#include <cmath>

using Core::ShapeType::Size2d;

constexpr Size2d::Size2d(double width, double height) noexcept
	: width(width), height(height)
{
}

void Size2d::Set(double width, double height) noexcept
{
	width = width;
	height = height;
}

void Size2d::Reset() noexcept
{
	width = 0.0;
	height = 0.0;
}

double Size2d::Area() const noexcept
{
	return width * height;
}

double Size2d::AspectRatio() const noexcept
{
	return (height != 0.0) ? (width / height) : 0.0;
}

bool Size2d::IsEmpty() const noexcept
{
	return width <= 0.0 || height <= 0.0;
}

bool Size2d::IsValid() const noexcept
{
	return width >= 0.0 && height >= 0.0;
}

void Size2d::ClampMin(double minValue) noexcept
{
	width = std::max(width, minValue);
	height = std::max(height, minValue);
}

void Size2d::ClampMax(double maxValue) noexcept
{
	width = std::min(width, maxValue);
	height = std::min(height, maxValue);
}

bool Size2d::EqualsEpsilon(const Size2d& rhs, double eps) const noexcept
{
	return std::abs(width - rhs.width) <= eps &&
		std::abs(height - rhs.height) <= eps;
}

constexpr Size2d Size2d::Zero() noexcept
{
	return Size2d(0.0, 0.0);
}

Size2d& Size2d::operator+=(const Size2d& rhs) noexcept
{
	width += rhs.width;
	height += rhs.height;
	return *this;
}

Size2d& Size2d::operator-=(const Size2d& rhs) noexcept
{
	width -= rhs.width;
	height -= rhs.height;
	return *this;
}

Size2d& Size2d::operator*=(double scalar) noexcept
{
	width *= scalar;
	height *= scalar;
	return *this;
}

Size2d& Size2d::operator/=(double scalar) noexcept
{
	if (scalar != 0.0)
	{
		width /= scalar;
		height /= scalar;
	}
	return *this;
}

Size2d Size2d::operator+(const Size2d& rhs) const noexcept
{
	return Size2d(width + rhs.width, height + rhs.height);
}

Size2d Size2d::operator-(const Size2d& rhs) const noexcept
{
	return Size2d(width - rhs.width, height - rhs.height);
}

Size2d Size2d::operator*(double scalar) const noexcept
{
	return Size2d(width * scalar, height * scalar);
}

Size2d Size2d::operator/(double scalar) const noexcept
{
	return (scalar != 0.0)
		? Size2d(width / scalar, height / scalar)
		: Size2d();
}

bool Size2d::operator==(const Size2d& rhs) const noexcept
{
	return width == rhs.width && height == rhs.height;
}

bool Size2d::operator!=(const Size2d& rhs) const noexcept
{
	return !(*this == rhs);
}