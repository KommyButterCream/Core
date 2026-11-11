#pragma once
#include "Point2.h"
#include "Size2.h"
#include <algorithm>

namespace Core::ShapeType
{
    template <ShapeScalar T> struct Rect2
    {
        using value_type = T;
        using real_type = ShapeReal<T>;

        T left{};
        T top{};
        T right{};
        T bottom{};

        constexpr Rect2() noexcept = default;
        constexpr Rect2(T l, T t, T r, T b) noexcept : left(l), top(t), right(r), bottom(b)
        {
        }

        constexpr void Set(T l, T t, T r, T b) noexcept
        {
            left = l;
            top = t;
            right = r;
            bottom = b;
        }

        constexpr void Reset() noexcept
        {
            left = top = right = bottom = T{};
        }

        constexpr T Width() const noexcept
        {
            return right - left;
        }

        constexpr T Height() const noexcept
        {
            return bottom - top;
        }

        constexpr Size2<real_type> Size() const noexcept
        {
            return {static_cast<real_type>(right) - left, static_cast<real_type>(bottom) - top};
        }

        constexpr real_type Area() const noexcept
        {
            return Empty() ? real_type{} : (static_cast<real_type>(right) - left) *
                                              (static_cast<real_type>(bottom) - top);
        }

        constexpr bool Empty() const noexcept
        {
            return left >= right || top >= bottom;
        }

        constexpr Point2<real_type> Center() const noexcept
        {
            return {(static_cast<real_type>(left) + static_cast<real_type>(right)) * real_type{0.5},
                    (static_cast<real_type>(top) + static_cast<real_type>(bottom)) * real_type{0.5}};
        }

        constexpr void Normalize() noexcept
        {
            if (left > right)
                std::swap(left, right);
            if (top > bottom)
                std::swap(top, bottom);
        }

        template <ShapeScalar U> constexpr bool Contains(U x, U y) const noexcept
        {
            return static_cast<double>(x) >= static_cast<double>(left) &&
                   static_cast<double>(x) <= static_cast<double>(right) &&
                   static_cast<double>(y) >= static_cast<double>(top) &&
                   static_cast<double>(y) <= static_cast<double>(bottom);
        }

        template <ShapeScalar U> constexpr bool Contains(const Point2<U> &p) const noexcept
        {
            return Contains(p.x, p.y);
        }

        constexpr bool Contains(const Point2<real_type> &p) const noexcept
        {
            return Contains(p.x, p.y);
        }

        constexpr bool Contains(const Rect2 &r) const noexcept
        {
            return r.left >= left && r.right <= right && r.top >= top && r.bottom <= bottom;
        }

        constexpr bool Intersects(const Rect2 &r) const noexcept
        {
            return !(r.right <= left || r.left >= right || r.bottom <= top || r.top >= bottom);
        }

        constexpr Rect2 Intersect(const Rect2 &r) const noexcept
        {
            Rect2 out{(std::max)(left, r.left), (std::max)(top, r.top), (std::min)(right, r.right),
                      (std::min)(bottom, r.bottom)};

            return out.Empty() ? Rect2{} : out;
        }

        constexpr Rect2 Unite(const Rect2 &r) const noexcept
        {
            if (Empty())
                return r;

            if (r.Empty())
                return *this;

            return {(std::min)(left, r.left), (std::min)(top, r.top), (std::max)(right, r.right),
                    (std::max)(bottom, r.bottom)};
        }

        constexpr Rect2 &IntersectWith(const Rect2 &r) noexcept
        {
            return *this = Intersect(r);
        }

        constexpr Rect2 &UniteWith(const Rect2 &r) noexcept
        {
            return *this = Unite(r);
        }

        constexpr void Offset(T dx, T dy) noexcept
        {
            left += dx;
            right += dx;
            top += dy;
            bottom += dy;
        }

        constexpr void Inflate(T dx, T dy) noexcept
        {
            left -= dx;
            right += dx;
            top -= dy;
            bottom += dy;
        }

        constexpr void Deflate(T dx, T dy) noexcept
        {
            left += dx;
            right -= dx;
            top += dy;
            bottom -= dy;
        }

        constexpr void ScaleFromCenter(T sx, T sy) noexcept
        {
            const auto c = Center();
            // Integer rectangles retain the original truncation of half extents.
            const T hw = static_cast<T>(Width() * real_type{0.5} * sx);
            const T hh = static_cast<T>(Height() * real_type{0.5} * sy);
            Set(static_cast<T>(c.x - hw), static_cast<T>(c.y - hh), static_cast<T>(c.x + hw),
                static_cast<T>(c.y + hh));
        }

        constexpr void MoveCenterTo(T cx, T cy) noexcept
        {
            const auto c = Center();
            const real_type dx = static_cast<real_type>(cx) - c.x;
            const real_type dy = static_cast<real_type>(cy) - c.y;
            Set(static_cast<T>(left + dx), static_cast<T>(top + dy),
                static_cast<T>(right + dx), static_cast<T>(bottom + dy));
        }

        constexpr Rect2 operator&(const Rect2 &r) const noexcept
        {
            return Intersect(r);
        }

        constexpr Rect2 operator|(const Rect2 &r) const noexcept
        {
            return Unite(r);
        }

        constexpr Rect2 &operator&=(const Rect2 &r) noexcept
        {
            return IntersectWith(r);
        }

        constexpr Rect2 &operator|=(const Rect2 &r) noexcept
        {
            return UniteWith(r);
        }

        constexpr bool operator==(const Rect2 &) const noexcept = default;
    };
    using Rect2i = Rect2<int32_t>;
    using Rect2u = Rect2<uint32_t>;
    using Rect2f = Rect2<float>;
    using Rect2d = Rect2<double>;
    using Rect = Rect2i;
} // namespace Core::ShapeType
