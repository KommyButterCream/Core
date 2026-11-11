#pragma once
#include "ShapeTypeBase.h"

namespace Core::ShapeType
{
    template <ShapeScalar T> struct Point3
    {
        using value_type = T;

        T x{};
        T y{};
        T z{};

        constexpr Point3() noexcept = default;
        constexpr Point3(T x, T y, T z) noexcept : x(x), y(y), z(z)
        {
        }

        constexpr void Set(T new_x, T new_y, T new_z) noexcept
        {
            x = new_x;
            y = new_y;
            z = new_z;
        }

        constexpr void Reset() noexcept
        {
            x = T{};
            y = T{};
            z = T{};
        }

        constexpr void Offset(T dx, T dy, T dz) noexcept
        {
            x += dx;
            y += dy;
            z += dz;
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

        constexpr Point3 operator+(const Point3 &rhs) const noexcept
        {
            return {x + rhs.x, y + rhs.y, z + rhs.z};
        }

        constexpr Point3 &operator+=(const Point3 &rhs) noexcept
        {
            x += rhs.x;
            y += rhs.y;
            z += rhs.z;

            return *this;
        }

        constexpr Point3 operator-(const Point3 &rhs) const noexcept
        {
            return {x - rhs.x, y - rhs.y, z - rhs.z};
        }

        constexpr Point3 &operator-=(const Point3 &rhs) noexcept
        {
            x -= rhs.x;
            y -= rhs.y;
            z -= rhs.z;

            return *this;
        }

        constexpr Point3 operator*(T scalar) const noexcept
        {
            return {x * scalar, y * scalar, z * scalar};
        }

        constexpr Point3 &operator*=(T scalar) noexcept
        {
            x *= scalar;
            y *= scalar;
            z *= scalar;

            return *this;
        }

        constexpr Point3 operator/(T scalar) const noexcept
        {
            return {x / scalar, y / scalar, z / scalar};
        }

        constexpr Point3 &operator/=(T scalar) noexcept
        {
            x /= scalar;
            y /= scalar;
            z /= scalar;

            return *this;
        }

        constexpr bool operator==(const Point3 &) const noexcept = default;
    };
    using Point3i = Point3<int32_t>;
    using Point3u = Point3<uint32_t>;
    using Point3f = Point3<float>;
    using Point3d = Point3<double>;
} // namespace Core::ShapeType
