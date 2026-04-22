#include "Polygon2f.h"
#include <float.h>
#include <math.h>

using Core::ShapeType::Point2f;
using Core::ShapeType::Polygon2f;
using Core::ShapeType::Rect2f;

Polygon2f::Polygon2f(Point2f* points, size_t pointCount)
{
	if (!points || pointCount == 0)
		return;

	Reserve(pointCount);

	for (size_t i = 0; i < pointCount; ++i)
		vertices[i] = points[i];

	size = pointCount;
}

Polygon2f::Polygon2f(const Polygon2f& rhs)
{
	CopyFrom(rhs);
}

Polygon2f::Polygon2f(Polygon2f&& rhs) noexcept
{
	MoveFrom(rhs);
}

Polygon2f& Polygon2f::operator=(const Polygon2f& rhs)
{
	if (this != &rhs)
	{
		Clear();
		CopyFrom(rhs);
	}
	return *this;
}

Polygon2f& Polygon2f::operator=(Polygon2f&& rhs) noexcept
{
	if (this != &rhs)
	{
		Clear();
		MoveFrom(rhs);
	}
	return *this;
}

Polygon2f::~Polygon2f()
{
	Clear();
}

bool Polygon2f::Reserve(size_t newCapacity)
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

bool Polygon2f::AddVertex(const vertex_type& point)
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

void Polygon2f::Clear() noexcept
{
	if (vertices)
	{
		delete[] vertices;
		vertices = nullptr;
	}

	size = 0;
	capacity = 0;
}

size_t Polygon2f::Size() const noexcept
{
	return size;
}

bool Polygon2f::Empty() const noexcept
{
	return size == 0;
}

bool Polygon2f::IsValid() const noexcept
{
	return size >= 3;
}

const Polygon2f::vertex_type* Polygon2f::GetVertices() const noexcept
{
	return vertices;
}

Polygon2f::vertex_type* Polygon2f::GetVertices() noexcept
{
	return vertices;
}

float Polygon2f::Area() const noexcept
{
	if (!IsValid())
		return 0.0f;

	float area = 0.0f;

	for (size_t i = 0; i < size; ++i)
	{
		const auto& p1 = vertices[i];
		const auto& p2 = vertices[(i + 1) % size];

		area += (p1.x * p2.y - p2.x * p1.y);
	}

	return 0.5f * static_cast<float>(fabs(area));
}

Rect2f Polygon2f::GetBoundingBox() const noexcept
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

Point2f Polygon2f::GetCentroid() const noexcept
{
	if (!IsValid())
		return Point2f();

	float cx = 0.0f;
	float cy = 0.0f;
	float factor;
	float area = 0.0f;

	for (size_t i = 0; i < size; ++i)
	{
		const auto& p1 = vertices[i];
		const auto& p2 = vertices[(i + 1) % size];

		factor = (p1.x * p2.y - p2.x * p1.y);

		cx += (p1.x + p2.x) * factor;
		cy += (p1.y + p2.y) * factor;
		area += factor;
	}

	area *= 0.5f;

	if (fabs(area) < 1e-7f)
		return Point2f();

	cx /= (6.0f * area);
	cy /= (6.0f * area);

	return Point2f(cx, cy);
}

bool Polygon2f::PointInPolygon(const Point2f& point) const noexcept
{
	if (!IsValid())
		return false;

	bool inside = false;
	size_t n = size;

	for (size_t i = 0, j = n - 1; i < n; j = i++)
	{
		const auto& vi = vertices[i];
		const auto& vj = vertices[j];

		bool intersect =
			((vi.y > point.y) != (vj.y > point.y)) &&
			(point.x < (vj.x - vi.x) * (point.y - vi.y) /
				(vj.y - vi.y + 1e-8f) + vi.x);

		if (intersect)
			inside = !inside;
	}

	return inside;
}

void Polygon2f::CopyFrom(const Polygon2f& rhs)
{
	if (rhs.size == 0)
		return;

	Reserve(rhs.size);

	for (uint32_t i = 0; i < rhs.size; ++i)
		vertices[i] = rhs.vertices[i];

	size = rhs.size;
}

void Polygon2f::MoveFrom(Polygon2f& rhs)
{
	vertices = rhs.vertices;
	size = rhs.size;
	capacity = rhs.capacity;

	rhs.vertices = nullptr;
	rhs.size = 0;
	rhs.capacity = 0;
}
