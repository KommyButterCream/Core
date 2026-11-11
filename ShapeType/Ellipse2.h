#pragma once
#include "Rect2.h"
#include "ShapeTypeBase.h"
#include <algorithm>
#include <cmath>
#include <numbers>

namespace Core::ShapeType
{
    template <ShapeScalar T> struct Ellipse2
    {
      public:
        using value_type = T;
        using real_type = ShapeReal<T>;

      public:
        // < Variables >
        T x{T{0}};        // < center x >
        T y{T{0}};        // < center y >
        T radiusX{T{0}};  // < x radius >
        T radiusY{T{0}};  // < y radius >
        real_type angleRad{}; // < rotation angle (radian, CCW) >

        // < Constructor >
        constexpr Ellipse2() noexcept = default;
        Ellipse2(T cx, T cy, T rx, T ry, real_type angleRad = real_type{0}) noexcept;
        Ellipse2(const Point2<T> &center, T rx, T ry, real_type angleRad = real_type{0}) noexcept;

        Ellipse2(const Ellipse2 &) = default;
        Ellipse2(Ellipse2 &&) noexcept = default;

        Ellipse2 &operator=(const Ellipse2 &) = default;
        Ellipse2 &operator=(Ellipse2 &&) noexcept = default;

        ~Ellipse2() = default;

      public:
        // < Set / Reset >
        void Set(T cx, T cy, T rx, T ry, real_type angleRad) noexcept;

        void Set(const Point2<T> &center, T rx, T ry, real_type angleRad) noexcept;

        void Reset() noexcept;

        // < Geometry >
        Point2<T> Center() const noexcept;

        real_type Area() const noexcept;

        bool Contains(T px, T py) const noexcept;

        bool Contains(const Point2<T> &p) const noexcept;

        // < Helpers >
        Rect2<real_type> BoundingBox() const noexcept;
    };

    template <ShapeScalar T>
    Ellipse2<T>::Ellipse2(T cx, T cy, T rx, T ry, real_type angle) noexcept
        : x(cx), y(cy), radiusX(rx), radiusY(ry), angleRad(angle)
    {
    }

    template <ShapeScalar T>
    Ellipse2<T>::Ellipse2(const Point2<T> &center, T rx, T ry, real_type angle) noexcept
        : x(center.x), y(center.y), radiusX(rx), radiusY(ry), angleRad(angle)
    {
    }

    template <ShapeScalar T> void Ellipse2<T>::Set(T cx, T cy, T rx, T ry, real_type angle) noexcept
    {
        x = cx;
        y = cy;
        radiusX = rx;
        radiusY = ry;
        angleRad = angle;
    }

    template <ShapeScalar T> void Ellipse2<T>::Set(const Point2<T> &center, T rx, T ry, real_type angle) noexcept
    {
        x = center.x;
        y = center.y;
        radiusX = rx;
        radiusY = ry;
        angleRad = angle;
    }

    template <ShapeScalar T> void Ellipse2<T>::Reset() noexcept
    {
        x = T{0};
        y = T{0};
        radiusX = T{0};
        radiusY = T{0};
        angleRad = real_type{0};
    }

    template <ShapeScalar T> Point2<T> Ellipse2<T>::Center() const noexcept
    {
        return Point2<T>{x, y};
    }

    template <ShapeScalar T> auto Ellipse2<T>::Area() const noexcept -> real_type
    {
        return std::numbers::pi_v<real_type> * radiusX * radiusY;
    }

    template <ShapeScalar T> bool Ellipse2<T>::Contains(T px, T py) const noexcept
    {
        if (radiusX <= T{0} || radiusY <= T{0})
            return false;

        const real_type dx = static_cast<real_type>(px) - static_cast<real_type>(x);
        const real_type dy = static_cast<real_type>(py) - static_cast<real_type>(y);
        const real_type c = std::cos(angleRad);
        const real_type s = std::sin(angleRad);

        // inverse rotation
        const real_type lx = dx * c + dy * s;
        const real_type ly = -dx * s + dy * c;
        const real_type nx = lx / radiusX;
        const real_type ny = ly / radiusY;

        return (nx * nx + ny * ny) <= T{1};
    }

    template <ShapeScalar T> bool Ellipse2<T>::Contains(const Point2<T> &p) const noexcept
    {
        return Contains(p.x, p.y);
    }

    template <ShapeScalar T> auto Ellipse2<T>::BoundingBox() const noexcept -> Rect2<real_type>
    {
        if (radiusX <= T{0} || radiusY <= T{0})
            return Rect2<real_type>(static_cast<real_type>(x), static_cast<real_type>(y),
                                    static_cast<real_type>(x), static_cast<real_type>(y));

        const real_type c = std::cos(angleRad);
        const real_type s = std::sin(angleRad);

        // ellipse extreme extents after rotation
        const real_type ex = std::hypot(radiusX * c, radiusY * s);
        const real_type ey = std::hypot(radiusX * s, radiusY * c);

        return Rect2<real_type>(x - ex, y - ey, x + ex, y + ey);
    }

    using Ellipse2i = Ellipse2<int32_t>;
    using Ellipse2u = Ellipse2<uint32_t>;
    using Ellipse2f = Ellipse2<float>;
    using Ellipse2d = Ellipse2<double>;
} // namespace Core::ShapeType
