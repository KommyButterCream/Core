#pragma once
#include "ShapeTypeBase.h"
#include "Point2d.h"

namespace Core
{
	namespace ShapeType
	{
    struct Rect2d;

    struct QuadRect2d
    {
        using value_type = double;

        // < Variables >
        Point2d TopLeft{ 0.0, 0.0 };
        Point2d TopRight{ 0.0, 0.0 };
        Point2d BottomRight{ 0.0, 0.0 };
        Point2d BottomLeft{ 0.0, 0.0 };

        // < Constructor >
        constexpr QuadRect2d() noexcept = default;

        QuadRect2d(
            const Point2d& tl,
            const Point2d& tr,
            const Point2d& br,
            const Point2d& bl) noexcept;

        QuadRect2d(
            double tl_x, double tl_y,
            double tr_x, double tr_y,
            double br_x, double br_y,
            double bl_x, double bl_y) noexcept;

        QuadRect2d(const QuadRect2d& rhs) = default;
        QuadRect2d(QuadRect2d&& rhs) noexcept = default;
        QuadRect2d& operator=(const QuadRect2d& rhs) = default;
        QuadRect2d& operator=(QuadRect2d&& rhs) noexcept = default;

        // < Methods >
        void Set(
            const Point2d& tl,
            const Point2d& tr,
            const Point2d& br,
            const Point2d& bl) noexcept;

        void Reset() noexcept;

        Point2d Center() const noexcept;
        double Area() const noexcept;
        bool Empty() const noexcept;

        bool IsConvex() const noexcept;
        bool Contains(const Point2d& p) const noexcept;

        void Offset(double dx, double dy) noexcept;
        void ScaleFromCenter(double sx, double sy) noexcept;

        Rect2d ComputeBoundingRect() const noexcept;
        Rect2d ComputeInnerRect() const noexcept;

        // < Conversion >
        Rect2d ToRect2d() const noexcept;

        // < Operators >
        bool operator==(const QuadRect2d& rhs) const noexcept;
        bool operator!=(const QuadRect2d& rhs) const noexcept;
    };
	}
}