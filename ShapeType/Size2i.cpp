#include "Size2i.h"
#include <algorithm>
#include <cmath>

using Core::ShapeType::Size2i;

constexpr Size2i::Size2i(int32_t width, int32_t height) noexcept
	: width(width), height(height)
{
}

void Size2i::Set(int32_t width, int32_t height) noexcept
{
	width = width;
	height = height;
}

void Size2i::Reset() noexcept
{
	width = 0;
	height = 0;
}

int32_t Size2i::Area() const noexcept
{
	return width * height;
}

int32_t Size2i::AspectRatio() const noexcept
{
	return (height != 0) ? (width / height) : 0;
}

bool Size2i::IsEmpty() const noexcept
{
	return width <= 0 || height <= 0;
}

bool Size2i::IsValid() const noexcept
{
	return width >= 0 && height >= 0;
}

void Size2i::ClampMin(int32_t minValue) noexcept
{
	width = std::max(width, minValue);
	height = std::max(height, minValue);
}

void Size2i::ClampMax(int32_t maxValue) noexcept
{
	width = std::min(width, maxValue);
	height = std::min(height, maxValue);
}

bool Size2i::EqualsEpsilon(const Size2i& rhs) const noexcept
{
	return std::abs(width - rhs.width) &&
		std::abs(height - rhs.height);
}

constexpr Size2i Size2i::Zero() noexcept
{
	return Size2i(0, 0);
}

Size2i& Size2i::operator+=(const Size2i& rhs) noexcept
{
	width += rhs.width;
	height += rhs.height;
	return *this;
}

Size2i& Size2i::operator-=(const Size2i& rhs) noexcept
{
	width -= rhs.width;
	height -= rhs.height;
	return *this;
}

Size2i& Size2i::operator*=(int32_t scalar) noexcept
{
	width *= scalar;
	height *= scalar;
	return *this;
}

Size2i& Size2i::operator/=(int32_t scalar) noexcept
{
	if (scalar != 0)
	{
		width /= scalar;
		height /= scalar;
	}
	return *this;
}

Size2i Size2i::operator+(const Size2i& rhs) const noexcept
{
	return Size2i(width + rhs.width, height + rhs.height);
}

Size2i Size2i::operator-(const Size2i& rhs) const noexcept
{
	return Size2i(width - rhs.width, height - rhs.height);
}

Size2i Size2i::operator*(int32_t scalar) const noexcept
{
	return Size2i(width * scalar, height * scalar);
}

Size2i Size2i::operator/(int32_t scalar) const noexcept
{
	return (scalar != 0)
		? Size2i(width / scalar, height / scalar)
		: Size2i();
}

bool Size2i::operator==(const Size2i& rhs) const noexcept
{
	return width == rhs.width && height == rhs.height;
}

bool Size2i::operator!=(const Size2i& rhs) const noexcept
{
	return !(*this == rhs);
}