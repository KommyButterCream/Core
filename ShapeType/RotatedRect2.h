#pragma once
#include "Rect2.h"
#include <array>
#include <cmath>

namespace Core::ShapeType
{
    template <ShapeScalar T> struct RotatedRect2
    {
        using value_type = T;
        using real_type = ShapeReal<T>;
        using VertexArray = std::array<Point2<T>, 4>;

        enum VertexIndex : int32_t
        {
            TopLeft,
            TopRight,
            BottomRight,
            BottomLeft
        };

        Point2<T> center{};
        Size2<T> size{};
        real_type angleRad{};

        constexpr RotatedRect2() noexcept = default;
        constexpr RotatedRect2(const Point2<T> &c, const Size2<T> &s, real_type a) noexcept
            : center(c), size(s), angleRad(a)
        {
        }
        constexpr RotatedRect2(T cx, T cy, T w, T h, real_type a) noexcept
            : center(cx, cy), size(w, h), angleRad(a)
        {
        }

        constexpr void Set(T cx, T cy, T w, T h, real_type a) noexcept
        {
            center.Set(cx, cy);
            size.Set(w, h);
            angleRad = a;
        }

        constexpr void Reset() noexcept
        {
            center.Reset();
            size.Reset();
            angleRad = real_type{};
        }

        constexpr T Width() const noexcept
        {
            return size.width;
        }

        constexpr T Height() const noexcept
        {
            return size.height;
        }

        constexpr const Point2<T> &GetCenter() const noexcept
        {
            return center;
        }

        constexpr const Size2<T> &GetSize() const noexcept
        {
            return size;
        }

        constexpr real_type GetAngle() const noexcept
        {
            return angleRad;
        }

        constexpr void Rotate(real_type delta) noexcept
        {
            angleRad += delta;
        }

        constexpr void SetAngle(real_type a) noexcept
        {
            angleRad = a;
        }

        constexpr void Translate(T dx, T dy) noexcept
        {
            center.Offset(dx, dy);
        }

        constexpr bool IsValid() const noexcept
        {
            return size.width > T{} && size.height > T{};
        }

        VertexArray GetVertices() const noexcept
        {
            const real_type hw = static_cast<real_type>(size.width) * real_type{0.5},
                            hh = static_cast<real_type>(size.height) * real_type{0.5};
            const real_type c = std::cos(angleRad), s = std::sin(angleRad);
            const Point2<real_type> local[4] = {{-hw, -hh}, {hw, -hh}, {hw, hh}, {-hw, hh}};
            VertexArray out{};

            for (size_t i = 0; i < 4; ++i)
            {
                const real_type x = static_cast<real_type>(center.x) + local[i].x * c - local[i].y * s;
                const real_type y = static_cast<real_type>(center.y) + local[i].x * s + local[i].y * c;

    			if constexpr (std::integral<T>)
                    out[i] = {static_cast<T>(std::round(x)), static_cast<T>(std::round(y))};
                else
                    out[i] = {x, y};
            }

            return out;
        }

        Rect2<T> GetBoundingRect() const noexcept
        {
            const auto v = GetVertices();
            Rect2<T> r{v[0].x, v[0].y, v[0].x, v[0].y};

    		for (size_t i = 1; i < 4; ++i)
            {
                r.left = (std::min)(r.left, v[i].x);
                r.top = (std::min)(r.top, v[i].y);
                r.right = (std::max)(r.right, v[i].x);
                r.bottom = (std::max)(r.bottom, v[i].y);
            }
            return r;
        }
    };
    using RotatedRect2i = RotatedRect2<int32_t>;
    using RotatedRect2u = RotatedRect2<uint32_t>;
    using RotatedRect2f = RotatedRect2<float>;
    using RotatedRect2d = RotatedRect2<double>;
} // namespace Core::ShapeType
