#pragma once
#include "PolyShapeView.h"
#include <new>
#include <stdexcept>
#include <vector>

namespace Core::ShapeType
{
    // Module-local owner. Do not export this type or transfer its destruction to
    // another DLL; export a view or a pointer/count pair instead.
    template <ShapeScalar T, bool Closed> class PolyShape
    {
      public:
        using vertex_type = Point2<T>;
        using view_type = PolyShapeView<T, Closed>;
        using real_type = ShapeReal<T>;

        PolyShape() = default;
        PolyShape(const vertex_type *data, size_t count)
        {
            if (count && !data)
                throw std::invalid_argument("null shape vertices");
            if (count)
                vertices_.assign(data, data + count);
        }
        explicit PolyShape(view_type view) : PolyShape(view.Data(), view.Size())
        {
        }

        PolyShape(const PolyShape &) = default;
        PolyShape(PolyShape &&) noexcept = default;

        PolyShape &operator=(const PolyShape &) = default;
        PolyShape &operator=(PolyShape &&) noexcept = default;

        ~PolyShape() = default;

        bool Reserve(size_t capacity)
        {
            try
            {
                vertices_.reserve(capacity);
                return true;
            }
            catch (const std::bad_alloc &)
            {
                return false;
            }
            catch (const std::length_error &)
            {
                return false;
            }
        }

        bool AddVertex(const vertex_type &point)
        {
            // Copy first: point may refer to an element invalidated by reallocation.
            const vertex_type copy = point;
            try
            {
                vertices_.push_back(copy);
                return true;
            }
            catch (const std::bad_alloc &)
            {
                return false;
            }
            catch (const std::length_error &)
            {
                return false;
            }
        }

        void Clear() noexcept
        {
            std::vector<vertex_type>{}.swap(vertices_);
        }

        size_t Size() const noexcept
        {
            return vertices_.size();
        }

        size_t Capacity() const noexcept
        {
            return vertices_.capacity();
        }

        bool Empty() const noexcept
        {
            return vertices_.empty();
        }

        bool IsValid() const noexcept
        {
            return View().IsValid();
        }

        vertex_type *Data() noexcept
        {
            return vertices_.data();
        }

        const vertex_type *Data() const noexcept
        {
            return vertices_.data();
        }

        vertex_type *GetVertices() noexcept
        {
            return Data();
        }

        const vertex_type *GetVertices() const noexcept
        {
            return Data();
        }

        view_type View() const & noexcept
        {
            return {Data(), Size()};
        }

        view_type View() const && = delete;

        Rect2<T> GetBoundingBox() const noexcept
        {
            return View().GetBoundingBox();
        }

        Point2<real_type> GetCentroid() const noexcept
        {
            return View().GetCentroid();
        }

        real_type Area() const noexcept
            requires Closed
        {
            return View().Area();
        }

        bool PointInPolygon(const vertex_type &p) const noexcept
            requires Closed
        {
            return View().PointInPolygon(p);
        }

      private:
        std::vector<vertex_type> vertices_;
    };

    template <ShapeScalar T> using Polygon = PolyShape<T, true>;
    template <ShapeScalar T> using Polyline = PolyShape<T, false>;
    using Polygon2i = Polygon<int32_t>;
    using Polygon2u = Polygon<uint32_t>;
    using Polygon2f = Polygon<float>;
    using Polygon2d = Polygon<double>;
    using Polyline2i = Polyline<int32_t>;
    using Polyline2u = Polyline<uint32_t>;
    using Polyline2f = Polyline<float>;
    using Polyline2d = Polyline<double>;
} // namespace Core::ShapeType
