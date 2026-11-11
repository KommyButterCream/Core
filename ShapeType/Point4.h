#pragma once
#include "ShapeTypeBase.h"

namespace Core::ShapeType
{
    template <ShapeScalar T> struct Point4
    {
        using value_type = T;

        T x{};
        T y{};
        T z{};
        T w{};

        constexpr Point4() noexcept = default;
        constexpr Point4(T x, T y, T z, T w) noexcept : x(x), y(y), z(z), w(w)
        {
        }

        constexpr void Set(T new_x, T new_y, T new_z, T new_w) noexcept
        {
            x = new_x;
            y = new_y;
            z = new_z;
            w = new_w;
        }

        constexpr void Reset() noexcept
        {
            x = T{};
            y = T{};
            z = T{};
            w = T{};
        }

        constexpr void Offset(T dx, T dy, T dz, T dw) noexcept
        {
            x += dx;
            y += dy;
            z += dz;
            w += dw;
        }

        constexpr void OffsetX(T dx) noexcept
        {
            x += dx;
        }

        constexpr void OffsetY(T dy) noexcept
        {
            y += dy;
        }

        constexpr void OffsetZ(T dz) noexcept
        {
            z += dz;
        }

        constexpr void OffsetW(T dw) noexcept
        {
            w += dw;
        }

        constexpr Point4 operator+(const Point4 &rhs) const noexcept
        {
            return {x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w};
        }

        constexpr Point4 &operator+=(const Point4 &rhs) noexcept
        {
            x += rhs.x;
            y += rhs.y;
            z += rhs.z;
            w += rhs.w;

            return *this;
        }

        constexpr Point4 operator-(const Point4 &rhs) const noexcept
        {
            return {x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w};
        }

        constexpr Point4 &operator-=(const Point4 &rhs) noexcept
        {
            x -= rhs.x;
            y -= rhs.y;
            z -= rhs.z;
            w -= rhs.w;

            return *this;
        }

        constexpr Point4 operator*(T scalar) const noexcept
        {
            return {x * scalar, y * scalar, z * scalar, w * scalar};
        }

        constexpr Point4 &operator*=(T scalar) noexcept
        {
            x *= scalar;
            y *= scalar;
            z *= scalar;
            w *= scalar;

            return *this;
        }

        constexpr Point4 operator/(T scalar) const noexcept
        {
            return {x / scalar, y / scalar, z / scalar, w / scalar};
        }

        constexpr Point4 &operator/=(T scalar) noexcept
        {
            x /= scalar;
            y /= scalar;
            z /= scalar;
            w /= scalar;

            return *this;
        }

        constexpr bool operator==(const Point4 &) const noexcept = default;
    };
    using Point4i = Point4<int32_t>;
    using Point4u = Point4<uint32_t>;
    using Point4f = Point4<float>;
    using Point4d = Point4<double>;
} // namespace Core::ShapeType
