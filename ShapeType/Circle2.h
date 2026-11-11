#pragma once
#include "Rect2.h"
#include "ShapeTypeBase.h"
#include <algorithm>
#include <cmath>
#include <numbers>

namespace Core::ShapeType
{
    template <ShapeScalar T> struct Circle2
    {
      public:
        using value_type = T;
        using real_type = ShapeReal<T>;

      public:
        T x{T{0}};      // center x
        T y{T{0}};      // center y
        T radius{T{0}}; // radius

        // --- ctor ---
        constexpr Circle2() noexcept = default;
        Circle2(T cx, T cy, T r) noexcept;
        Circle2(const Point2<T> &center, T r) noexcept;

        Circle2(const Circle2 &) = default;
        Circle2(Circle2 &&) noexcept = default;

        Circle2 &operator=(const Circle2 &) = default;
        Circle2 &operator=(Circle2 &&) noexcept = default;

        ~Circle2() = default;

      public:
        // < Methods >
        void Set(T cx, T cy, T r) noexcept;

        void Set(const Point2<T> &center, T r) noexcept;

        void Reset() noexcept;

        // < Geometry >
        Point2<T> Center() const noexcept;

        real_type Area() const noexcept;

        bool Contains(T px, T py) const noexcept;

        bool Contains(const Point2<T> &p) const noexcept;

        // < Helpers >
        Rect2<real_type> BoundingBox() const noexcept;
    };

    template <ShapeScalar T> Circle2<T>::Circle2(T cx, T cy, T r) noexcept : x(cx), y(cy), radius(r)
    {
    }

    template <ShapeScalar T>
    Circle2<T>::Circle2(const Point2<T> &center, T r) noexcept : x(center.x), y(center.y), radius(r)
    {
    }

    template <ShapeScalar T> void Circle2<T>::Set(T cx, T cy, T r) noexcept
    {
        x = cx;
        y = cy;
        radius = r;
    }

    template <ShapeScalar T> void Circle2<T>::Set(const Point2<T> &center, T r) noexcept
    {
        x = center.x;
        y = center.y;
        radius = r;
    }

    template <ShapeScalar T> void Circle2<T>::Reset() noexcept
    {
        x = T{0};
        y = T{0};
        radius = T{0};
    }

    template <ShapeScalar T> Point2<T> Circle2<T>::Center() const noexcept
    {
        return Point2<T>{x, y};
    }

    template <ShapeScalar T> auto Circle2<T>::Area() const noexcept -> real_type
    {
        return std::numbers::pi_v<real_type> * radius * radius;
    }

    template <ShapeScalar T> bool Circle2<T>::Contains(T px, T py) const noexcept
    {
        const real_type dx = static_cast<real_type>(px) - static_cast<real_type>(x);
        const real_type dy = static_cast<real_type>(py) - static_cast<real_type>(y);

        return (dx * dx + dy * dy) <= (static_cast<real_type>(radius) * radius);
    }

    template <ShapeScalar T> bool Circle2<T>::Contains(const Point2<T> &p) const noexcept
    {
        return Contains(p.x, p.y);
    }

    template <ShapeScalar T> auto Circle2<T>::BoundingBox() const noexcept -> Rect2<real_type>
    {
        const real_type cx = static_cast<real_type>(x), cy = static_cast<real_type>(y);
        const real_type r = static_cast<real_type>(radius);

        return Rect2<real_type>(cx - r, cy - r, cx + r, cy + r);
    }

    using Circle2i = Circle2<int32_t>;
    using Circle2u = Circle2<uint32_t>;
    using Circle2f = Circle2<float>;
    using Circle2d = Circle2<double>;
} // namespace Core::ShapeType
