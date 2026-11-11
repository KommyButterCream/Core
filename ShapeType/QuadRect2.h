#pragma once
#include "Rect2.h"
#include <cmath>
#include <limits>

namespace Core::ShapeType
{
    template <ShapeScalar T> struct QuadRect2
    {
        using value_type = T;
        using real_type = ShapeReal<T>;

        Point2<T> TopLeft{};
        Point2<T> TopRight{};
        Point2<T> BottomRight{};
        Point2<T> BottomLeft{};

        constexpr QuadRect2() noexcept = default;
        constexpr QuadRect2(const Point2<T> &tl, const Point2<T> &tr, const Point2<T> &br,
                            const Point2<T> &bl) noexcept
            : TopLeft(tl), TopRight(tr), BottomRight(br), BottomLeft(bl)
        {
        }
        constexpr QuadRect2(T tx, T ty, T rx, T ry, T bx, T by, T lx, T ly) noexcept
            : QuadRect2({tx, ty}, {rx, ry}, {bx, by}, {lx, ly})
        {
        }

        constexpr void Set(const Point2<T> &tl, const Point2<T> &tr, const Point2<T> &br,
                           const Point2<T> &bl) noexcept
        {
            TopLeft = tl;
            TopRight = tr;
            BottomRight = br;
            BottomLeft = bl;
        }

        constexpr void Reset() noexcept
        {
            TopLeft = TopRight = BottomRight = BottomLeft = {};
        }

        constexpr Point2<real_type> Center() const noexcept
        {
            return {(static_cast<real_type>(TopLeft.x) + TopRight.x + BottomRight.x + BottomLeft.x) *
                        real_type{0.25},
                    (static_cast<real_type>(TopLeft.y) + TopRight.y + BottomRight.y + BottomLeft.y) *
                        real_type{0.25}};
        }

        real_type Area() const noexcept
        {
            const auto cross = [](const Point2<T> &a, const Point2<T> &b) {
                return static_cast<double>(a.x) * b.y - static_cast<double>(a.y) * b.x;
            };

            return static_cast<real_type>(std::abs(cross(TopLeft, TopRight) + cross(TopRight, BottomRight) +
                                                   cross(BottomRight, BottomLeft) + cross(BottomLeft, TopLeft)) * 0.5);
        }

        bool Empty() const noexcept
        {
            return Area() <= std::numeric_limits<T>::epsilon();
        }

        bool IsConvex() const noexcept
        {
            const auto cross = [](const Point2<T> &a, const Point2<T> &b, const Point2<T> &c) {
                return (static_cast<double>(b.x) - a.x) * (static_cast<double>(c.y) - a.y) -
                       (static_cast<double>(b.y) - a.y) * (static_cast<double>(c.x) - a.x);
            };

            const auto z1 = cross(TopLeft, TopRight, BottomRight), z2 = cross(TopRight, BottomRight, BottomLeft),
                       z3 = cross(BottomRight, BottomLeft, TopLeft), z4 = cross(BottomLeft, TopLeft, TopRight);

            return (z1 > 0 && z2 > 0 && z3 > 0 && z4 > 0) || (z1 < 0 && z2 < 0 && z3 < 0 && z4 < 0);
        }

        bool Contains(const Point2<T> &p) const noexcept
        {
            const auto same = [](const Point2<T> &a, const Point2<T> &b, const Point2<T> &p1,
                                 const Point2<T> &p2) {
                const auto dx = static_cast<double>(b.x) - a.x, dy = static_cast<double>(b.y) - a.y;
                const auto c1 = dx * (static_cast<double>(p1.y) - a.y) - dy * (static_cast<double>(p1.x) - a.x),
                           c2 = dx * (static_cast<double>(p2.y) - a.y) - dy * (static_cast<double>(p2.x) - a.x);
                return (c1 >= 0 && c2 >= 0) || (c1 <= 0 && c2 <= 0);
            };

            return same(TopLeft, TopRight, p, BottomLeft) && same(TopRight, BottomRight, p, TopLeft) &&
                   same(BottomRight, BottomLeft, p, TopRight) && same(BottomLeft, TopLeft, p, BottomRight);
        }

        constexpr void Offset(T dx, T dy) noexcept
        {
            TopLeft.Offset(dx, dy);
            TopRight.Offset(dx, dy);
            BottomRight.Offset(dx, dy);
            BottomLeft.Offset(dx, dy);
        }

        void ScaleFromCenter(T sx, T sy) noexcept
        {
            const auto c = Center();
            const auto scale = [&](Point2<T> &p) {
                const auto x = c.x + (p.x - c.x) * sx, y = c.y + (p.y - c.y) * sy;
                if constexpr (std::integral<T>)
                    p = {static_cast<T>(std::round(x)), static_cast<T>(std::round(y))};
                else
                    p = {x, y};
            };

            scale(TopLeft);
            scale(TopRight);
            scale(BottomRight);
            scale(BottomLeft);
        }

        Rect2<T> ComputeBoundingRect() const noexcept
        {
            return {(std::min)({TopLeft.x, TopRight.x, BottomRight.x, BottomLeft.x}),
                    (std::min)({TopLeft.y, TopRight.y, BottomRight.y, BottomLeft.y}),
                    (std::max)({TopLeft.x, TopRight.x, BottomRight.x, BottomLeft.x}),
                    (std::max)({TopLeft.y, TopRight.y, BottomRight.y, BottomLeft.y})};
        }

        Rect2<T> ComputeInnerRect() const noexcept
        {
            Rect2<T> r{(std::max)(TopLeft.x, BottomLeft.x), (std::max)(TopLeft.y, TopRight.y),
                       (std::min)(TopRight.x, BottomRight.x), (std::min)(BottomLeft.y, BottomRight.y)};

            return r.left > r.right || r.top > r.bottom ? Rect2<T>{} : r;
        }

        constexpr bool operator==(const QuadRect2 &) const noexcept = default;
    };
    using QuadRect2i = QuadRect2<int32_t>;
    using QuadRect2u = QuadRect2<uint32_t>;
    using QuadRect2f = QuadRect2<float>;
    using QuadRect2d = QuadRect2<double>;
} // namespace Core::ShapeType
