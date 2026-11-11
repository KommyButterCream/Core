#pragma once
#include "ShapeTypeBase.h"
#include <cmath>

namespace Core::ShapeType
{
    template <ShapeScalar T> struct Line2
    {
        using value_type = T;
        using real_type = ShapeReal<T>;

        T sx{};
        T sy{};
        T ex{};
        T ey{};

        constexpr Line2() noexcept = default;
        constexpr Line2(T sx, T sy, T ex, T ey) noexcept : sx(sx), sy(sy), ex(ex), ey(ey)
        {
        }

        constexpr void Set(T x1, T y1, T x2, T y2) noexcept
        {
            sx = x1;
            sy = y1;
            ex = x2;
            ey = y2;
        }

        constexpr void SetStart(T x, T y) noexcept
        {
            sx = x;
            sy = y;
        }

        constexpr void SetEnd(T x, T y) noexcept
        {
            ex = x;
            ey = y;
        }

        constexpr void Reset() noexcept
        {
            sx = sy = ex = ey = T{};
        }

        constexpr void Offset(T dx, T dy) noexcept
        {
            OffsetX(dx);
            OffsetY(dy);
        }

        constexpr void OffsetX(T dx) noexcept
        {
            sx += dx;
            ex += dx;
        }

        constexpr void OffsetY(T dy) noexcept
        {
            sy += dy;
            ey += dy;
        }

        constexpr real_type LengthSq() const noexcept
        {
            const auto dx = static_cast<real_type>(ex) - static_cast<real_type>(sx);
            const auto dy = static_cast<real_type>(ey) - static_cast<real_type>(sy);

            return dx * dx + dy * dy;
        }

        real_type Length() const noexcept
        {
            return std::sqrt(LengthSq());
        }

        real_type Angle() const noexcept
        {
            return static_cast<real_type>(std::atan2(static_cast<double>(ey) - sy, static_cast<double>(ex) - sx));
        }

        constexpr void Direction(T &dx, T &dy) const noexcept
        {
            dx = ex - sx;
            dy = ey - sy;
        }

        constexpr void Center(real_type &cx, real_type &cy) const noexcept
        {
            cx = (static_cast<real_type>(sx) + static_cast<real_type>(ex)) * real_type{0.5};
            cy = (static_cast<real_type>(sy) + static_cast<real_type>(ey)) * real_type{0.5};
        }
    };
    using Line2i = Line2<int32_t>;
    using Line2u = Line2<uint32_t>;
    using Line2f = Line2<float>;
    using Line2d = Line2<double>;
    using Line = Line2i;
} // namespace Core::ShapeType
