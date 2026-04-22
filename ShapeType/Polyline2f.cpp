#include "Polyline2f.h"
#include <float.h>

using Core::ShapeType::Point2f;
using Core::ShapeType::Polyline2f;
using Core::ShapeType::Rect2f;

Polyline2f::Polyline2f(Point2f* points, size_t pointCount)
{
	if (!points || pointCount == 0)
		return;

	Reserve(pointCount);

	for (size_t i = 0; i < pointCount; ++i)
		vertices[i] = points[i];

	size = pointCount;
}

Polyline2f::Polyline2f(const Polyline2f& rhs)
{
	CopyFrom(rhs);
}

Polyline2f::Polyline2f(Polyline2f&& rhs) noexcept
{
	MoveFrom(rhs);
}

Polyline2f& Polyline2f::operator=(const Polyline2f& rhs)
{
	if (this != &rhs)
	{
		Clear();
		CopyFrom(rhs);
	}
	return *this;
}

Polyline2f& Polyline2f::operator=(Polyline2f&& rhs) noexcept
{
	if (this != &rhs)
	{
		Clear();
		MoveFrom(rhs);
	}
	return *this;
}

Polyline2f::~Polyline2f()
{
	Clear();
}

bool Polyline2f::Reserve(size_t newCapacity)
{
	if (newCapacity <= capacity)
		return true;

	vertex_type* newBuffer = new vertex_type[newCapacity];
	if (!newBuffer)
		return false;

	for (size_t i = 0; i < size; ++i)
		newBuffer[i] = vertices[i];

	delete[] vertices;

	vertices = newBuffer;
	capacity = newCapacity;

	return true;
}

bool Polyline2f::AddVertex(const vertex_type& point)
{
	if (size >= capacity)
	{
		size_t newCap = (capacity == 0) ? 4 : capacity * 2;

		if (!Reserve(newCap))
			return false;
	}

	vertices[size++] = point;

	return true;
}

void Polyline2f::Clear() noexcept
{
	if (vertices)
	{
		delete[] vertices;
		vertices = nullptr;
	}

	size = 0;
	capacity = 0;
}

size_t Polyline2f::Size() const noexcept
{
	return size;
}

bool Polyline2f::Empty() const noexcept
{
	return size == 0;
}

const Polyline2f::vertex_type* Polyline2f::GetVertices() const noexcept
{
	return vertices;
}

Polyline2f::vertex_type* Polyline2f::GetVertices() noexcept
{
	return vertices;
}

Rect2f Polyline2f::GetBoundingBox() const noexcept
{
	if (Empty())
	{
		return Rect2f();
	}

	// std::min(a,b), std::max(a,b) 와 컴파일러가 구분을 못해서
	// 아래와 같이 괄호를 추가하여 처리함.
	float minX = FLT_MAX;
	float minY = FLT_MAX;
	float maxX = FLT_MIN;
	float maxY = FLT_MIN;

	for (size_t i = 0; i < size; i++)
	{
		const Point2f& point = vertices[i];

		if (point.x < minX) minX = point.x;
		if (point.y < minY) minY = point.y;
		if (point.x > maxX) maxX = point.x;
		if (point.y > maxY) maxY = point.y;
	}

	return Rect2f(minX, minY, maxX, maxY);
}

Point2f Polyline2f::GetCentroid() const noexcept
{
	if (Empty())
		return Point2f();

	float sx = 0.0f;
	float sy = 0.0f;

	for (uint32_t i = 0; i < size; ++i)
	{
		sx += vertices[i].x;
		sy += vertices[i].y;
	}

	return Point2f(sx / size, sy / size);
}

void Polyline2f::CopyFrom(const Polyline2f& rhs)
{
	if (rhs.size == 0)
		return;

	Reserve(rhs.size);

	for (uint32_t i = 0; i < rhs.size; ++i)
		vertices[i] = rhs.vertices[i];

	size = rhs.size;
}

void Polyline2f::MoveFrom(Polyline2f& rhs)
{
	vertices = rhs.vertices;
	size = rhs.size;
	capacity = rhs.capacity;

	rhs.vertices = nullptr;
	rhs.size = 0;
	rhs.capacity = 0;
}
