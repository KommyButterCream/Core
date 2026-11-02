#pragma once

#include <stdint.h>
#include <memory.h>
#include <stdlib.h>

#include "../ShapeType/Rect2i.h"

enum class PixelType : uint8_t
{
	U8,
	U16,
	F32
};

struct Pixel8UC2
{
	uint8_t v[2];
};

struct Pixel8UC3
{
	uint8_t v[3];
};

struct Pixel8UC4
{
	uint8_t v[4];
};

enum class PixelValueFormat : uint8_t
{
	Integer,
	Float
};

struct PixelValue
{
	PixelValueFormat format;
	union
	{
		int64_t i;
		double f;
	};
};

namespace Core
{
	namespace ImageType
	{
		class ImageBase
		{
		public:
			// < Constructor & Destructor >
			ImageBase() = default;
			virtual ~ImageBase() { ReleaseBuffer(); };

			ImageBase(const ImageBase& rhs);
			ImageBase& operator=(const ImageBase& rhs);

			ImageBase(ImageBase&& rhs) noexcept;
			ImageBase& operator=(ImageBase&& rhs) noexcept;

			// ROI
			ImageBase(const ImageBase& src, const Core::ShapeType::Rect2i& roi);
			ImageBase ROI(const Core::ShapeType::Rect2i& roi) const;

		public:
			// < Methods >
			inline int32_t Width() const { return m_width; };
			inline int32_t Height() const { return m_height; };
			inline int32_t Stride() const { return m_stride; };

			inline uint8_t* ImageBuffer() { return m_data; };
			inline const uint8_t* ImageBuffer() const { return m_data; };

			inline uint8_t* Ptr(int32_t y) { return m_data + y * m_stride; };
			inline const uint8_t* Ptr(int32_t y) const { return m_data + y * m_stride; };

			inline PixelType GetPixelType() const { return m_pixelType; };
			inline int32_t Channel() const { return m_channel; };

			inline bool IsEmpty() const { return m_data == nullptr; };
			inline bool IsInside(int32_t x, int32_t y) const { return (x >= 0 && x < m_width) && (y >= 0 && y < m_height); };
			inline bool IsContinuous() const
			{
				return m_stride == m_width * (m_channel * (m_bitsPerChannel / 8));
			}

			void ReleaseBuffer();

			void Attach(void* externalBuffer, int32_t width, int32_t height, int32_t stride, int32_t channel, int32_t bitsPerChannel);

		protected:
			bool Allocate(int32_t width, int32_t height, int32_t channel, int32_t bitsPerChannel, int32_t stride = 0);

			void AddRef();
			void Release();

		private:
			static constexpr size_t IMAGE_ALIGNMENT = 32;
			inline int32_t AlignStride(int32_t rowBytes)
			{
				constexpr int32_t ALIGN = IMAGE_ALIGNMENT;
				return (rowBytes + ALIGN - 1) / ALIGN * ALIGN;
			}

			inline void* AlignedAlloc(size_t size)
			{
				return ::_aligned_malloc(size, IMAGE_ALIGNMENT);
			}

			inline void AlignedFree(void* ptr)
			{
				::_aligned_free(ptr);
			}

		protected:
			uint8_t* m_data = nullptr;
			int32_t m_width = 0;
			int32_t m_height = 0;
			int32_t m_stride = 0;
			int32_t m_bitsPerChannel = 0;
			int32_t m_channel = 0;
			PixelType m_pixelType = PixelType::U8;

			bool m_ownBuffer = false;
			int32_t* m_referenceCount = nullptr;
		};
	}
}
