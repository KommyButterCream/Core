#pragma once
#include "ShapeTypeBase.h"
#include "Point2f.h"
#include "Rect2f.h"

namespace Core
{
	namespace ShapeType
	{
	struct Polygon2f
	{
	public:
		using vertex_type = Point2f;

		// < Variables >
		vertex_type* vertices{ nullptr };
		size_t size{ 0 };
		size_t capacity{ 0 };

		// < Constructor & Destructor >
		constexpr Polygon2f() noexcept = default;
		Polygon2f(Point2f* points, size_t pointCount);

		Polygon2f(const Polygon2f& rhs);
		Polygon2f(Polygon2f&& rhs) noexcept;
		Polygon2f& operator=(const Polygon2f& rhs);
		Polygon2f& operator=(Polygon2f&& rhs) noexcept;

		~Polygon2f();

		// < Methods >
		bool Reserve(size_t newCapacity);
		bool AddVertex(const vertex_type& point);
		void Clear() noexcept;

		size_t Size() const noexcept;
		bool Empty() const noexcept;
		bool IsValid() const noexcept;

		const vertex_type* GetVertices() const noexcept;
		vertex_type* GetVertices() noexcept;

		float Area() const noexcept;
		Rect2f GetBoundingBox() const noexcept;
		Point2f GetCentroid() const noexcept;
		bool PointInPolygon(const Point2f& point) const noexcept;

	private:
		// < Internal Methods >
		void CopyFrom(const Polygon2f& rhs);
		void MoveFrom(Polygon2f& rhs);
	};
	}
}