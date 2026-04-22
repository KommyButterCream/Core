#pragma once
#include "ShapeTypeBase.h"
#include "Point2f.h"
#include "Rect2f.h"

namespace Core
{
	namespace ShapeType
	{
	struct Polyline2f
	{
	public:
		using vertex_type = Point2f;

		// < Variables >
		vertex_type* vertices{ nullptr };
		size_t size{ 0 };
		size_t capacity{ 0 };

		// < Constructor & Destructor >
		constexpr Polyline2f() noexcept = default;
		Polyline2f(Point2f* points, size_t pointCount);

		Polyline2f(const Polyline2f& rhs);
		Polyline2f(Polyline2f&& rhs) noexcept;
		Polyline2f& operator=(const Polyline2f& rhs);
		Polyline2f& operator=(Polyline2f&& rhs) noexcept;

		~Polyline2f();

		// < Methods >
		bool Reserve(size_t newCapacity);
		bool AddVertex(const vertex_type& point);
		void Clear() noexcept;

		size_t Size() const noexcept;
		bool Empty() const noexcept;

		const vertex_type* GetVertices() const noexcept;
		vertex_type* GetVertices() noexcept;

		Rect2f GetBoundingBox() const noexcept;
		Point2f GetCentroid() const noexcept;

	private:
		// < Internal Methods >
		void CopyFrom(const Polyline2f& rhs);
		void MoveFrom(Polyline2f& rhs);
	};
	}
}