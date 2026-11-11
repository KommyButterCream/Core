#pragma once
#include "ShapeTypeBase.h"

namespace Core::ShapeType
{
    template <ShapeScalar T> struct Point2
    {
        using value_type = T;

        T x{};
        T y{};

        constexpr Point2() noexcept = default;
        constexpr Point2(T x, T y) noexcept : x(x), y(y)
        {
        }

        constexpr void Set(T new_x, T new_y) noexcept
        {
            x = new_x;
            y = new_y;
        }

        constexpr void Reset() noexcept
        {
            x = T{};
            y = T{};
        }

        constexpr void Offset(T dx, T dy) noexcept
        {
            x += dx;
            y += dy;
        }

        constexpr void OffsetX(T dx) noexcept
        {
            x += dx;
        }

        constexpr void OffsetY(T dy) noexcept
        {
            y += dy;
        }

        constexpr Point2 operator+(const Point2 &rhs) const noexcept
        {
            return {x + rhs.x, y + rhs.y};
        }

        constexpr Point2 &operator+=(const Point2 &rhs) noexcept
        {
            x += rhs.x;
            y += rhs.y;

            return *this;
        }

        constexpr Point2 operator-(const Point2 &rhs) const noexcept
        {
            return {x - rhs.x, y - rhs.y};
        }

        constexpr Point2 &operator-=(const Point2 &rhs) noexcept
        {
            x -= rhs.x;
            y -= rhs.y;

            return *this;
        }

        constexpr Point2 operator*(T scalar) const noexcept
        {
            return {x * scalar, y * scalar};
        }

        constexpr Point2 &operator*=(T scalar) noexcept
        {
            x *= scalar;
            y *= scalar;

            return *this;
        }

        constexpr Point2 operator/(T scalar) const noexcept
        {
            return {x / scalar, y / scalar};
        }

        constexpr Point2 &operator/=(T scalar) noexcept
        {
            x /= scalar;
            y /= scalar;

            return *this;
        }

        constexpr bool operator==(const Point2 &) const noexcept = default;
    };
    using Point2i = Point2<int32_t>;
    using Point2u = Point2<uint32_t>;
    using Point2f = Point2<float>;
    using Point2d = Point2<double>;
    using Point = Point2i;
} // namespace Core::ShapeType
