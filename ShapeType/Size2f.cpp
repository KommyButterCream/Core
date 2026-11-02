#include "Size2f.h"
#include <algorithm>
#include <cmath>

using Core::ShapeType::Size2f;

constexpr Size2f::Size2f(float width, float height) noexcept
	: width(width), height(height)
{
}

void Size2f::Set(float width, float height) noexcept
{
	width = width;
	height = height;
}

void Size2f::Reset() noexcept
{
	width = 0.0f;
	height = 0.0f;
}

float Size2f::Area() const noexcept
{
	return width * height;
}

float Size2f::AspectRatio() const noexcept
{
	return (height != 0.0f) ? (width / height) : 0.0f;
}

bool Size2f::IsEmpty() const noexcept
{
	return width <= 0.0f || height <= 0.0f;
}

bool Size2f::IsValid() const noexcept
{
	return width >= 0.0f && height >= 0.0f;
}

void Size2f::ClampMin(float minValue) noexcept
{
	width = std::max(width, minValue);
	height = std::max(height, minValue);
}

void Size2f::ClampMax(float maxValue) noexcept
{
	width = std::min(width, maxValue);
	height = std::min(height, maxValue);
}

bool Size2f::EqualsEpsilon(const Size2f& rhs, float eps) const noexcept
{
	return std::abs(width - rhs.width) <= eps &&
		std::abs(height - rhs.height) <= eps;
}

constexpr Size2f Size2f::Zero() noexcept
{
	return Size2f(0.0f, 0.0f);
}

Size2f& Size2f::operator+=(const Size2f& rhs) noexcept
{
	width += rhs.width;
	height += rhs.height;
	return *this;
}

Size2f& Size2f::operator-=(const Size2f& rhs) noexcept
{
	width -= rhs.width;
	height -= rhs.height;
	return *this;
}

Size2f& Size2f::operator*=(float scalar) noexcept
{
	width *= scalar;
	height *= scalar;
	return *this;
}

Size2f& Size2f::operator/=(float scalar) noexcept
{
	if (scalar != 0.0f)
	{
		width /= scalar;
		height /= scalar;
	}
	return *this;
}

Size2f Size2f::operator+(const Size2f& rhs) const noexcept
{
	return Size2f(width + rhs.width, height + rhs.height);
}

Size2f Size2f::operator-(const Size2f& rhs) const noexcept
{
	return Size2f(width - rhs.width, height - rhs.height);
}

Size2f Size2f::operator*(float scalar) const noexcept
{
	return Size2f(width * scalar, height * scalar);
}

Size2f Size2f::operator/(float scalar) const noexcept
{
	return (scalar != 0.0f)
		? Size2f(width / scalar, height / scalar)
		: Size2f();
}

bool Size2f::operator==(const Size2f& rhs) const noexcept
{
	return width == rhs.width && height == rhs.height;
}

bool Size2f::operator!=(const Size2f& rhs) const noexcept
{
	return !(*this == rhs);
}