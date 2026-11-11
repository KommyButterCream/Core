#pragma once
#include "Rect2.h"
#include <cmath>

namespace Core::ShapeType
{
    // Borrowed, read-only vertices. The caller retains ownership. Never store this
    // view beyond the lifetime of its source, or across source reallocation.
    template <ShapeScalar T, bool Closed> struct PolyShapeView
    {
        using vertex_type = Point2<T>;
        using real_type = ShapeReal<T>;

        const vertex_type *vertices{};
        size_t count{};

        constexpr PolyShapeView() noexcept = default;
        constexpr PolyShapeView(const vertex_type *data, size_t size) noexcept
            : vertices(data), count(data ? size : 0)
        {
        }

        constexpr const vertex_type *Data() const noexcept
        {
            return vertices;
        }

        constexpr const vertex_type *GetVertices() const noexcept
        {
            return vertices;
        }

        constexpr size_t Size() const noexcept
        {
            return vertices ? count : 0;
        }

        constexpr bool Empty() const noexcept
        {
            return Size() == 0;
        }

        constexpr bool IsValid() const noexcept
        {
            return Size() >= (Closed ? 3u : 2u);
        }

        constexpr Rect2<T> GetBoundingBox() const noexcept
        {
            if (Empty())
                return {};

            Rect2<T> r{vertices[0].x, vertices[0].y, vertices[0].x, vertices[0].y};
            for (size_t i = 1; i < Size(); ++i)
            {
                r.left = (std::min)(r.left, vertices[i].x);
                r.top = (std::min)(r.top, vertices[i].y);
                r.right = (std::max)(r.right, vertices[i].x);
                r.bottom = (std::max)(r.bottom, vertices[i].y);
            }

            return r;
        }

        real_type Area() const noexcept
            requires Closed
        {
            if (!IsValid())
                return {};

            double sum = 0;
            for (size_t i = 0, j = Size() - 1; i < Size(); j = i++)
                sum += static_cast<double>(vertices[j].x) * vertices[i].y -
                       static_cast<double>(vertices[i].x) * vertices[j].y;

            return static_cast<real_type>(std::abs(sum) * 0.5);
        }

        Point2<real_type> GetCentroid() const noexcept
        {
            if (Empty())
                return {};

            double x = 0, y = 0;
            if constexpr (Closed)
            {
                if (!IsValid())
                    return {};

                double area2 = 0;
                for (size_t i = 0, j = Size() - 1; i < Size(); j = i++)
                {
                    const auto &a = vertices[j];
                    const auto &b = vertices[i];
                    const double cross = static_cast<double>(a.x) * b.y - static_cast<double>(b.x) * a.y;
                    x += (static_cast<double>(a.x) + b.x) * cross;
                    y += (static_cast<double>(a.y) + b.y) * cross;
                    area2 += cross;
                }

                if (std::abs(area2) < 2e-7)
                    return {};

                x /= 3 * area2;
                y /= 3 * area2;
            }
            else
            {
                for (size_t i = 0; i < Size(); ++i)
                {
                    x += vertices[i].x;
                    y += vertices[i].y;
                }

                x /= static_cast<double>(Size());
                y /= static_cast<double>(Size());
            }

            return {static_cast<real_type>(x), static_cast<real_type>(y)};
        }

        bool PointInPolygon(const vertex_type &point) const noexcept
            requires Closed
        {
            if (!IsValid())
                return false;

            bool inside = false;
            for (size_t i = 0, j = Size() - 1; i < Size(); j = i++)
            {
                const auto &a = vertices[i];
                const auto &b = vertices[j];
                if ((a.y > point.y) != (b.y > point.y) && point.x < (static_cast<double>(b.x) - a.x) *
                                                                    (static_cast<double>(point.y) - a.y) /
                                                                    (static_cast<double>(b.y) - a.y) + a.x)
                    inside = !inside;
            }

            return inside;
        }
    };

    template <ShapeScalar T> using PolygonView = PolyShapeView<T, true>;
    template <ShapeScalar T> using PolylineView = PolyShapeView<T, false>;
    using PolygonView2i = PolygonView<int32_t>;
    using PolygonView2u = PolygonView<uint32_t>;
    using PolygonView2f = PolygonView<float>;
    using PolygonView2d = PolygonView<double>;
    using PolylineView2i = PolylineView<int32_t>;
    using PolylineView2u = PolylineView<uint32_t>;
    using PolylineView2f = PolylineView<float>;
    using PolylineView2d = PolylineView<double>;
    static_assert(std::is_standard_layout_v<PolygonView2f> && std::is_trivially_copyable_v<PolygonView2f>);
    static_assert(std::is_standard_layout_v<PolylineView2f> && std::is_trivially_copyable_v<PolylineView2f>);
} // namespace Core::ShapeType
